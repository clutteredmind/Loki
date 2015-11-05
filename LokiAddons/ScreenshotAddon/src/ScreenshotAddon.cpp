// 
// Purpose: Defines the node addon that deals with taking a screenshot
//

#include "ScreenshotAddon.hpp"

#include <Windows.h>

#include <node_buffer.h>

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
      // hand screen buffer back to JavaScript
      auto return_value = node::Buffer::Copy(isolate, reinterpret_cast<const char*>(screen_buffer.data()), screen_buffer.size()).ToLocalChecked();

      auto callback = Local<Function>::Cast(args [0]);
      const int argc = 1;
      Handle<Value> argv [argc] = {return_value};
      callback->Call(isolate->GetCurrentContext()->Global(), argc, argv);
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

   // clear buffer
   screen_buffer.clear();

   HDC desktop_dc;
   HDC compatible_dc;
   HBITMAP bitmap;

   try
   {
      desktop_dc = CreateDC(L"DISPLAY", NULL, NULL, 0);
      compatible_dc = CreateCompatibleDC(desktop_dc);
      bitmap = CreateCompatibleBitmap(desktop_dc, screen_width, screen_height);
      SelectObject(compatible_dc, bitmap);
      BitBlt(compatible_dc, 0, 0, screen_width, screen_height, desktop_dc, screen_x, screen_y, SRCCOPY | CAPTUREBLT);

      // TODO: send this to JavaScript
      //copy to clipboard
      OpenClipboard(NULL);
      EmptyClipboard();
      SetClipboardData(CF_BITMAP, bitmap);
      CloseClipboard();
   }
   catch (std::exception& exception)
   {
      // save the error message
      SetLastErrorString(std::string(exception.what()));
      // clear the screen buffer
      screen_buffer.clear();
      // set failed flag
      success = false;
   }

   // free memory
   ReleaseDC(NULL, desktop_dc);
   DeleteDC(compatible_dc);
   DeleteObject(bitmap);

   return success;
}