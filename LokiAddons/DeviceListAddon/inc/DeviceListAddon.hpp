// 
// Purpose: Declares the node addon that deals with the Windows device list
//

#pragma once

#include "LokiAddon.hpp"

namespace Loki
{
   class DeviceListAddon : public LokiAddon<DeviceListAddon>
   {
      public:
      // Constructor
      DeviceListAddon() = default;
      // destructor
      virtual ~DeviceListAddon() override = default;
      // Initialization function used by Node to set up this addon.
      static void Initialize(v8::Handle <v8::Object> target);
      // Gets a list of all installed devices. Exposed to JavaScript.
      static void GetDevices(const v8::FunctionCallbackInfo<v8::Value>& args);

      protected:
      // Gets a list of all installed devices.
      v8::Local<v8::Array> getDevices(v8::Isolate* isolate);
   };
}
