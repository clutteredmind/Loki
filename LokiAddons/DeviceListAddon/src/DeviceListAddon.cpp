// 
// Purpose: Defines the node addon that deals with the Windows device list
//

#include "DeviceListAddon.hpp"

#define WINVER _WIN32_WINNT_WINBLUE

// windows API headers
#include <Windows.h>
#include <cfgmgr32.h>
#include <SetupAPI.h>

// standard includes
#include <vector>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Do all the magic to make this module accessible by node/javascript
// THE FIRST PARAMETER TO THE MACRO BELOW MUST MATCH THE MODULE FILENAME
NODE_MODULE(DeviceListAddon, DeviceListAddon::Initialize)

// constructor
DeviceListAddon::DeviceListAddon() {}

// destructor
DeviceListAddon::~DeviceListAddon() {}

// init function called by the main init routine.
void DeviceListAddon::Initialize(Handle<Object> target)
{
   auto isolate = Isolate::GetCurrent();

   // Prepare constructor template
   auto function_template = FunctionTemplate::New(isolate, Create);
   function_template->SetClassName(String::NewFromUtf8(isolate, "DeviceListAddon"));
   function_template->InstanceTemplate()->SetInternalFieldCount(1);

   // Export functions to JavaScript
   NODE_SET_PROTOTYPE_METHOD(function_template, "getDevices", GetDevices);

   constructor.Reset(isolate, function_template->GetFunction());
   target->Set(String::NewFromUtf8(isolate, "DeviceListAddon"), function_template->GetFunction());
}

