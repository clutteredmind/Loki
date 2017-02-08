//
// Purpose: Defines a Node module
//

#include "SystemInformationModule.hpp"

#include <Windows.h>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Tell node which function to use to set up this module
NODE_MODULE (SystemInformationModule, Loki::SystemInformationModule::Initialize)

namespace Loki
{
   // The v8 constructor from the template class
   Persistent<Function> SystemInformationModule::constructor;

   // The template class's descriptor object
   LokiModuleDescriptor SystemInformationModule::descriptor;

   // module metadata
   const std::string module_name = "SystemInformationModule";
   const int module_version[3]{ 1 /*major*/, 0 /*minor*/, 0 /*patch*/ };
   const std::string module_description = "System Information Module";

   // for converting bytes into kilobytes
   const int DIVISOR = 1024;

   // Pre-initialization.
   void SystemInformationModule::Initialize (Handle<Object> target)
   {
      // set module metadata
      descriptor.SetName (module_name);
      descriptor.SetVersion (LokiModuleDescriptor::GetVersionStringFromArray (module_version));
      descriptor.SetDescription (module_description);
      // register this class's exported functions for the framework
      descriptor.AddFunction ("getMemoryInformation", GetMemoryInformation, "Queries Windows for memory information.", NO_PARAMETERS, RETURNS_AN OBJECT);
      descriptor.AddFunction ("getCpuInformation", GetCpuInformation, "Queries Windows for CPU information.", NO_PARAMETERS, RETURNS_AN OBJECT);
      // Register module with Node
      Register (target);
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
         // return memory information to caller
         args.GetReturnValue ().Set (process_list_module->getMemoryInformation (isolate));
      }
      else
      {
         // if parameter validation failed for whatever reason, report the error
         isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, error_string.c_str ())));
      }
   }

   // Gets CPU information. Exposed to JavaScript.
   void SystemInformationModule::GetCpuInformation (const FunctionCallbackInfo<Value>& args)
   {
      auto isolate = args.GetIsolate ();
      HandleScope scope (isolate);

      // validate parameters
      std::string error_string;
      if (descriptor.ValidateParameters (GetCpuInformation, args, error_string))
      {
         // unwrap object so we can call the correct function on the instance
         auto process_list_module = ObjectWrap::Unwrap<SystemInformationModule> (args.Holder ());
         // return CPU info to caller
         args.GetReturnValue ().Set (process_list_module->getCpuInformation (isolate));
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
      memoryInfo->Set (String::NewFromUtf8 (isolate, "totalPhysicalMemory"), Number::New (isolate, static_cast<double>(statex.ullTotalPhys / DIVISOR)));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "freePhysicalMemory"), Number::New (isolate, static_cast<double>(statex.ullAvailPhys / DIVISOR)));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "totalPageFileSize"), Number::New (isolate, static_cast<double>(statex.ullTotalPageFile / DIVISOR)));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "freePageFileMemory"), Number::New (isolate, static_cast<double>(statex.ullAvailPageFile / DIVISOR)));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "totalVirtualMemory"), Number::New (isolate, static_cast<double>(statex.ullTotalVirtual / DIVISOR)));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "freeVirtualMemory"), Number::New (isolate, static_cast<double>(statex.ullAvailVirtual / DIVISOR)));
      memoryInfo->Set (String::NewFromUtf8 (isolate, "availableExtendedMemory"), Number::New (isolate, static_cast<double>(statex.ullAvailExtendedVirtual / DIVISOR)));

      return memoryInfo;
   }

   // Gets CPU information.
   Local<Object> SystemInformationModule::getCpuInformation (Isolate* isolate)
   {
      HandleScope scope (isolate);

      SYSTEM_INFO info;

      GetSystemInfo (&info);

      // the array of devices to return to JavaScript
      auto cpuInfo = Object::New (isolate);

      cpuInfo->Set (String::NewFromUtf8 (isolate, "pageSize"), Integer::New (isolate, info.dwPageSize));
      cpuInfo->Set (String::NewFromUtf8 (isolate, "numberOfProcessors"), Integer::New (isolate, info.dwNumberOfProcessors));

      std::string cpuArchitecture;
      switch (info.wProcessorArchitecture)
      {
      case PROCESSOR_ARCHITECTURE_AMD64:
         cpuArchitecture = "x64";
         break;
      case PROCESSOR_ARCHITECTURE_ARM:
         cpuArchitecture = "ARM";
         break;
      case PROCESSOR_ARCHITECTURE_IA64:
         cpuArchitecture = "Itanium";
         break;
      case PROCESSOR_ARCHITECTURE_INTEL:
         cpuArchitecture = "x86";
         break;
      case PROCESSOR_ARCHITECTURE_UNKNOWN:
         cpuArchitecture = "Unknown";
         break;
      }

      cpuInfo->Set (String::NewFromUtf8 (isolate, "processorArchitecture"), String::NewFromUtf8 (isolate, cpuArchitecture.c_str ()));

      return cpuInfo;
   }
}
