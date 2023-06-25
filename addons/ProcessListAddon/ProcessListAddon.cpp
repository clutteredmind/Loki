/*
 * File: ProcessListAddon.cpp
 * Created Date: Saturday, June 24th 2023
 * Author: Aaron Hall
 * -----
 * Last Modified: Sunday, 25th June 2023 6:53:07 am
 * Modified By: Aaron Hall
 * -----
 * Copyright (c) 2020 - 2023
 */

#pragma comment(lib, "setupapi.lib")

#include <napi.h>

// Windows API headers
#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h> // For CreateToolhelp32Snapshot and some other Win32 API things.

#include <string>
#include <sstream>

namespace ProcessListAddon
{
  const int MODULE_VERSION[3]{1 /*major*/, 0 /*minor*/, 0 /*patch*/};

  Napi::String GetAddonVersion(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    return Napi::String::New(env, std::to_string(MODULE_VERSION[0]) + '.' + std::to_string(MODULE_VERSION[1]) + '.' + std::to_string(MODULE_VERSION[2]));
  }

  Napi::Value GetProcesses(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    Napi::Value result;

    // Array of processes to return to JavaScript.
    auto processes = Napi::Array::New(env);

    HANDLE processSnapshotHandle = INVALID_HANDLE_VALUE;

    try
    {
      processSnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

      if (processSnapshotHandle == INVALID_HANDLE_VALUE)
      {
        throw std::exception("Unable to create process snapshot.");
      }
      else
      {
        PROCESSENTRY32 processEntry;

        processEntry.dwSize = sizeof(PROCESSENTRY32);

        if (!Process32First(processSnapshotHandle, &processEntry))
        {
          throw std::exception("Could not retrieve information about first process.");
        }
        else
        {
          int counter = 0;
          // Loop while there are processes to enumerate.
          do
          {
            // Save process information.
            auto process = Napi::Object::New(env);

            // Save process name.
            std::wstring processName(processEntry.szExeFile);
            process.Set(Napi::String::New(env, "processName"), Napi::String::New(env, std::string(processName.begin(), processName.end()).c_str()));

            // Save process ID.
            process.Set(Napi::String::New(env, "processProcessId"), Napi::Number::New(env, processEntry.th32ProcessID));
            // Save thread count.
            process.Set(Napi::String::New(env, "processThreadCount"), Napi::Number::New(env, processEntry.cntThreads));
            // Save parent process ID.
            process.Set(Napi::String::New(env, "processParentProcessId"), Napi::Number::New(env, processEntry.th32ParentProcessID));
            // Save priority base.
            process.Set(Napi::String::New(env, "processPriorityBase"), Napi::Number::New(env, processEntry.pcPriClassBase));

            // Add process to list.
            processes.Set(Napi::Number::New(env, counter), process);

            counter++;
          } while (Process32Next(processSnapshotHandle, &processEntry));
        }
        result = processes;
      }
    }
    catch (std::exception &exception)
    {
      Napi::Error::New(env, Napi::String::New(env, exception.what())).ThrowAsJavaScriptException();
      result = env.Null();
    }

    // Clean up process snapshot handle.
    CloseHandle(processSnapshotHandle);

    // Return processes array, which may be empty if there were failures above.
    return result;
  }

  Napi::Value GetProcessModules(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    Napi::Value result;

    if (!info[0].IsNumber())
    {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
      result = env.Null();
    }
    else
    {
      int pid = info[0].As<Napi::Number>().Int32Value();

      // Array of processes to return to JavaScript.
      auto modules = Napi::Array::New(env);

      try
      {
        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (NULL == processHandle)
        {
          throw std::exception("Could not retrieve process handle.");
        }

        HMODULE moduleHandles[1024];
        DWORD bytes_needed;

        if (EnumProcessModulesEx(processHandle, moduleHandles, sizeof(moduleHandles), &bytes_needed, LIST_MODULES_ALL))
        {
          int counter = 0;
          for (unsigned int index = 0; index < (bytes_needed / sizeof(HMODULE)); index++)
          {
            auto module = Napi::Object::New(env);

            char module_name[MAX_PATH];
            // Get the full path to the module's file.
            if (GetModuleFileNameExA(processHandle, moduleHandles[index], module_name, sizeof(module_name)))
            {
              module.Set(Napi::String::New(env, "moduleName"), Napi::String::New(env, module_name));
              std::ostringstream handle_value;
              handle_value << "0x" << std::hex << moduleHandles[index];
              module.Set(Napi::String::New(env, "moduleHandle"), Napi::String::New(env, handle_value.str().c_str()));

              // Save module.
              modules.Set(Napi::Number::New(env, counter), module);
              counter++;
            }
          }
        }
        else
        {
          std::string errorMessage = "EnumProcessModules failed with error code: " + std::to_string(GetLastError());
          throw std::exception(errorMessage.c_str());
        }

        CloseHandle(processHandle);

        result = modules;
      }
      catch (std::exception &exception)
      {
        std::string errorMessage = "Error while enumerating modules: ";
        errorMessage += exception.what();
        Napi::Error::New(env, Napi::String::New(env, errorMessage)).ThrowAsJavaScriptException();
        result = env.Null();
      }

      // return list of modules
      result = modules;
    }

    return result;
  }

  Napi::Object Init(Napi::Env env, Napi::Object exports)
  {
    exports.Set(Napi::String::New(env, "GetProcesses"), Napi::Function::New(env, GetProcesses));
    exports.Set(Napi::String::New(env, "GetProcessModules"), Napi::Function::New(env, GetProcessModules));
    exports.Set(Napi::String::New(env, "GetAddonVersion"), Napi::Function::New(env, GetAddonVersion));
    return exports;
  }

  NODE_API_MODULE(ProcessListAddon, Init)
}