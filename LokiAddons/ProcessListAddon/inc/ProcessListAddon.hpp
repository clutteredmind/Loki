// 
// Purpose: Declares the node addon that deals with the Windows process list
//

#pragma once

#include "LokiAddonBase.hpp"

class ProcessListAddon : LokiAddonBase
{
   public:
   // constructor
   ProcessListAddon();
   // destructor
   virtual ~ProcessListAddon() override;
   // Initialization. This function is required by node.
   static void Initialize(v8::Handle <v8::Object> target);
   // Creates a new instance of this class.
   static void Create(const v8::FunctionCallbackInfo<v8::Value>& args);
   // gets a list of all running processes with their associated PIDs
   static void GetProcesses(const v8::FunctionCallbackInfo<v8::Value>& args);

   private:
   // v8 constructor
   static v8::Persistent<v8::Function> constructor;
   // query the Windows API for a list of running processes
   v8::Local<v8::Array> getProcesses();
};
