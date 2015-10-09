// 
// Purpose: Declares the node addon that deals with the Windows device list
//

#pragma once

#include "LokiAddonBase.hpp"

#include <node.h>

class DeviceListAddon : LokiAddonBase
{
   public:
   // this addon's name
   static const std::string AddonName;
   // Initialization.  This function is required by node.
   static void init(v8::Handle <v8::Object> target);

#pragma region Node.js functions

   // gets a list of all installed devices
   static void GetDevices(const v8::FunctionCallbackInfo<v8::Value>& args);
   // exposes the describe function to JavaScript
   static void Describe(const v8::FunctionCallbackInfo<v8::Value>& args);

#pragma endregion // Node.js functions

   private:
#pragma region Addon logic

   // constructor
   DeviceListAddon();
   // destructor
   ~DeviceListAddon();
   // describes this object in JSON
   v8::Local<v8::Object> describe();
   // query the Windows API for a list of devices
   v8::Local<v8::Array> getDevices();

#pragma endregion // Addon logic
};
