// 
// Purpose: Declares the node addon that deals with taking a screenshot
//

#pragma once

#include "LokiAddon.hpp"

// standard includes
#include <vector>

namespace Loki
{
   class ScreenshotAddon : public LokiAddon<ScreenshotAddon>
   {
      public:
      // constructor
      ScreenshotAddon() = default;
      // destructor
      virtual ~ScreenshotAddon() override = default;
      // Pre-initialization.
      static void PreInitialize(v8::Handle <v8::Object> target);
      // Takes a screenshot via the Windows API. Exposed to JavaScript.
      static void CaptureScreen(const v8::FunctionCallbackInfo<v8::Value>& args);

      private:
      // Takes a screenshot via the Windows API.
      bool captureScreen(std::vector<uint8_t>& screen_buffer);
   };
}
