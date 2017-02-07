//
// Purpose: Declares a Node module
//

#pragma once

#include "LokiModule.hpp"

namespace Loki
{
   class SystemInformationModule : public LokiModule<SystemInformationModule>
   {
      public:
      // Constructor
      SystemInformationModule() = default;
      // destructor
      virtual ~SystemInformationModule() override = default;
      // Initialization function used by Node to set up this module.
      static void Initialize(v8::Handle <v8::Object> target);
      // Gets memory information. Exposed to JavaScript.
      static void GetMemoryInformation (const v8::FunctionCallbackInfo<v8::Value>& args);
      // Gets CPU information. Exposed to JavaScript.
      static void GetCpuInformation (const v8::FunctionCallbackInfo<v8::Value>& args);

   protected:
      // Gets memory information.
      v8::Local<v8::Object> getMemoryInformation (v8::Isolate* isolate);
      // Gets CPU information.
      v8::Local<v8::Object> getCpuInformation (v8::Isolate* isolate);
   };
}
