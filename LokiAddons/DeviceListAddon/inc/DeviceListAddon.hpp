// 
// Purpose: Declares the node addon that deals with the Windows device list
//

#pragma once

#include "LokiAddonBase.hpp"

class DeviceListAddon : LokiAddonBase
{
   public:
   // constructor
   DeviceListAddon();
   // destructor
   virtual ~DeviceListAddon() override;
   // Initialization. This function is required by node.
   static void Initialize(v8::Handle <v8::Object> target);
   // Creates a new instance of this class.
   static void Create(const v8::FunctionCallbackInfo<v8::Value>& args);
   // gets a list of all installed devices
   static void GetDevices(const v8::FunctionCallbackInfo<v8::Value>& args);

   private:
   // v8 constructor
   static v8::Persistent<v8::Function> constructor;
   // query the Windows API for a list of devices
   v8::Local<v8::Array> getDevices();
};
