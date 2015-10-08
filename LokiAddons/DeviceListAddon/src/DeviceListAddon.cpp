// 
// Purpose: Defines the node addon that deals with the Windows device list
//

#include "DeviceListAddon.hpp"

// the node libraries
#include <node.h>

#define WINVER _WIN32_WINNT_WINBLUE

// windows API headers
#include <Windows.h>
#include <cfgmgr32.h>
#include <SetupAPI.h>

// standard includes
#include <vector>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// set the addon's name
const std::string DeviceListAddon::AddonName = "DeviceListAddon";

// constructor
DeviceListAddon::DeviceListAddon() {}

// destructor
DeviceListAddon::~DeviceListAddon() {}

// describes this object in JSON
Local<Object> DeviceListAddon::describe()
{
   Isolate* isolate = Isolate::GetCurrent();

   Local<Object> description = Object::New(isolate);
   
   description->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, "DeviceListAddon"));
   description->Set(String::NewFromUtf8(isolate, "version"), String::NewFromUtf8(isolate, "0.0.1"));

   return description;
}

// gets process list from Windows
Local<Array> DeviceListAddon::getDeviceList()
{
   Isolate* isolate = Isolate::GetCurrent();

   Local<Array> devices = Array::New(isolate);

   HDEVINFO allDeviceClasses;

   // Create a HDEVINFO with all device classes
   allDeviceClasses = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES);

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
      for (unsigned int counter = 0; counter < allClassGuids.size(); counter++)
      {
         HDEVINFO deviceInfo = SetupDiGetClassDevs(&allClassGuids[counter], NULL, NULL, DIGCF_PRESENT);
         if (deviceInfo == INVALID_HANDLE_VALUE)
         {
            OLECHAR* guidAsString;
            StringFromCLSID(allClassGuids [counter], &guidAsString);
            std::wstring errorString = L"Unable to get device info for GUID:";
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
                  Local<Object> device = Object::New(isolate);

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
                  // increment counter
                  counter++;
               }
               
               // increment device index
               deviceIndex++;
               // attempt to get next device
               SetupDiEnumDeviceInfo(deviceInfo, deviceIndex, &deviceInfoData);
               // check to see if we should be done
               keepProcessing = !(GetLastError() == ERROR_NO_MORE_ITEMS);
            }
         }
      }
      // add hardware info to device list
   }

   // free deviceInfoSet memory
   if (allDeviceClasses)
   {
      SetupDiDestroyDeviceInfoList(allDeviceClasses);
   }

   // return devices array
   return devices;
}

// init function called by the main init routine.
void DeviceListAddon::init(Handle<Object> target)
{
   FunctionList functionList {std::make_pair("getDevices", getDevices)};
   // initialize the base class
   baseInit(target, AddonName, functionList);
}

// gets a list of all processes with their associated IDs
void DeviceListAddon::getDevices(const FunctionCallbackInfo<Value>& args)
{
   // unwrap object so we can call the correct function on the instance
   auto deviceListAddon(ObjectWrap::Unwrap<DeviceListAddon>(args.Holder()));
   // return process list to caller
   args.GetReturnValue().Set(deviceListAddon->getDeviceList());
}

// Do all the magic to make this module accessible by node/javascript
// THE FIRST PARAMETER TO THE MACRO BELOW MUST MATCH THE MODULE FILENAME
NODE_MODULE(DeviceListAddon, DeviceListAddon::init)
