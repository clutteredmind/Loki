// 
// Purpose: Declares the node module that deals with the Windows process list
//

#pragma once

#include <node.h>
#include <node_object_wrap.h>

class ProcessListAddon : public node::ObjectWrap
{
   public:
   // Initialization.  This function is required by node.
   static void init(v8::Handle <v8::Object> target);

   private:
#pragma region Addon logic

   ProcessListAddon();
   virtual ~ProcessListAddon() override;

   v8::Local<v8::Array> getProcesses();

#pragma endregion // Addon logic

#pragma region Node.js functions

   static void create(const v8::FunctionCallbackInfo<v8::Value>& args);

   // gets a list of all running processes with their associated PIDs
   static void getProcesses(const v8::FunctionCallbackInfo<v8::Value>& args);

   static v8::Persistent<v8::Function> constructor;
#pragma endregion // Node.js functions
};
