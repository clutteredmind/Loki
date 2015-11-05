// 
// Purpose: Defines the node addon that deals with the Windows process list
//

#include "ProcessListAddon.hpp"

// Windows API headers
#include <Windows.h>
#include <tlhelp32.h>
#include <string>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Do all the magic to make this module accessible by node/javascript
// THE FIRST PARAMETER TO THE MACRO BELOW MUST MATCH THE MODULE FILENAME
NODE_MODULE(ProcessListAddon, Loki::ProcessListAddon::Initialize)

namespace Loki
{
   // The v8 constructor
   v8::Persistent<v8::Function> ProcessListAddon::constructor;

   // Initialization. This function is required by node.
   void ProcessListAddon::Initialize(Handle<Object> target)
   {
      auto isolate = Isolate::GetCurrent();

      // Prepare constructor template
      auto function_template = FunctionTemplate::New(isolate, Create);
      function_template->SetClassName(String::NewFromUtf8(isolate, "ProcessListAddon"));
      function_template->InstanceTemplate()->SetInternalFieldCount(1);

      // Export functions to JavaScript
      NODE_SET_PROTOTYPE_METHOD(function_template, "getProcesses", GetProcesses);

      constructor.Reset(isolate, function_template->GetFunction());
      target->Set(String::NewFromUtf8(isolate, "ProcessListAddon"), function_template->GetFunction());
   }

   // Creates a new instance of this class.
   void ProcessListAddon::Create(const FunctionCallbackInfo<Value>& args)
   {
      auto isolate = args.GetIsolate();

      if (args.IsConstructCall())
      {
         // Invoked as constructor: new MyObject(...)
         auto process_list_addon = new ProcessListAddon();
         process_list_addon->Wrap(args.This());
         args.GetReturnValue().Set(args.This());
      }
      else
      {
         // Invoked as plain function `MyObject(...)`, turn into construct call.
         auto ctor = Local<Function>::New(isolate, constructor);
         args.GetReturnValue().Set(ctor->NewInstance());
      }
   }

   // Gets a list of all running processes with their associated PIDs. Exposed to JavaScript.
   void ProcessListAddon::GetProcesses(const FunctionCallbackInfo<Value>& args)
   {
      // unwrap object so we can call the correct function on the instance
      auto process_list_addon = ObjectWrap::Unwrap<ProcessListAddon>(args.Holder());
      // return process list to caller
      args.GetReturnValue().Set(process_list_addon->getProcesses());
   }

   // Gets a list of all running processes with their associated PIDs.
   Local<Array> ProcessListAddon::getProcesses()
   {
      auto isolate = Isolate::GetCurrent();
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
