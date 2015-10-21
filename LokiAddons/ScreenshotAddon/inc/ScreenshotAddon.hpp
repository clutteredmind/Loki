// 
// Purpose: Declares the node addon that deals with taking a screenshot
//

#pragma once

#include "LokiAddonBase.hpp"

class ScreenshotAddon : public LokiAddonBase
{
   public:
   // constructor
   ScreenshotAddon();
   // destructor
   virtual ~ScreenshotAddon() override;
   // Initialization. This function is required by node.
   static void Initialize(v8::Handle <v8::Object> target);
   // Creates a new instance of this class.
   static void Create(const v8::FunctionCallbackInfo<v8::Value>& args);
   // Takes a screenshot via the Windows API. Exposed to JavaScript.
   static void CaptureScreen(const v8::FunctionCallbackInfo<v8::Value>& args);

   private:
   // Takes a screenshot via the Windows API.
   v8::Local<v8::Object> captureScreen();
};