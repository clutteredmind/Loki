// 
// Purpose: Declares the node module that deals with the Windows process list
//

#pragma once

#include "LokiModule.hpp"

namespace Loki
{
   class ProcessListModule : public LokiModule<ProcessListModule>
   {
      public:
      // Constructor
      ProcessListModule() = default;
      // destructor
      virtual ~ProcessListModule() override = default;
      // Initialization function used by Node to set up this module.
      static void Initialize(v8::Handle <v8::Object> target);
      // Gets a list of all running processes with their associated PIDs. Exposed to JavaScript.
      static void GetProcesses(const v8::FunctionCallbackInfo<v8::Value>& args);

      protected:
      // Gets a list of all running processes with their associated PIDs.
      v8::Local<v8::Array> getProcesses(v8::Isolate* isolate);
   };
}
