// 
// Purpose: Defines the node addon that deals with the Windows process list
//

#include "ProcessListAddon.hpp"

// the node libraries
#include <node.h>

// Windows API headers
#include <Windows.h>
#include <tlhelp32.h>
#include <string>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

Persistent<Function> ProcessListAddon::constructor;

// constructor
ProcessListAddon::ProcessListAddon() {}

// destructor
ProcessListAddon::~ProcessListAddon() {}

// gets process list from Windows
Local<Array> ProcessListAddon::getProcesses()
{
   Isolate* isolate = Isolate::GetCurrent();

   Local<Array> processes = Array::New(isolate);

   HANDLE processSnapshotHandle;
   PROCESSENTRY32 processEntry;

   // get process snapshot
   processSnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
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
               Local<Object> process = Object::New(isolate);

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
         catch(...)
         {
            // clean up process snapshot handle on any exception
            CloseHandle(processSnapshotHandle);
         }
      }
   }

   // return processes array, which may be empty, if there were failures above
   return processes;
}

// init function called by the main init routine.
void ProcessListAddon::init(Handle<Object> target)
{
   Isolate* isolate = Isolate::GetCurrent();

   // Prepare constructor template
   Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, create);
   tpl->SetClassName(String::NewFromUtf8(isolate, "ProcessListAddon"));
   tpl->InstanceTemplate()->SetInternalFieldCount(1);

   // Set up function prototypes
   NODE_SET_PROTOTYPE_METHOD(tpl, "getProcesses", getProcesses);
   NODE_SET_PROTOTYPE_METHOD(tpl, "describe", describe);

   constructor.Reset(isolate, tpl->GetFunction());
   target->Set(String::NewFromUtf8(isolate, "ProcessListAddon"), tpl->GetFunction());
}

// create a new instance and wrap it up to be passed back to node
void ProcessListAddon::create(const FunctionCallbackInfo<Value>& args)
{
   Isolate* isolate = args.GetIsolate();

   if (args.IsConstructCall())
   {
      // invoked as constructor: 'new Object(...)'
      ProcessListAddon* processListAddon = new ProcessListAddon();
      processListAddon->Wrap(args.This());

      args.GetReturnValue().Set(args.This());
   }
   else
   {
      // invoked as a plain function 'Object(...)', turn into function call
      const int argc = 1;
      Local<Value> argv [argc] = { args[0] };
      Local<Function> ctor = Local<Function>::New(isolate, constructor);
      args.GetReturnValue().Set(ctor->NewInstance(argc, argv));
   }
}

// gets a list of all processes with their associated IDs
void ProcessListAddon::getProcesses(const FunctionCallbackInfo<Value>& args)
{
   // unwrap object so we can call the correct function on the instance
   ProcessListAddon* processListAddon (ObjectWrap::Unwrap<ProcessListAddon>(args.Holder()));
   // return process list to caller
   args.GetReturnValue().Set(processListAddon->getProcesses());
}
