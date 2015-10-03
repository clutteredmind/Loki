// 
// Purpose: Declares the node addon that deals with the Windows process list
//

#pragma once

#include "LokiAddonBase.hpp"

#include <node.h>
#include <node_object_wrap.h>

class ProcessListAddon : LokiAddonBase
{
   public:
   // Initialization.  This function is required by node.
   static void init(v8::Handle <v8::Object> target);

   private:
#pragma region Addon logic

   ProcessListAddon();
   ~ProcessListAddon();
   // query the Windows API for a list of running processes
   v8::Local<v8::Array> getProcesses();

#pragma endregion // Addon logic

#pragma region Node.js functions

   static void create(const v8::FunctionCallbackInfo<v8::Value>& args);

   // gets a list of all running processes with their associated PIDs
   static void getProcesses(const v8::FunctionCallbackInfo<v8::Value>& args);

   static v8::Persistent<v8::Function> constructor;

#pragma endregion // Node.js functions
};
