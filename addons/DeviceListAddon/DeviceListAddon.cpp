/*
 * File: DeviceListAddon.cpp
 * Created Date: Saturday, June 24th 2023
 * Author: Aaron Hall
 * -----
 * Last Modified: Saturday, 24th June 2023 4:21:45 pm
 * Modified By: Aaron Hall
 * -----
 * Copyright (c) 2020 - 2023
 */

#pragma comment(lib, "setupapi.lib")

#include <napi.h>

#define WINVER _WIN32_WINNT_WINBLUE

// Windows API headers.
#include <Windows.h>
#include <cfgmgr32.h> // for MAX_DEVICE_ID_LEN
#include <SetupAPI.h> // for SetupDiGetClassDevs, etc.

namespace DeviceListAddon
{
  const int MODULE_VERSION[3]{1 /*major*/, 0 /*minor*/, 0 /*patch*/};

  Napi::String GetAddonVersion(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    return Napi::String::New(env, std::to_string(MODULE_VERSION[0]) +
                                      '.' +
                                      std::to_string(MODULE_VERSION[1]) +
                                      '.' +
                                      std::to_string(MODULE_VERSION[2]));
  }

  Napi::Value GetDevices(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    Napi::Value result;

    // The array of devices to return to JavaScript.
    auto devices = Napi::Array::New(env);

    HDEVINFO allDeviceClasses;
    try
    {
      allDeviceClasses = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES);

      if (allDeviceClasses == INVALID_HANDLE_VALUE)
      {
        throw std::exception("Unable to get all device classes.");
      }
      else
      {
        SP_DEVINFO_DATA deviceInfoData;
        ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
        deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        DWORD deviceIndex = 0;
        bool keepProcessing = true;

        // Loop through and collect all class GUIDs.
        std::vector<GUID> allClassGuids;
        while (keepProcessing)
        {
          // Get device information.
          SetupDiEnumDeviceInfo(allDeviceClasses, deviceIndex, &deviceInfoData);
          // Save the GUID if it's not already in the list.
          if (std::find(allClassGuids.begin(), allClassGuids.end(), deviceInfoData.ClassGuid) == allClassGuids.end())
          {
            allClassGuids.push_back(deviceInfoData.ClassGuid);
          }
          // Increment device index.
          deviceIndex++;
          // Check to see if we should be done.
          keepProcessing = !(GetLastError() == ERROR_NO_MORE_ITEMS);
        }

        // Loop through GUIDs to get hardware info for each.
        for (auto guid : allClassGuids)
        {
          auto deviceInformation = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT);
          if (deviceInformation == INVALID_HANDLE_VALUE)
          {
            OLECHAR *guidAsString;
            StringFromCLSID(guid, &guidAsString);
            std::wstring errorMessage = L"Unable to get device info for GUID: ";
            errorMessage += guidAsString;
            CoTaskMemFree(guidAsString);
            throw std::exception(std::string(errorMessage.begin(), errorMessage.end()).c_str());
          }
          else
          {
            TCHAR deviceInstanceId[MAX_DEVICE_ID_LEN];
            // Reset deviceInfoData.
            ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
            deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
            // Reset device index.
            deviceIndex = 0;
            // A counter for indexing the v8 array.
            int counter = 0;
            keepProcessing = true;
            // Make an initial call to SetupDiEnumDeviceInfo to populate deviceInfoData.
            SetupDiEnumDeviceInfo(deviceInformation, deviceIndex, &deviceInfoData);
            while (keepProcessing)
            {
              ZeroMemory(deviceInstanceId, MAX_DEVICE_ID_LEN);
              if (SetupDiGetDeviceInstanceId(deviceInformation, &deviceInfoData, deviceInstanceId, MAX_DEVICE_ID_LEN, NULL))
              {
                // Store device information.
                auto device = Napi::Object::New(env);

                // Save device instance ID.
                std::basic_string<TCHAR> deviceInstanceIdString(deviceInstanceId);
                device.Set(Napi::String::New(env, "DeviceInstanceId"), Napi::String::New(env, std::string(deviceInstanceIdString.begin(), deviceInstanceIdString.end()).c_str()));

                BYTE *pBuffer;
                DWORD propertySize = 0;

                // Get friendly name size.
                SetupDiGetDeviceRegistryProperty(deviceInformation, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, NULL, 0, &propertySize);
                propertySize = propertySize > 1 ? propertySize : 1;
                // Set buffer size and zero it out.
                pBuffer = new BYTE[propertySize];
                ZeroMemory(pBuffer, propertySize);
                // Get friendly name.
                SetupDiGetDeviceRegistryProperty(deviceInformation, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, pBuffer, propertySize, NULL);
                // Save friendly name.
                std::basic_string<TCHAR> friendlyName(reinterpret_cast<TCHAR *>(pBuffer));
                device.Set(Napi::String::New(env, "FriendlyName"), Napi::String::New(env, std::string(friendlyName.begin(), friendlyName.end()).c_str()));
                // release buffer memory.
                delete[] pBuffer;

                // Get class size.
                SetupDiGetDeviceRegistryProperty(deviceInformation, &deviceInfoData, SPDRP_CLASS, NULL, NULL, 0, &propertySize);
                propertySize = propertySize > 1 ? propertySize : 1;
                // Set buffer size and zero it out.
                pBuffer = new BYTE[propertySize];
                ZeroMemory(pBuffer, propertySize);
                // Get class.
                SetupDiGetDeviceRegistryProperty(deviceInformation, &deviceInfoData, SPDRP_CLASS, NULL, pBuffer, propertySize, NULL);
                // Save class.
                std::basic_string<TCHAR> class_name(reinterpret_cast<TCHAR *>(pBuffer));
                device.Set(Napi::String::New(env, "ClassName"), Napi::String::New(env, std::string(class_name.begin(), class_name.end()).c_str()));
                // Release buffer memory.
                delete[] pBuffer;

                // Get hardware id size.
                SetupDiGetDeviceRegistryProperty(deviceInformation, &deviceInfoData, SPDRP_HARDWAREID, NULL, NULL, 0, &propertySize);
                propertySize = propertySize > 1 ? propertySize : 1;
                // Set buffer size and zero it out.
                pBuffer = new BYTE[propertySize];
                ZeroMemory(pBuffer, propertySize);
                // Get hardware id.
                SetupDiGetDeviceRegistryProperty(deviceInformation, &deviceInfoData, SPDRP_HARDWAREID, NULL, pBuffer, propertySize, NULL);
                // Save hardware id.
                std::basic_string<TCHAR> hardwareId(reinterpret_cast<TCHAR *>(pBuffer));
                device.Set(Napi::String::New(env, "HardwareId"), Napi::String::New(env, std::string(hardwareId.begin(), hardwareId.end()).c_str()));
                // Release buffer memory.
                delete[] pBuffer;

                // Save device.
                devices.Set(Napi::Number::New(env, counter), device);
                // Increment counter.
                counter++;
              }

              // Increment device index.
              deviceIndex++;
              // Attempt to get next device.
              SetupDiEnumDeviceInfo(deviceInformation, deviceIndex, &deviceInfoData);
              // Check to see if we should be done.
              keepProcessing = !(GetLastError() == ERROR_NO_MORE_ITEMS);
            }
          }
        }

        result = devices;
      }
    }
    catch (std::exception &exception)
    {
      Napi::Error::New(env, Napi::String::New(env, exception.what())).ThrowAsJavaScriptException();
      result = env.Null();
    }

    // Free dynamic memory.
    if (allDeviceClasses)
    {
      SetupDiDestroyDeviceInfoList(allDeviceClasses);
    }

    return devices;
  }

  Napi::Object Init(Napi::Env env, Napi::Object exports)
  {
    exports.Set(Napi::String::New(env, "GetDevices"), Napi::Function::New(env, GetDevices));
    exports.Set(Napi::String::New(env, "GetAddonVersion"), Napi::Function::New(env, GetAddonVersion));
    return exports;
  }

  NODE_API_MODULE(DeviceListAddon, Init)
}