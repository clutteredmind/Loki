// 
// Purpose: Defines the node module that deals with taking a screenshot
//

#include "ScreenshotModule.hpp"

#include "jpge.h" // for JPEG compression

#include <Windows.h>

#include <node_buffer.h>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Tell node which function to use to set up this module
NODE_MODULE (ScreenshotModule, Loki::ScreenshotModule::Initialize)

namespace Loki
{
   // The v8 constructor from the template class
   Persistent<Function> ScreenshotModule::constructor;

   // The template class's descriptor object
   LokiModuleDescriptor ScreenshotModule::descriptor;

   // module metadata
   const std::string MODULE_NAME = "ScreenshotModule";
   const std::string MODULE_DISPLAY_NAME = "Screenshot";
   const int MODULE_VERSION[3]{ 1 /*major*/, 0 /*minor*/, 0 /*patch*/ };
   const std::string MODULE_DESCRIPTION = "Grabs a screenshot of the display via the Windows API.";

   // Initialization function used by Node to set up this module.
   void ScreenshotModule::Initialize (Handle<Object> target)
   {
      // set module metadata
      descriptor.SetMetadata (MODULE_NAME, MODULE_DISPLAY_NAME, LokiModuleDescriptor::GetVersionStringFromArray (MODULE_VERSION), MODULE_DESCRIPTION);
      // register this class's exported functions for the framework
      descriptor.AddFunction ("captureScreen", CaptureScreen, "Takes a screenshot via the Windows API.", { LOKI_PARAMETER (ParameterType::FUNCTION, "callback") });
      // Register module with Node
      Register (target);
   }

