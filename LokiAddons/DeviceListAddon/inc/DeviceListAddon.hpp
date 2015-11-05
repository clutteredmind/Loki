// 
// Purpose: Declares the node addon that deals with the Windows device list
//

#pragma once

#include "LokiAddonBase.hpp"

namespace Loki
{
   class DeviceListAddon : public LokiAddonBase
   {
      public:
      // constructor
      DeviceListAddon() = default;
      // destructor
      virtual ~DeviceListAddon() override = default;
      // Initialization. This function is required by node.
      static void Initialize(v8::Handle <v8::Object> target);
      // Creates a new instance of this class.
      static void Create(const v8::FunctionCallbackInfo<v8::Value>& args);
      // Gets a list of all installed devices. Exposed to JavaScript.
      static void GetDevices(const v8::FunctionCallbackInfo<v8::Value>& args);

      private:
      // The v8 constructor
      static v8::Persistent<v8::Function> constructor;
      // Gets a list of all installed devices.
      v8::Local<v8::Array> getDevices();
   };
}
