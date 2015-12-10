// 
// Purpose: Defines the node module that deals with the Windows process list
//

#include "ProcessListModule.hpp"

// Windows API headers
#include <Windows.h>
#include <tlhelp32.h> // for CreateToolhelp32Snapshot and some other things
#include <string>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Tell node which function to use to set up this module
NODE_MODULE(ProcessListModule, Loki::ProcessListModule::Initialize)

namespace Loki
{
   // The v8 constructor from the template class
   Persistent<Function> ProcessListModule::constructor;

   // The template class's descriptor object
   LokiModuleDescriptor ProcessListModule::descriptor;

   // module metadata
   const std::string module_name = "ProcessListModule";
   const int module_version [3] {1 /*major*/, 0 /*minor*/, 0 /*patch*/};
   const std::string module_description = "Retrieves a list of running processes via the Windows API";

   // Pre-initialization.
   void ProcessListModule::Initialize(Handle<Object> target)
   {
      // set module metadata
      descriptor.SetName(module_name);
      descriptor.SetVersion(LokiModuleDescriptor::GetVersionStringFromArray(module_version));
      descriptor.SetDescription(module_description);
      // register this class's exported functions for the framework
      descriptor.AddFunction("getProcesses", GetProcesses, "Gets a list of all running processes.", NO_PARAMETERS, RETURNS_AN OBJECT);
      // Register module with Node
      Register(target);
   }

   // Gets a list of all running processes with their associated PIDs. Exposed to JavaScript.
   void ProcessListModule::GetProcesses(const FunctionCallbackInfo<Value>& args)
   {
      auto isolate = args.GetIsolate();
      HandleScope scope(isolate);

      // validate parameters
      std::string error_string;
      if (descriptor.ValidateParameters(GetProcesses, args, error_string))
      {
         // unwrap object so we can call the correct function on the instance
         auto process_list_module = ObjectWrap::Unwrap<ProcessListModule>(args.Holder());
         // return process list to caller
         args.GetReturnValue().Set(process_list_module->getProcesses(isolate));
      }
      else
      {
         // if parameter validation failed for whatever reason, report the error
         isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, error_string.c_str())));
      }
   }

   // Gets a list of all running processes with their associated PIDs.
   Local<Array> ProcessListModule::getProcesses(Isolate* isolate)
   {
      HandleScope scope(isolate);

      // the array of processes to return to JavaScript
      auto processes = Array::New(isolate);

      // get process snapshot
      HANDLE process_snapshot_handle = INVALID_HANDLE_VALUE;

      try
      {
         process_snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

         if (process_snapshot_handle == INVALID_HANDLE_VALUE)
         {
            throw std::exception("Unable to create process snapshot.");
         }
         else
         {
            PROCESSENTRY32 process_entry;

            process_entry.dwSize = sizeof(PROCESSENTRY32);

            if (!Process32First(process_snapshot_handle, &process_entry))
            {
               throw std::exception("Could not retrieve information about first process.");
            }
            else
            {
               int counter = 0;
               // loop while there are processes to enumerate
               do
               {
                  // save process information
                  auto process = Object::New(isolate);

                  // save process name
                  std::wstring process_name(process_entry.szExeFile);
                  process->Set(String::NewFromUtf8(isolate, "process_name"), String::NewFromUtf8(isolate, std::string(process_name.begin(), process_name.end()).c_str()));

                  // save process ID
                  process->Set(String::NewFromUtf8(isolate, "process_process_id"), Integer::New(isolate, process_entry.th32ProcessID));
                  // save thread count
                  process->Set(String::NewFromUtf8(isolate, "process_thread_count"), Integer::New(isolate, process_entry.cntThreads));
                  // save parent process ID
                  process->Set(String::NewFromUtf8(isolate, "process_parent_process_id"), Integer::New(isolate, process_entry.th32ParentProcessID));
                  // save priority base
                  process->Set(String::NewFromUtf8(isolate, "process_priority_base"), Integer::New(isolate, process_entry.pcPriClassBase));

                  // add process to list
                  processes->Set(Integer::New(isolate, counter), process);

                  // increment counter
                  counter++;
               } while (Process32Next(process_snapshot_handle, &process_entry));
            }
         }
      }
      catch (std::exception& exception)
      {
         isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, exception.what())));
      }

      // clean up process snapshot handle
      CloseHandle(process_snapshot_handle);

      // return processes array, which may be empty if there were failures above
      return processes;
   }
}