   // Takes a screenshot via the Windows API. Exposed to JavaScript.
   void ScreenshotModule::CaptureScreen (const FunctionCallbackInfo<Value>& args)
   {
      auto isolate = args.GetIsolate ();
      HandleScope scope (isolate);

      // set default return value
      args.GetReturnValue ().Set (Undefined (isolate));

      try
      {
         // validate parameters
         std::string error_string;
         if (descriptor.ValidateParameters (CaptureScreen, args, error_string))
         {
            // unwrap object so we can call the correct function on the instance
            auto screenshot_module = ObjectWrap::Unwrap<ScreenshotModule> (args.Holder ());

            // the screen buffer
            std::vector<uint8_t> screen_buffer;
            // if it doesn't work, report the error
            if (!screenshot_module->captureScreen (screen_buffer))
            {
               std::string error = "Unable to capture screenshot";
               std::string last_error_string;
               if (screenshot_module->GetLastErrorString (last_error_string))
               {
                  // append more specific error message
                  error += ": ";
                  error += last_error_string;
                  throw std::exception (error.c_str ());
               }
            }
            // hand screen buffer back to JavaScript
            auto screen_data = node::Buffer::Copy (isolate, reinterpret_cast<const char*>(screen_buffer.data ()), screen_buffer.size ()).ToLocalChecked ();

            // Assemble the argument array for the callback
            const unsigned argc = 1;
            Local<Value> argv[argc] = { screen_data };

            // Get the callback
            Local<Function> callback = Local <Function>::Cast (args[0]);
            // Call it
            callback->Call (isolate->GetCurrentContext ()->Global (), argc, argv);
         }
         else
         {
            // if parameter validation failed for whatever reason, report the error
            throw std::exception (error_string.c_str ());
         }
      }
      catch (std::exception& exception)
      {
         isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, exception.what ())));
      }
   }

   // Takes a screenshot via the Windows API.
   bool ScreenshotModule::captureScreen (std::vector<uint8_t>& screen_buffer)
   {
      bool success = true;

      // get display dimensions
      int screen_x = GetSystemMetrics (SM_XVIRTUALSCREEN);
      int screen_y = GetSystemMetrics (SM_YVIRTUALSCREEN);
      int screen_width = GetSystemMetrics (SM_CXVIRTUALSCREEN);
      int screen_height = GetSystemMetrics (SM_CYVIRTUALSCREEN);

      // clear target buffer
      screen_buffer.clear ();

      // objects for working with the screen through the Windows API
      HDC screen_device_context = NULL;
      HDC compatible_device_context = NULL;
      HBITMAP bitmap_handle = NULL;
      // Bitmap format info. Used by GetDIBits.
      LPBITMAPINFO bitmap_info = NULL;
      // memory buffers for JPEG compression
      uint8_t* image_data = NULL;
      uint8_t* image_data_compressed = NULL;

      try
      {
         // get a device context for the whole screen
         screen_device_context = CreateDC (L"DISPLAY", NULL, NULL, 0);
         if (screen_device_context == NULL)
         {
            throw std::exception ("captureScreen: Unable to get screen device context");
         }

         // get a memory device context compatible with the DC acquired above
         compatible_device_context = CreateCompatibleDC (screen_device_context);
         if (compatible_device_context == NULL)
         {
            throw std::exception ("captureScreen: Unable to create compatible device context");
         }

         // From MSDN: create a bitmap compatible with the device that is associated with the specified device context
         bitmap_handle = CreateCompatibleBitmap (screen_device_context, screen_width, screen_height);
         if (bitmap_handle == NULL)
         {
            throw std::exception ("captureScreen: Unable to create compatible bitmap");
         }

         // select the bitmap into the device context
         if (SelectObject (compatible_device_context, bitmap_handle) == NULL)
         {
            throw std::exception ("captureScreen: Unable to select bitmap object");
         }

         // copy color data from the screen DC into the compatible DC
         if (BitBlt (compatible_device_context, 0, 0, screen_width, screen_height, screen_device_context, screen_x, screen_y, SRCCOPY) == NULL)
         {
            throw std::exception ("captureScreen: Unable to capture the screen");
         }

         // Reserve memory for bitmap info (BITMAPINFOHEADER + largest possible palette):
         bitmap_info = reinterpret_cast<LPBITMAPINFO> (new char[sizeof (BITMAPINFOHEADER) + 256 * sizeof (RGBQUAD)]);
         if (bitmap_info == NULL)
         {
            throw std::exception ("captureScreen: Failed to allocate memory for bitmap");
         }
         ZeroMemory (&bitmap_info->bmiHeader, sizeof (BITMAPINFOHEADER));
         bitmap_info->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);

         // GetDIBits requires format info about the bitmap.
         // We can have GetDIBits fill a structure with that info if we pass NULL for lpvBits.
         if (!GetDIBits (compatible_device_context, bitmap_handle, 0, screen_height /* lpvBits */, NULL, bitmap_info, DIB_RGB_COLORS))
         {
            throw std::exception ("captureScreen: Unable to get bitmap information");
         }

         // The raw screen buffer. This will contain RGB+alpha information
         std::vector<uint8_t> raw_screen_buffer;
         raw_screen_buffer.resize (screen_width * screen_height * 4); // Width * Pixels * 4 bytes per pixel, RGB + alpha
                                                                     // Now that we have all of the bitmap's information, we pass a pointer to the beginning of
                                                                     // raw_screen_buffer for GetDIBits to fill it in with bitmap data
         if (!GetDIBits (compatible_device_context, bitmap_handle, 0, screen_height, &raw_screen_buffer[0], bitmap_info, DIB_RGB_COLORS))
         {
            throw std::exception ("captureScreen: Unable to copy screen capture bitmap");
         }

         // calculate the size that the image buffers need to be
         int buffer_size = screen_width * screen_height * 3;
         // image_data needs to be big enough to hold all the bitmap data
         image_data = new uint8_t[buffer_size];
         // image_data_compressed only needs to be big enough for the compressed version of the image,
         // but we can't know how big that will be until after compression has been performed.
         // So we're allocating more space than will be needed in the end.
         image_data_compressed = new uint8_t[buffer_size];
         // The final screen buffer. This will omit the alpha channel, so we allocate a bit less space
         std::vector<uint8_t> final_screen_buffer;
         final_screen_buffer.resize (buffer_size);
         // Convert the raw screen buffer to the final buffer.
         for (int line = 0; line < screen_height; line++)
         {
            for (int column = 0; column < screen_width; column++)
            {
               int raw_pixel = (line * screen_width) + column;
               int final_pixel = ((screen_height - line - 1) * screen_width) + column;
               image_data[final_pixel * 3 + 2] = final_screen_buffer[final_pixel * 3] = raw_screen_buffer[raw_pixel * 4];
               image_data[final_pixel * 3 + 1] = final_screen_buffer[final_pixel * 3 + 1] = raw_screen_buffer[raw_pixel * 4 + 1];
               image_data[final_pixel * 3] = final_screen_buffer[final_pixel * 3 + 2] = raw_screen_buffer[raw_pixel * 4 + 2];
            }
         }
         // compress_image_to_jpeg_file_in_memory will reset buffer_size to the size of the compressed data if it is successful
         if (!jpge::compress_image_to_jpeg_file_in_memory (image_data_compressed, buffer_size, screen_width, screen_height, 3, image_data))
         {
            throw std::exception ("captureScreen: Unable to compress bitmap to JPEG");
         }
         // set size of target buffer
         screen_buffer.resize (buffer_size);
         // copy compressed memory into return buffer
         std::copy (image_data_compressed, image_data_compressed + buffer_size, screen_buffer.begin ());
      }
      catch (std::exception& exception)
      {
         // save the error message
         SetLastErrorString (std::string (exception.what ()));
         // clear the screen buffer
         screen_buffer.clear ();
         // set failed flag
         success = false;
      }

      // clean up memory, where applicable
      if (NULL != compatible_device_context)
      {
         DeleteDC (compatible_device_context);
      }
      if (NULL != bitmap_handle)
      {
         DeleteObject (bitmap_handle);
      }
      if (NULL != bitmap_info)
      {
         delete bitmap_info;
      }
      if (NULL != image_data)
      {
         delete image_data;
      }
      if (NULL != image_data_compressed)
      {
         delete image_data_compressed;
      }

      return success;
   }
}
