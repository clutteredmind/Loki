// 
// Purpose: Defines the node addon that deals with taking a screenshot
//

#include "ScreenshotAddon.hpp"

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Do all the magic to make this module accessible by node/javascript
// THE FIRST PARAMETER TO THE MACRO BELOW MUST MATCH THE MODULE FILENAME
NODE_MODULE(ScreenshotAddon, ScreenshotAddon::Initialize)

// constructor
ScreenshotAddon::ScreenshotAddon() {}

// destructor
ScreenshotAddon::~ScreenshotAddon() {}

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
   // unwrap object so we can call the correct function on the instance
   auto screenshot_addon = ObjectWrap::Unwrap<ScreenshotAddon>(args.Holder());
   // return process list to caller
   args.GetReturnValue().Set(screenshot_addon->captureScreen());
}

Local<Object> ScreenshotAddon::captureScreen()
{
   auto isolate = Isolate::GetCurrent();

   // TODO: implement this
   
   return Object::New(isolate);
}