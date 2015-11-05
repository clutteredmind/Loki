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
NODE_MODULE(DeviceListAddon, Loki::DeviceListAddon::Initialize)

namespace Loki
{
   // The v8 constructor
   v8::Persistent<v8::Function> DeviceListAddon::constructor;

   // Initialization. This function is required by node.
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
         auto device_list_addon = new DeviceListAddon();
         device_list_addon->Wrap(args.This());
         args.GetReturnValue().Set(args.This());
      }
      else
      {
         // Invoked as plain function `MyObject(...)`, turn into construct call.
         auto ctor = Local<Function>::New(isolate, constructor);
         args.GetReturnValue().Set(ctor->NewInstance());
      }
   }

   // Gets a list of all installed devices. Exposed to JavaScript.
   void DeviceListAddon::GetDevices(const FunctionCallbackInfo<Value>& args)
   {
      // unwrap object so we can call the correct function on the instance
      auto device_list_addon = ObjectWrap::Unwrap<DeviceListAddon>(args.Holder());
      // return process list to caller
      args.GetReturnValue().Set(device_list_addon->getDevices());
   }

   // Gets a list of all installed devices.
   Local<Array> DeviceListAddon::getDevices()
   {
      auto isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      // the array of devices to return to JavaScript
      auto devices = Array::New(isolate);

      // Create a HDEVINFO with all device classes
      HDEVINFO all_device_classes;

      try
      {
         all_device_classes = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES);

         if (all_device_classes == INVALID_HANDLE_VALUE)
         {
            throw std::exception("Unable to get all device classes.");
         }
         else
         {
            SP_DEVINFO_DATA device_info_data;
            ZeroMemory(&device_info_data, sizeof(SP_DEVINFO_DATA));
            device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
            DWORD device_index = 0;
            bool keep_processing = true;

            // loop through and collect all class GUIDs
            std::vector<GUID> all_class_guids;
            while (keep_processing)
            {
               // get device information
               SetupDiEnumDeviceInfo(all_device_classes, device_index, &device_info_data);
               // save the GUID if it's not already in the list
               if (std::find(all_class_guids.begin(), all_class_guids.end(), device_info_data.ClassGuid) == all_class_guids.end())
               {
                  all_class_guids.push_back(device_info_data.ClassGuid);
               }
               // increment device index
               device_index++;
               // check to see if we should be done
               keep_processing = !(GetLastError() == ERROR_NO_MORE_ITEMS);
            }

            // loop through GUIDs to get hardware info for each
            for (auto guid : all_class_guids)
            {
               auto device_info = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT);
               if (device_info == INVALID_HANDLE_VALUE)
               {
                  OLECHAR* guid_as_string;
                  StringFromCLSID(guid, &guid_as_string);
                  std::wstring error_message = L"Unable to get device info for GUID: ";
                  error_message += guid_as_string;
                  CoTaskMemFree(guid_as_string);
                  throw std::exception(std::string(error_message.begin(), error_message.end()).c_str());
               }
               else
               {
                  TCHAR device_instance_id [MAX_DEVICE_ID_LEN];
                  // reset device_info_data
                  ZeroMemory(&device_info_data, sizeof(SP_DEVINFO_DATA));
                  device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
                  // reset device index
                  device_index = 0;
                  // a counter for indexing the v8 array
                  int counter = 0;
                  keep_processing = true;
                  // make an initial call to SetupDiEnumDeviceInfo to populate deviceInfoData
                  SetupDiEnumDeviceInfo(device_info, device_index, &device_info_data);
                  while (keep_processing)
                  {
                     ZeroMemory(device_instance_id, MAX_DEVICE_ID_LEN);
                     if (SetupDiGetDeviceInstanceId(device_info, &device_info_data, device_instance_id, MAX_DEVICE_ID_LEN, NULL))
                     {
                        // store device information
                        auto device = Object::New(isolate);

                        // save device instance ID
                        std::wstring device_instance_id_string(device_instance_id);
                        device->Set(String::NewFromUtf8(isolate, "device_instance_id"), String::NewFromUtf8(isolate, std::string(device_instance_id_string.begin(), device_instance_id_string.end()).c_str()));

                        BYTE* buffer;
                        DWORD property_size = 0;

                        // get friendly name size
                        SetupDiGetDeviceRegistryProperty(device_info, &device_info_data, SPDRP_FRIENDLYNAME, NULL, NULL, 0, &property_size);
                        property_size = property_size > 1 ? property_size : 1;
                        // set buffer size and zero it out
                        buffer = new BYTE [property_size];
                        ZeroMemory(buffer, property_size);
                        // get friendly name
                        SetupDiGetDeviceRegistryProperty(device_info, &device_info_data, SPDRP_FRIENDLYNAME, NULL, buffer, property_size, NULL);
                        // save friendly name
                        std::wstring friendly_name(reinterpret_cast<wchar_t*>(buffer));
                        device->Set(String::NewFromUtf8(isolate, "friendly_name"), String::NewFromUtf8(isolate, std::string(friendly_name.begin(), friendly_name.end()).c_str()));
                        // release buffer memory
                        delete [] buffer;

                        // get class size
                        SetupDiGetDeviceRegistryProperty(device_info, &device_info_data, SPDRP_CLASS, NULL, NULL, 0, &property_size);
                        property_size = property_size > 1 ? property_size : 1;
                        // set buffer size and zero it out
                        buffer = new BYTE [property_size];
                        ZeroMemory(buffer, property_size);
                        // get class
                        SetupDiGetDeviceRegistryProperty(device_info, &device_info_data, SPDRP_CLASS, NULL, buffer, property_size, NULL);
                        // save class
                        std::wstring class_name(reinterpret_cast<wchar_t*>(buffer));
                        device->Set(String::NewFromUtf8(isolate, "class_name"), String::NewFromUtf8(isolate, std::string(class_name.begin(), class_name.end()).c_str()));
                        // release buffer memory
                        delete [] buffer;

                        // get hardware id size
                        SetupDiGetDeviceRegistryProperty(device_info, &device_info_data, SPDRP_HARDWAREID, NULL, NULL, 0, &property_size);
                        property_size = property_size > 1 ? property_size : 1;
                        // set buffer size and zero it out
                        buffer = new BYTE [property_size];
                        ZeroMemory(buffer, property_size);
                        // get hardware id
                        SetupDiGetDeviceRegistryProperty(device_info, &device_info_data, SPDRP_HARDWAREID, NULL, buffer, property_size, NULL);
                        // save hardware id
                        std::wstring hardware_id(reinterpret_cast<wchar_t*>(buffer));
                        device->Set(String::NewFromUtf8(isolate, "hardware_id"), String::NewFromUtf8(isolate, std::string(hardware_id.begin(), hardware_id.end()).c_str()));
                        // release buffer memory
                        delete [] buffer;

                        // save device
                        devices->Set(Integer::New(isolate, counter), device);
                        // increment counter
                        counter++;
                     }

                     // increment device index
                     device_index++;
                     // attempt to get next device
                     SetupDiEnumDeviceInfo(device_info, device_index, &device_info_data);
                     // check to see if we should be done
                     keep_processing = !(GetLastError() == ERROR_NO_MORE_ITEMS);
                  }
               }
            }
         }
      }
      catch (std::exception& exception)
      {
         isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, exception.what())));
      }

      // free dynamic memory
      if (all_device_classes)
      {
         SetupDiDestroyDeviceInfoList(all_device_classes);
      }

      // return devices array, which may be empty if there were failures above
      return devices;
   }
}
