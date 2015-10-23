// 
// Purpose: Defines the node addon that deals with taking a screenshot
//

#include "ScreenshotAddon.hpp"

#include <Windows.h>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Do all the magic to make this module accessible by node/javascript
// THE FIRST PARAMETER TO THE MACRO BELOW MUST MATCH THE MODULE FILENAME
NODE_MODULE(ScreenshotAddon, ScreenshotAddon::Initialize)

// Initialization. This function is required by node.
void ScreenshotAddon::Initialize(Handle<Object> target)
{
   auto isolate = Isolate::GetCurrent();

   // Prepare constructor template
   auto function_template = FunctionTemplate::New(isolate, Create);
   function_template->SetClassName(String::NewFromUtf8(isolate, "ScreenshotAddon"));
   function_template->InstanceTemplate()->SetInternalFieldCount(1);

   // Export functions to JavaScript
   NODE_SET_PROTOTYPE_METHOD(function_template, "captureScreen", CaptureScreen);

   constructor.Reset(isolate, function_template->GetFunction());
   target->Set(String::NewFromUtf8(isolate, "ScreenshotAddon"), function_template->GetFunction());
}

// Creates a new instance of this class.
void ScreenshotAddon::Create(const FunctionCallbackInfo<Value>& args)
{
   auto isolate = args.GetIsolate();

   if (args.IsConstructCall())
   {
      // Invoked as constructor: new MyObject(...)
      auto screenshot_addon = new ScreenshotAddon();
      screenshot_addon->Wrap(args.This());
      args.GetReturnValue().Set(args.This());
   }
   else
   {
      // Invoked as plain function `MyObject(...)`, turn into construct call.
      auto ctor = Local<Function>::New(isolate, constructor);
      args.GetReturnValue().Set(ctor->NewInstance());
   }
}

// Takes a screenshot via the Windows API. Exposed to JavaScript.
void ScreenshotAddon::CaptureScreen(const FunctionCallbackInfo<Value>& args)
{
   auto isolate = args.GetIsolate();

   // set default return value
   args.GetReturnValue().Set(Undefined(isolate));

   try
   {
      // sanity-check arguments
      if (args.Length() != 1)
      {
         throw std::exception("Invalid parameter count. Expected one function parameter.");
      }
      if (!args [0]->IsFunction())
      {
         throw std::exception("The parameter must be a function.");
      }

      // unwrap object so we can call the correct function on the instance
      auto screenshot_addon = ObjectWrap::Unwrap<ScreenshotAddon>(args.Holder());

      // the screen buffer
      std::vector<uint8_t> screen_buffer;
      // if it doesn't work, report the error
      if (!screenshot_addon->captureScreen(screen_buffer))
      {
         std::string error = "Unable to capture screenshot";
         std::string last_error_string;
         if (screenshot_addon->GetLastErrorString(last_error_string))
         {
            // append more specific error message
            error += ": ";
            error += last_error_string;
            throw std::exception(error.c_str());
         }
      }
      // TODO: hand screen buffer back to JavaScript somehow

      // get the callback function
      auto callback = Local <Function>::Cast(args [0]);
      // call it
      // TODO: argc and argv will need to be set to some real data here
      callback->Call(isolate->GetCurrentContext()->Global(), 0, {});
   }
   catch (std::exception& exception)
   {
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, exception.what())));
   }
}

// Takes a screenshot via the Windows API.
bool ScreenshotAddon::captureScreen(std::vector<uint8_t>& screen_buffer)
{
   bool success = true;

   // get screen dimensions
   int screen_x = GetSystemMetrics(SM_XVIRTUALSCREEN);
   int screen_y = GetSystemMetrics(SM_YVIRTUALSCREEN);
   int screen_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
   int screen_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
   // resize the screen buffer to accomodate enough data
   screen_buffer.resize(screen_width * screen_height * 4);
   // objects for working with the Windows API
   LPBITMAPINFO bitmap_info = NULL;
   HDC compatible_device_context = NULL;
   HDC screen_device_context = NULL;
   HBITMAP bitmap_handle = NULL;

   try
   {
      // Get a DC compatible with the screen
      compatible_device_context = CreateCompatibleDC(0);
      if (compatible_device_context == NULL)
      {
         throw std::exception("captureScreen: Unable to create compatible device context");
      }

      screen_device_context = GetDC(0);
      if (screen_device_context == NULL)
      {
         throw std::exception("captureScreen: Unable to get screen device context");
      }
      // make a bmp in memory to store the capture in
      bitmap_handle = CreateCompatibleBitmap(screen_device_context, screen_width, screen_height);
      if (bitmap_handle == NULL)
      {
         throw std::exception("captureScreen: Unable to create compatible bitmap");
      }

      // join em up
      HGDIOBJ previous_object(SelectObject(compatible_device_context, bitmap_handle));
      if (previous_object == NULL)
      {
         throw std::exception("captureScreen: Unable to select bitmap object");
      }

      // copy from the screen to bitmap
      if (BitBlt(compatible_device_context, 0, 0, screen_width, screen_height, screen_device_context, screen_x, screen_y, SRCCOPY) == NULL)
      {
         throw std::exception("captureScreen: Unable to capture the screen");
      }

      // GetDIBits requires format info about the bitmap. We can have GetDIBits
      // fill a structure with that info if we pass a NULL pointer for lpvBits:
      // Reserve memory for bitmap info (BITMAPINFOHEADER + largest possible
      // palette):
      if ((bitmap_info = (LPBITMAPINFO) (new char [sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)])) == NULL)
      {
         throw std::exception("captureScreen: Failed to allocate memory for bitmap");
      }
      ZeroMemory(&bitmap_info->bmiHeader, sizeof(BITMAPINFOHEADER));
      bitmap_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

      // Get info
      if (!GetDIBits(compatible_device_context, bitmap_handle, 0, screen_height, NULL, bitmap_info, DIB_RGB_COLORS))
      {
         throw std::exception("captureScreen: Unable to get bitmap information");
      }
      // Get the bits out
      if (!GetDIBits(compatible_device_context, bitmap_handle, 0, screen_height, &screen_buffer [0], bitmap_info, DIB_RGB_COLORS))
      {
         throw std::exception("captureScreen: Unable to copy screen capture bitmap");
      }
   }
   catch (std::exception& exception)
   {
      // save the error message
      SetLastErrorString(std::string(exception.what()));
      // clear the screen buffer before sending it back
      screen_buffer.clear();
      // set failed flag
      success = false;
   }

   // free dynamic memory
   ReleaseDC(NULL, compatible_device_context);
   ReleaseDC(NULL, screen_device_context);
   DeleteObject(bitmap_handle);
   if (bitmap_info)
   {
      delete bitmap_info;
   }

   return success;
}