// Creates a new instance of this class.
void DeviceListAddon::Create(const FunctionCallbackInfo<Value>& args)
{
   auto isolate = args.GetIsolate();

   if (args.IsConstructCall())
   {
      // Invoked as constructor: new MyObject(...)
      auto deviceListAddon = new DeviceListAddon();
      deviceListAddon->Wrap(args.This());
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
void DeviceListAddon::GetDevices(const FunctionCallbackInfo<Value>& args)
{
   // unwrap object so we can call the correct function on the instance
   auto deviceListAddon(ObjectWrap::Unwrap<DeviceListAddon>(args.Holder()));
   // return process list to caller
   args.GetReturnValue().Set(deviceListAddon->getDevices());
}

// gets process list from Windows
Local<Array> DeviceListAddon::getDevices()
{
   auto isolate = Isolate::GetCurrent();

   auto devices = Array::New(isolate);

   // Create a HDEVINFO with all device classes
   auto allDeviceClasses = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES);

   if (allDeviceClasses == INVALID_HANDLE_VALUE)
   {
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Unable to get all device classes.")));
   }
   else
   {
      SP_DEVINFO_DATA deviceInfoData;
      ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
      deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
      DWORD deviceIndex = 0;
      bool keepProcessing = true;

      // loop through and collect all class GUIDs
      std::vector<GUID> allClassGuids;
      while (keepProcessing)
      {
         // get device information
         SetupDiEnumDeviceInfo(allDeviceClasses, deviceIndex, &deviceInfoData);
         // save the GUID if it's not already in the list
         if (std::find(allClassGuids.begin(), allClassGuids.end(), deviceInfoData.ClassGuid) == allClassGuids.end())
         {
            allClassGuids.push_back(deviceInfoData.ClassGuid);
         }
         // increment device index
         deviceIndex++;
         // check to see if we should be done
         keepProcessing = !(GetLastError() == ERROR_NO_MORE_ITEMS);
      }

      // loop through GUIDs to get hardware info for each
      for (auto guid : allClassGuids)
      {
         auto deviceInfo = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT);
         if (deviceInfo == INVALID_HANDLE_VALUE)
         {
            OLECHAR* guidAsString;
            StringFromCLSID(guid, &guidAsString);
            std::wstring errorString = L"Unable to get device info for GUID: ";
            errorString += guidAsString;
            isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, std::string(errorString.begin(), errorString.end()).c_str())));
            CoTaskMemFree(guidAsString);
         }
         else
         {
            TCHAR deviceInstanceId [MAX_DEVICE_ID_LEN];
            // reset deviceInfoData
            ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
            deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
            // reset device index
            deviceIndex = 0;
            // a counter for indexing the v8 array
            int counter = 0;
            keepProcessing = true;
            // make an initial call to SetupDiEnumDeviceInfo to populate deviceInfoData
            SetupDiEnumDeviceInfo(deviceInfo, deviceIndex, &deviceInfoData);
            while (keepProcessing)
            {
               ZeroMemory(deviceInstanceId, MAX_DEVICE_ID_LEN);
               if (SetupDiGetDeviceInstanceId(deviceInfo, &deviceInfoData, deviceInstanceId, MAX_DEVICE_ID_LEN, NULL))
               {
                  // store device information
                  auto device = Object::New(isolate);

                  // save device instance ID
                  std::wstring deviceInstanceIdString(deviceInstanceId);
                  device->Set(String::NewFromUtf8(isolate, "device_instance_id"), String::NewFromUtf8(isolate, std::string(deviceInstanceIdString.begin(), deviceInstanceIdString.end()).c_str()));

                  BYTE* buffer;
                  DWORD propertySize = 0;

                  // get friendly name size
                  SetupDiGetDeviceRegistryProperty(deviceInfo, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, NULL, 0, &propertySize);
                  propertySize = propertySize > 1 ? propertySize : 1;
                  // set buffer size and zero it out
                  buffer = new BYTE [propertySize];
                  ZeroMemory(buffer, propertySize);
                  // get friendly name
                  SetupDiGetDeviceRegistryProperty(deviceInfo, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, buffer, propertySize, NULL);
                  // save friendly name
                  std::wstring friendlyName(reinterpret_cast<wchar_t*>(buffer));
                  device->Set(String::NewFromUtf8(isolate, "friendly_name"), String::NewFromUtf8(isolate, std::string(friendlyName.begin(), friendlyName.end()).c_str()));
                  // release buffer memory
                  delete [] buffer;

                  // get class size
                  SetupDiGetDeviceRegistryProperty(deviceInfo, &deviceInfoData, SPDRP_CLASS, NULL, NULL, 0, &propertySize);
                  propertySize = propertySize > 1 ? propertySize : 1;
                  // set buffer size and zero it out
                  buffer = new BYTE [propertySize];
                  ZeroMemory(buffer, propertySize);
                  // get class
                  SetupDiGetDeviceRegistryProperty(deviceInfo, &deviceInfoData, SPDRP_CLASS, NULL, buffer, propertySize, NULL);
                  // save class
                  std::wstring className(reinterpret_cast<wchar_t*>(buffer));
                  device->Set(String::NewFromUtf8(isolate, "class_name"), String::NewFromUtf8(isolate, std::string(className.begin(), className.end()).c_str()));
                  // release buffer memory
                  delete [] buffer;

                  // get hardware id size
                  SetupDiGetDeviceRegistryProperty(deviceInfo, &deviceInfoData, SPDRP_HARDWAREID, NULL, NULL, 0, &propertySize);
                  propertySize = propertySize > 1 ? propertySize : 1;
                  // set buffer size and zero it out
                  buffer = new BYTE [propertySize];
                  ZeroMemory(buffer, propertySize);
                  // get hardware id
                  SetupDiGetDeviceRegistryProperty(deviceInfo, &deviceInfoData, SPDRP_HARDWAREID, NULL, buffer, propertySize, NULL);
                  // save hardware id
                  std::wstring hardwareId(reinterpret_cast<wchar_t*>(buffer));
                  device->Set(String::NewFromUtf8(isolate, "hardware_id"), String::NewFromUtf8(isolate, std::string(hardwareId.begin(), hardwareId.end()).c_str()));
                  // release buffer memory
                  delete [] buffer;

                  // save device
                  devices->Set(Integer::New(isolate, counter), device);
               }

               // increment device index
               deviceIndex++;
               // attempt to get next device
               SetupDiEnumDeviceInfo(deviceInfo, deviceIndex, &deviceInfoData);
               // check to see if we should be done
               keepProcessing = !(GetLastError() == ERROR_NO_MORE_ITEMS);
               // increment counter
               counter++;
            }
         }
      }
   }

   // free deviceInfoSet memory
   if (allDeviceClasses)
   {
      SetupDiDestroyDeviceInfoList(allDeviceClasses);
   }

   // return devices array, which may be empty if there were failures above
   return devices;
}
