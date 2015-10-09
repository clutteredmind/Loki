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

// set the addon's name
const std::string ProcessListAddon::AddonName = "ProcessListAddon";

// constructor
ProcessListAddon::ProcessListAddon() {}

// destructor
ProcessListAddon::~ProcessListAddon() {}

// describes this object in JSON
Local<Object> ProcessListAddon::describe()
{
   Isolate* isolate = Isolate::GetCurrent();

   Local<Object> description = Object::New(isolate);
   
   description->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, "ProcessListAddon"));
   description->Set(String::NewFromUtf8(isolate, "version"), String::NewFromUtf8(isolate, "0.0.1"));

   return description;
}

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
         catch (...)
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
   FunctionList functionList {std::make_pair("getProcesses", GetProcesses), std::make_pair("describe", Describe)};
   // initialize the base class
   baseInit(target, AddonName, functionList);
}

// gets a list of all processes with their associated IDs
void ProcessListAddon::GetProcesses(const FunctionCallbackInfo<Value>& args)
{
   // unwrap object so we can call the correct function on the instance
   auto processListAddon(ObjectWrap::Unwrap<ProcessListAddon>(args.Holder()));
   // return process list to caller
   args.GetReturnValue().Set(processListAddon->getProcesses());
}

// describes this object in JSON
void ProcessListAddon::Describe(const FunctionCallbackInfo<Value>& args)
{
   // unwrap object so we can call the correct function on the instance
   auto processListAddon(ObjectWrap::Unwrap<ProcessListAddon>(args.Holder()));
   // return process list to caller
   args.GetReturnValue().Set(processListAddon->describe());
}

// Do all the magic to make this module accessible by node/javascript
// THE FIRST PARAMETER TO THE MACRO BELOW MUST MATCH THE MODULE FILENAME
NODE_MODULE(ProcessListAddon, ProcessListAddon::init)
