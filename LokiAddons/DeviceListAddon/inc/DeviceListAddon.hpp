// 
// Purpose: Declares the node addon that deals with the Windows device list
//

#pragma once

#include "LokiAddonBase.hpp"

#include <node.h>

class DeviceListAddon : LokiAddonBase
{
   public:
   // Initialization.  This function is required by node.
   static void init(v8::Handle <v8::Object> target);

   private:
#pragma region Addon logic

   // constructor
   DeviceListAddon();
   // destructor
   ~DeviceListAddon();
   // query the Windows API for a list of devices
   v8::Local<v8::Array> getDevices();

#pragma endregion // Addon logic

#pragma region Node.js functions

   static void create(const v8::FunctionCallbackInfo<v8::Value>& args);

   // gets a list of all installed devices
   static void getDevices(const v8::FunctionCallbackInfo<v8::Value>& args);

   static v8::Persistent<v8::Function> constructor;

#pragma endregion // Node.js functions
};
