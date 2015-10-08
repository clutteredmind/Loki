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

   private:
#pragma region Addon logic

   // constructor
   DeviceListAddon();
   // destructor
   ~DeviceListAddon();
   // describes this object in JSON
   virtual v8::Local<v8::Object> describe() override;
   // query the Windows API for a list of devices
   v8::Local<v8::Array> getDeviceList();

#pragma endregion // Addon logic

#pragma region Node.js functions

   // gets a list of all installed devices
   static void getDevices(const v8::FunctionCallbackInfo<v8::Value>& args);

#pragma endregion // Node.js functions
};
