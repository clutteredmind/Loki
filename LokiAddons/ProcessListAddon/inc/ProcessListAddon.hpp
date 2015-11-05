// 
// Purpose: Declares the node addon that deals with the Windows process list
//

#pragma once

#include "LokiAddonBase.hpp"

namespace Loki
{
   class ProcessListAddon : public LokiAddonBase
   {
      public:
      // constructor
      ProcessListAddon() = default;
      // destructor
      virtual ~ProcessListAddon() override = default;
      // Initialization. This function is required by node.
      static void Initialize(v8::Handle <v8::Object> target);
      // Creates a new instance of this class.
      static void Create(const v8::FunctionCallbackInfo<v8::Value>& args);
      // Gets a list of all running processes with their associated PIDs. Exposed to JavaScript.
      static void GetProcesses(const v8::FunctionCallbackInfo<v8::Value>& args);

      private:
      // The v8 constructor
      static v8::Persistent<v8::Function> constructor;
      // Gets a list of all running processes with their associated PIDs.
      v8::Local<v8::Array> getProcesses();
   };
}
