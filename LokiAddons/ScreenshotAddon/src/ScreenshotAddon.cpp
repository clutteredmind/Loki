// 
// Purpose: Defines the node addon that deals with taking a screenshot
//

#include "ScreenshotAddon.hpp"

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
      if (args.Length() < 1)
      {
         throw std::exception("captureScreen: Invalid parameter count. Expected: Parameter 0 'callback' - function");
      }
      if (!args [0]->IsFunction())
      {
         throw std::exception("captureScreen: Parameter 0 'callback' must be a function");
      }

      // unwrap object so we can call the correct function on the instance
      auto screenshot_addon = ObjectWrap::Unwrap<ScreenshotAddon>(args.Holder());
      // get the screen buffer
      auto buffer = screenshot_addon->captureScreen();
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
std::vector<uint8_t> ScreenshotAddon::captureScreen()
{
   std::vector<uint8_t> screen_buffer;

   // TODO: implement this
   
   return screen_buffer;
}