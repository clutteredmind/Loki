// 
// Purpose: Defines the node module that deals with the Windows process list
//

#include "ProcessListModule.hpp"

// Windows API headers
#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h> // for CreateToolhelp32Snapshot and some other things

#include <string>
#include <sstream>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Tell node which function to use to set up this module
NODE_MODULE (ProcessListModule, Loki::ProcessListModule::Initialize)

namespace Loki
{
   // The v8 constructor from the template class
   Persistent<Function> ProcessListModule::constructor;

   // The template class's descriptor object
   LokiModuleDescriptor ProcessListModule::descriptor;

   // module metadata
   const std::string MODULE_NAME = "ProcessListModule";
   const std::string MODULE_DISPLAY_NAME = "Process List";
   const int MODULE_VERSION[3]{ 1 /*major*/, 0 /*minor*/, 0 /*patch*/ };
   const std::string MODULE_DESCRIPTION = "Retrieves a list of running processes via the Windows API";

   // Pre-initialization.
   void ProcessListModule::Initialize (Handle<Object> target)
   {
      // set module metadata
      descriptor.SetMetadata (MODULE_NAME, MODULE_DISPLAY_NAME, LokiModuleDescriptor::GetVersionStringFromArray (MODULE_VERSION), MODULE_DESCRIPTION);
      // register this class's exported functions for the framework
      descriptor.AddFunction ("getProcesses", GetProcesses, "Gets a list of all running processes.", NO_PARAMETERS, RETURNS_AN OBJECT);
      descriptor.AddFunction ("getProcessModules", GetProcessModules, "Gets a list of modules loaded by a particular process.", { LOKI_PARAMETER (ParameterType::NUMBER, "processId") }, RETURNS_AN OBJECT);
      // Register module with Node
      Register (target);
   }

   // Gets a list of all running processes with their associated PIDs. Exposed to JavaScript.
   void ProcessListModule::GetProcesses (const FunctionCallbackInfo<Value>& args)
   {
      auto isolate = args.GetIsolate ();
      HandleScope scope (isolate);

      // validate parameters
      std::string error_string;
      if (descriptor.ValidateParameters (GetProcesses, args, error_string))
      {
         // unwrap object so we can call the correct function on the instance
         auto process_list_module = ObjectWrap::Unwrap<ProcessListModule> (args.Holder ());
         // return process list to caller
         args.GetReturnValue ().Set (process_list_module->getProcesses (isolate));
      }
      else
      {
         // if parameter validation failed for whatever reason, report the error
         isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, error_string.c_str ())));
      }
   }

   // Gets a list of all modules for a particular process. Exposed to JavaScript.
   void ProcessListModule::GetProcessModules (const FunctionCallbackInfo<Value>& args)
   {
      auto isolate = args.GetIsolate ();
      HandleScope scope (isolate);

      // validate parameters
      std::string error_string;
      if (descriptor.ValidateParameters (GetProcessModules, args, error_string))
      {
         // unwrap object so we can call the correct function on the instance
         auto process_list_module = ObjectWrap::Unwrap<ProcessListModule> (args.Holder ());
         // return module list to caller
         args.GetReturnValue ().Set (process_list_module->getProcessModules (isolate, args[0]->Uint32Value ()));
      }
      else
      {
         // if parameter validation failed for whatever reason, report the error
         isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, error_string.c_str ())));
      }
   }

   // Gets a list of all running processes with their associated PIDs.
   Local<Array> ProcessListModule::getProcesses (Isolate* isolate)
   {
      HandleScope scope (isolate);

      // the array of processes to return to JavaScript
      auto processes = Array::New (isolate);

      // get process snapshot
      HANDLE process_snapshot_handle = INVALID_HANDLE_VALUE;

      try
      {
         process_snapshot_handle = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);

         if (process_snapshot_handle == INVALID_HANDLE_VALUE)
         {
            throw std::exception ("Unable to create process snapshot.");
         }
         else
         {
            PROCESSENTRY32 process_entry;

            process_entry.dwSize = sizeof (PROCESSENTRY32);

            if (!Process32First (process_snapshot_handle, &process_entry))
            {
               throw std::exception ("Could not retrieve information about first process.");
            }
            else
            {
               int counter = 0;
               // loop while there are processes to enumerate
               do
               {
                  // save process information
                  auto process = Object::New (isolate);

                  // save process name
                  std::wstring process_name (process_entry.szExeFile);
                  process->Set (String::NewFromUtf8 (isolate, "processName"), String::NewFromUtf8 (isolate, std::string (process_name.begin (), process_name.end ()).c_str ()));

                  // save process ID
                  process->Set (String::NewFromUtf8 (isolate, "processProcessId"), Integer::New (isolate, process_entry.th32ProcessID));
                  // save thread count
                  process->Set (String::NewFromUtf8 (isolate, "processThreadCount"), Integer::New (isolate, process_entry.cntThreads));
                  // save parent process ID
                  process->Set (String::NewFromUtf8 (isolate, "processParentProcessId"), Integer::New (isolate, process_entry.th32ParentProcessID));
                  // save priority base
                  process->Set (String::NewFromUtf8 (isolate, "processPriorityBase"), Integer::New (isolate, process_entry.pcPriClassBase));

                  // add process to list
                  processes->Set (Integer::New (isolate, counter), process);

                  // increment counter
                  counter++;
               } while (Process32Next (process_snapshot_handle, &process_entry));
            }
         }
      }
      catch (std::exception& exception)
      {
         isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, exception.what ())));
      }

      // clean up process snapshot handle
      CloseHandle (process_snapshot_handle);

      // return processes array, which may be empty if there were failures above
      return processes;
   }

   // Gets a list of all modules for a particular process.
   Local<Array> ProcessListModule::getProcessModules (Isolate* isolate, int process_id)
   {
      HandleScope scope (isolate);

      // the array of processes to return to JavaScript
      auto modules = Array::New (isolate);

      try
      {
         HANDLE process_handle = OpenProcess (PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);
         if (NULL == process_handle)
         {
            isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, "Could not retrieve process handle.")));
         }

         HMODULE module_handles[1024];
         DWORD bytes_needed;

         if (EnumProcessModulesEx (process_handle, module_handles, sizeof (module_handles), &bytes_needed, LIST_MODULES_ALL))
         {
            int counter = 0;
            for (unsigned int index = 0; index < (bytes_needed / sizeof (HMODULE)); index++)
            {
               auto module = Object::New (isolate);

               char module_name[MAX_PATH];
               // Get the full path to the module's file.
               if (GetModuleFileNameExA (process_handle, module_handles[index], module_name, sizeof (module_name)))
               {
                  module->Set (String::NewFromUtf8 (isolate, "moduleName"), String::NewFromUtf8 (isolate, module_name));
                  std::ostringstream handle_value;
                  handle_value << "0x" << std::hex << module_handles[index];
                  module->Set (String::NewFromUtf8 (isolate, "moduleHandle"), String::NewFromUtf8 (isolate, handle_value.str ().c_str ()));

                  // save module
                  modules->Set (Integer::New (isolate, counter), module);
                  counter++;
               }
            }
         }
         else
         {
            std::string error_message = "EnumProcessModules failed with error code: " + std::to_string (GetLastError ());
            isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, error_message.c_str ())));
         }

         CloseHandle (process_handle);
      }
      catch (std::exception& exception)
      {
         std::string error_message = "Error while enumerating modules: ";
         error_message += exception.what ();
         isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, error_message.c_str ())));
      }

      // return list of modules
      return modules;
   }
}
