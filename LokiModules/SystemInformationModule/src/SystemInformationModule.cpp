//
// Purpose: Defines a Node module
//

#include "SystemInformationModule.hpp"

#include <Windows.h>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Tell node which function to use to set up this module
NODE_MODULE(SystemInformationModule, Loki::SystemInformationModule::Initialize)

namespace Loki
{
   // The v8 constructor from the template class
   Persistent<Function> SystemInformationModule::constructor;

   // The template class's descriptor object
   LokiModuleDescriptor SystemInformationModule::descriptor;

   // module metadata
   const std::string module_name = "SystemInformationModule";
   const int module_version [3] {1 /*major*/, 0 /*minor*/, 0 /*patch*/};
   const std::string module_description = "System Information Module";

   // for converting bytes into kilobytes
   const int DIVISOR = 1024;

   // Pre-initialization.
   void SystemInformationModule::Initialize(Handle<Object> target)
   {
      // set module metadata
      descriptor.SetName(module_name);
      descriptor.SetVersion(LokiModuleDescriptor::GetVersionStringFromArray(module_version));
      descriptor.SetDescription(module_description);
      // register this class's exported functions for the framework
      descriptor.AddFunction ("getMemoryInformation", GetMemoryInformation, "Queries Windows for memory information.", NO_PARAMETERS, RETURNS_AN OBJECT);
      // Register module with Node
      Register(target);
   }

   // Gets RAM information. Exposed to JavaScript.
   void SystemInformationModule::GetMemoryInformation (const FunctionCallbackInfo<Value>& args)
   {
      auto isolate = args.GetIsolate ();
      HandleScope scope (isolate);

      // validate parameters
      std::string error_string;
      if (descriptor.ValidateParameters (GetMemoryInformation, args, error_string))
      {
         // unwrap object so we can call the correct function on the instance
         auto process_list_module = ObjectWrap::Unwrap<SystemInformationModule> (args.Holder ());
         // return process list to caller
         args.GetReturnValue ().Set (process_list_module->getMemoryInformation (isolate));
      }
      else
      {
         // if parameter validation failed for whatever reason, report the error
         isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, error_string.c_str ())));
      }
   }

   // Gets memory information.
   Local<Object> SystemInformationModule::getMemoryInformation (Isolate* isolate)
   {
      HandleScope scope (isolate);

      MEMORYSTATUSEX statex;
      statex.dwLength = sizeof (statex);

      GlobalMemoryStatusEx (&statex);

      // the array of devices to return to JavaScript
      auto memoryInfo = Object::New (isolate);

      memoryInfo->Set (String::NewFromUtf8 (isolate, "memoryLoad"), Integer::New (isolate, statex.dwMemoryLoad));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "totalPhysicalMemory"), Integer::New (isolate, statex.ullTotalPhys / DIVISOR));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "freePhysicalMemory"), Integer::New (isolate, statex.ullAvailPhys / DIVISOR));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "totalPageFileSize"), Integer::New (isolate, statex.ullTotalPageFile / DIVISOR));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "freePageFileMemory"), Integer::New (isolate, statex.ullAvailPageFile / DIVISOR));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "totalVirtualMemory"), Integer::New (isolate, statex.ullTotalVirtual / DIVISOR));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "freeVirtualMemory"), Integer::New (isolate, statex.ullAvailVirtual / DIVISOR));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "availableExtendedMemory"), Integer::New (isolate, statex.ullAvailExtendedVirtual / DIVISOR));

      return memoryInfo;
   }
}
