// 
// Purpose: Declares the node module that deals with taking a screenshot
//

#pragma once

#include "LokiModule.hpp"

namespace Loki
{
   class ScreenshotModule : public LokiModule<ScreenshotModule>
   {
   public:
      // Constructor
      ScreenshotModule () = default;
      // destructor
      virtual ~ScreenshotModule () override = default;
      // Initialization function used by Node to set up this module.
      static void Initialize (v8::Handle <v8::Object> target);
      // Takes a screenshot via the Windows API. Exposed to JavaScript.
      static void CaptureScreen (const v8::FunctionCallbackInfo<v8::Value>& args);

   protected:
      // Takes a screenshot via the Windows API and saves it to disk.
      bool captureScreen (const std::string full_path);
   };
}
