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
NODE_MODULE(ProcessListAddon, ProcessListAddon::Initialize)

// v8 constructor
Persistent<Function> ProcessListAddon::constructor;

// constructor
ProcessListAddon::ProcessListAddon() {}

// destructor
ProcessListAddon::~ProcessListAddon() {}

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
      auto processListAddon = new ProcessListAddon();
      processListAddon->Wrap(args.This());
      args.GetReturnValue().Set(args.This());
   }
   else
   {
      // Invoked as plain function `MyObject(...)`, turn into construct call.
      auto ctor = Local<Function>::New(isolate, constructor);
      args.GetReturnValue().Set(ctor->NewInstance());
   }
}

// gets a list of all processes with their associated IDs
void ProcessListAddon::GetProcesses(const FunctionCallbackInfo<Value>& args)
{
   // unwrap object so we can call the correct function on the instance
   auto processListAddon(ObjectWrap::Unwrap<ProcessListAddon>(args.Holder()));
   // return process list to caller
   args.GetReturnValue().Set(processListAddon->getProcesses());
}

// gets process list from Windows
Local<Array> ProcessListAddon::getProcesses()
{
   auto isolate = Isolate::GetCurrent();

   auto processes = Array::New(isolate);

   PROCESSENTRY32 processEntry;

   // get process snapshot
   auto processSnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if (processSnapshotHandle == INVALID_HANDLE_VALUE)
   {
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Unable to create process snapshot.")));
   }
   else
   {
      processEntry.dwSize = sizeof(PROCESSENTRY32);

      if (!Process32First(processSnapshotHandle, &processEntry))
      {
         isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Could not retrieve information about first process.")));
         CloseHandle(processSnapshotHandle);
      }
      else
      {
         try
         {
            int counter = 0;
            // loop while there are processes to enumerate
            do
            {
               // save process information
               auto process = Object::New(isolate);

               // save process name
               std::wstring processName(processEntry.szExeFile);
               process->Set(String::NewFromUtf8(isolate, "process_name"), String::NewFromUtf8(isolate, std::string(processName.begin(), processName.end()).c_str()));

               // save process ID
               process->Set(String::NewFromUtf8(isolate, "process_process_id"), Integer::New(isolate, processEntry.th32ProcessID));
               // save thread count
               process->Set(String::NewFromUtf8(isolate, "process_thread_count"), Integer::New(isolate, processEntry.cntThreads));
               // save parent process ID
               process->Set(String::NewFromUtf8(isolate, "process_parent_process_id"), Integer::New(isolate, processEntry.th32ParentProcessID));
               // save priority base
               process->Set(String::NewFromUtf8(isolate, "process_priority_base"), Integer::New(isolate, processEntry.pcPriClassBase));

               // add process to list
               processes->Set(Integer::New(isolate, counter), process);

               // increment counter
               counter++;
            } while (Process32Next(processSnapshotHandle, &processEntry));

            // clean up process snapshot handle
            CloseHandle(processSnapshotHandle);
         }
         catch (...)
         {
            // clean up process snapshot handle on any exception
            CloseHandle(processSnapshotHandle);
         }
      }
   }

   // return processes array, which may be empty if there were failures above
   return processes;
}
