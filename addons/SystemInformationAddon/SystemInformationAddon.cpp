/*
 * File: SystemInformationAddon.cpp
 * Created Date: Saturday, June 24th 2023
 * Author: Aaron Hall
 * -----
 * Last Modified: Saturday, 24th June 2023 3:55:50 pm
 * Modified By: Aaron Hall
 * -----
 * Copyright (c) 2020 - 2023
 */

#pragma comment(lib, "setupapi.lib")

#include <napi.h>

#include <Windows.h>

namespace SystemInformationAddon
{
  // For converting bytes into kilobytes.
  const int DIVISOR = 1024;
  const int MODULE_VERSION[3]{1 /*major*/, 0 /*minor*/, 0 /*patch*/};

  Napi::String GetAddonVersion(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    return Napi::String::New(env, std::to_string(MODULE_VERSION[0]) + '.' + std::to_string(MODULE_VERSION[1]) + '.' + std::to_string(MODULE_VERSION[2]));
  }

  Napi::Value GetMemoryInformation(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);

    GlobalMemoryStatusEx(&statex);

    // The array of devices to return to JavaScript.
    auto memoryInfo = Napi::Object::New(env);
    memoryInfo.Set(Napi::String::New(env, "memoryLoad"), Napi::Number::New(env, statex.dwMemoryLoad));
    memoryInfo.Set(Napi::String::New(env, "totalPhysicalMemory"), Napi::Number::New(env, static_cast<double>(statex.ullTotalPhys / DIVISOR)));
    memoryInfo.Set(Napi::String::New(env, "freePhysicalMemory"), Napi::Number::New(env, static_cast<double>(statex.ullAvailPhys / DIVISOR)));
    memoryInfo.Set(Napi::String::New(env, "totalPageFileSize"), Napi::Number::New(env, static_cast<double>(statex.ullTotalPageFile / DIVISOR)));
    memoryInfo.Set(Napi::String::New(env, "freePageFileMemory"), Napi::Number::New(env, static_cast<double>(statex.ullAvailPageFile / DIVISOR)));
    memoryInfo.Set(Napi::String::New(env, "totalVirtualMemory"), Napi::Number::New(env, static_cast<double>(statex.ullTotalVirtual / DIVISOR)));
    memoryInfo.Set(Napi::String::New(env, "freeVirtualMemory"), Napi::Number::New(env, static_cast<double>(statex.ullAvailVirtual / DIVISOR)));
    memoryInfo.Set(Napi::String::New(env, "availableExtendedMemory"), Napi::Number::New(env, static_cast<double>(statex.ullAvailExtendedVirtual / DIVISOR)));

    return memoryInfo;
  }

  Napi::Value GetCpuInformation(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    // tThe array of devices to return to JavaScript.
    auto cpuInfo = Napi::Object::New(env);

    cpuInfo.Set(Napi::String::New(env, "PageSize"), Napi::Number::New(env, systemInfo.dwPageSize));
    cpuInfo.Set(Napi::String::New(env, "NumberOfProcessors"), Napi::Number::New(env, systemInfo.dwNumberOfProcessors));

    std::string cpuArchitecture;
    switch (systemInfo.wProcessorArchitecture)
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

    cpuInfo.Set(Napi::String::New(env, "ProcessorArchitecture"), Napi::String::New(env, cpuArchitecture.c_str()));

    return cpuInfo;
  }

  Napi::Object Init(Napi::Env env, Napi::Object exports)
  {
    exports.Set(Napi::String::New(env, "GetMemoryInformation"), Napi::Function::New(env, GetMemoryInformation));
    exports.Set(Napi::String::New(env, "GetCpuInformation"), Napi::Function::New(env, GetCpuInformation));
    exports.Set(Napi::String::New(env, "GetAddonVersion"), Napi::Function::New(env, GetAddonVersion));
    return exports;
  }

  NODE_API_MODULE(SystemInformationAddon, Init)
}