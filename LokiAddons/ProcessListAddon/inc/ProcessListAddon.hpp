// 
// Purpose: Declares the node addon that deals with the Windows process list
//

#pragma once

#include "LokiAddonBase.hpp"

#include <node.h>

class ProcessListAddon : LokiAddonBase
{
   public:
   // this addon's name
   static const std::string AddonName;
   // Initialization.  This function is required by node.
   static void init(v8::Handle <v8::Object> target);

   private:
#pragma region Addon logic

   // constructor
   ProcessListAddon();
   // destructor
   ~ProcessListAddon();
   // describes this object in JSON
   virtual v8::Local<v8::Object> describe() override;
   // query the Windows API for a list of running processes
   v8::Local<v8::Array> getProcessList();

#pragma endregion // Addon logic

#pragma region Node.js functions

   // gets a list of all running processes with their associated PIDs
   static void getProcesses(const v8::FunctionCallbackInfo<v8::Value>& args);

#pragma endregion // Node.js functions
};
