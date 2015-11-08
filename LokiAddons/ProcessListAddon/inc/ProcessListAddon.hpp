// 
// Purpose: Declares the node addon that deals with the Windows process list
//

#pragma once

#include "LokiAddon.hpp"

namespace Loki
{
   class ProcessListAddon : public LokiAddon<ProcessListAddon>
   {
      public:
      // Constructor
      ProcessListAddon() = default;
      // destructor
      virtual ~ProcessListAddon() override = default;
      // Pre-initialization.
      static void PreInitialize(v8::Handle <v8::Object> target);
      // Gets a list of all running processes with their associated PIDs. Exposed to JavaScript.
      static void GetProcesses(const v8::FunctionCallbackInfo<v8::Value>& args);

      private:
      // Gets a list of all running processes with their associated PIDs.
      v8::Local<v8::Array> getProcesses();
   };
}
