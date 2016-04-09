// 
// Purpose: Declares the node module that deals with the Windows device list
//

#pragma once

#include "LokiModule.hpp"

namespace Loki
{
   class DeviceListModule : public LokiModule<DeviceListModule>
   {
      public:
      // Constructor
      DeviceListModule () = default;
      // destructor
      virtual ~DeviceListModule () override = default;
      // Initialization function used by Node to set up this module.
      static void Initialize (v8::Handle <v8::Object> target);
      // Gets a list of all installed devices. Exposed to JavaScript.
      static void GetDevices (const v8::FunctionCallbackInfo<v8::Value>& args);

      protected:
      // Gets a list of all installed devices.
      v8::Local<v8::Array> getDevices (v8::Isolate* isolate);
   };
}
