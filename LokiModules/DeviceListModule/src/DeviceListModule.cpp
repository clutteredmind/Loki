// 
// Purpose: Defines the node module that deals with the Windows device list
//

#include "DeviceListModule.hpp"

#define WINVER _WIN32_WINNT_WINBLUE

// windows API headers
#include <Windows.h>
#include <cfgmgr32.h> // for MAX_DEVICE_ID_LEN
#include <SetupAPI.h> // for SetupDiGetClassDevs, etc.

// standard includes
#include <vector>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Tell node which function to use to set up this module
NODE_MODULE (DeviceListModule, Loki::DeviceListModule::Initialize)

namespace Loki
{
   // The v8 constructor from the template class
   Persistent<Function> DeviceListModule::constructor;

   // The template class's descriptor object
   LokiModuleDescriptor DeviceListModule::descriptor;

   // module metadata
   const std::string MODULE_NAME = "DeviceListModule";
   const std::string MODULE_DISPLAY_NAME = "Device List";
   const int MODULE_VERSION[3]{ 1 /*major*/, 0 /*minor*/, 0 /*patch*/ };
   const std::string MODULE_DESCRIPTION = "Retrieves a list of installed devices via the Windows API";

   // Pre-initialization.
   void DeviceListModule::Initialize (Handle<Object> target)
   {
      // set module metadata
      descriptor.SetName (MODULE_NAME);
      descriptor.SetDisplayName (MODULE_DISPLAY_NAME);
      descriptor.SetVersion (LokiModuleDescriptor::GetVersionStringFromArray (MODULE_VERSION));
      descriptor.SetDescription (MODULE_DESCRIPTION);
      // register this class's exported functions for the framework
      descriptor.AddFunction ("getDevices", GetDevices, "Gets a list of all installed devices.", NO_PARAMETERS, RETURNS_AN OBJECT);
      // Register module with Node
      Register (target);
   }

   // Gets a list of all installed devices. Exposed to JavaScript.
   void DeviceListModule::GetDevices (const FunctionCallbackInfo<Value>& args)
   {
      auto isolate = args.GetIsolate ();
      HandleScope scope (isolate);

      // validate parameters
      std::string error_string;
      if (descriptor.ValidateParameters (GetDevices, args, error_string))
      {
         // unwrap object so we can call the correct function on the instance
         auto device_list_module = ObjectWrap::Unwrap<DeviceListModule> (args.Holder ());
         // return process list to caller
         args.GetReturnValue ().Set (device_list_module->getDevices (isolate));
      }
      else
      {
         // if parameter validation failed for whatever reason, report the error
         isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, error_string.c_str ())));
      }
   }

   // Gets a list of all installed devices.
   Local<Array> DeviceListModule::getDevices (Isolate* isolate)
   {
      HandleScope scope (isolate);

      // the array of devices to return to JavaScript
      auto devices = Array::New (isolate);

      // Create a HDEVINFO with all device classes
      HDEVINFO all_device_classes;

      try
      {
         all_device_classes = SetupDiGetClassDevs (NULL, NULL, NULL, DIGCF_ALLCLASSES);

         if (all_device_classes == INVALID_HANDLE_VALUE)
         {
            throw std::exception ("Unable to get all device classes.");
         }
         else
         {
            SP_DEVINFO_DATA device_info_data;
            ZeroMemory (&device_info_data, sizeof (SP_DEVINFO_DATA));
            device_info_data.cbSize = sizeof (SP_DEVINFO_DATA);
            DWORD device_index = 0;
            bool keep_processing = true;

            // loop through and collect all class GUIDs
            std::vector<GUID> all_class_guids;
            while (keep_processing)
            {
               // get device information
               SetupDiEnumDeviceInfo (all_device_classes, device_index, &device_info_data);
               // save the GUID if it's not already in the list
               if (std::find (all_class_guids.begin (), all_class_guids.end (), device_info_data.ClassGuid) == all_class_guids.end ())
               {
                  all_class_guids.push_back (device_info_data.ClassGuid);
               }
               // increment device index
               device_index++;
               // check to see if we should be done
               keep_processing = !(GetLastError () == ERROR_NO_MORE_ITEMS);
            }

            // loop through GUIDs to get hardware info for each
            for (auto guid : all_class_guids)
            {
               auto device_info = SetupDiGetClassDevs (&guid, NULL, NULL, DIGCF_PRESENT);
               if (device_info == INVALID_HANDLE_VALUE)
               {
                  OLECHAR* guid_as_string;
                  StringFromCLSID (guid, &guid_as_string);
                  std::wstring error_message = L"Unable to get device info for GUID: ";
                  error_message += guid_as_string;
                  CoTaskMemFree (guid_as_string);
                  throw std::exception (std::string (error_message.begin (), error_message.end ()).c_str ());
               }
               else
               {
                  TCHAR device_instance_id[MAX_DEVICE_ID_LEN];
                  // reset device_info_data
                  ZeroMemory (&device_info_data, sizeof (SP_DEVINFO_DATA));
                  device_info_data.cbSize = sizeof (SP_DEVINFO_DATA);
                  // reset device index
                  device_index = 0;
                  // a counter for indexing the v8 array
                  int counter = 0;
                  keep_processing = true;
                  // make an initial call to SetupDiEnumDeviceInfo to populate deviceInfoData
                  SetupDiEnumDeviceInfo (device_info, device_index, &device_info_data);
                  while (keep_processing)
                  {
                     ZeroMemory (device_instance_id, MAX_DEVICE_ID_LEN);
                     if (SetupDiGetDeviceInstanceId (device_info, &device_info_data, device_instance_id, MAX_DEVICE_ID_LEN, NULL))
                     {
                        // store device information
                        auto device = Object::New (isolate);

                        // save device instance ID
                        std::wstring device_instance_id_string (device_instance_id);
                        device->Set (String::NewFromUtf8 (isolate, "deviceInstanceId"), String::NewFromUtf8 (isolate, std::string (device_instance_id_string.begin (), device_instance_id_string.end ()).c_str ()));

                        BYTE* buffer;
                        DWORD property_size = 0;

                        // get friendly name size
                        SetupDiGetDeviceRegistryProperty (device_info, &device_info_data, SPDRP_FRIENDLYNAME, NULL, NULL, 0, &property_size);
                        property_size = property_size > 1 ? property_size : 1;
                        // set buffer size and zero it out
                        buffer = new BYTE[property_size];
                        ZeroMemory (buffer, property_size);
                        // get friendly name
                        SetupDiGetDeviceRegistryProperty (device_info, &device_info_data, SPDRP_FRIENDLYNAME, NULL, buffer, property_size, NULL);
                        // save friendly name
                        std::wstring friendly_name (reinterpret_cast<wchar_t*>(buffer));
                        device->Set (String::NewFromUtf8 (isolate, "friendlyName"), String::NewFromUtf8 (isolate, std::string (friendly_name.begin (), friendly_name.end ()).c_str ()));
                        // release buffer memory
                        delete[] buffer;

                        // get class size
                        SetupDiGetDeviceRegistryProperty (device_info, &device_info_data, SPDRP_CLASS, NULL, NULL, 0, &property_size);
                        property_size = property_size > 1 ? property_size : 1;
                        // set buffer size and zero it out
                        buffer = new BYTE[property_size];
                        ZeroMemory (buffer, property_size);
                        // get class
                        SetupDiGetDeviceRegistryProperty (device_info, &device_info_data, SPDRP_CLASS, NULL, buffer, property_size, NULL);
                        // save class
                        std::wstring class_name (reinterpret_cast<wchar_t*>(buffer));
                        device->Set (String::NewFromUtf8 (isolate, "className"), String::NewFromUtf8 (isolate, std::string (class_name.begin (), class_name.end ()).c_str ()));
                        // release buffer memory
                        delete[] buffer;

                        // get hardware id size
                        SetupDiGetDeviceRegistryProperty (device_info, &device_info_data, SPDRP_HARDWAREID, NULL, NULL, 0, &property_size);
                        property_size = property_size > 1 ? property_size : 1;
                        // set buffer size and zero it out
                        buffer = new BYTE[property_size];
                        ZeroMemory (buffer, property_size);
                        // get hardware id
                        SetupDiGetDeviceRegistryProperty (device_info, &device_info_data, SPDRP_HARDWAREID, NULL, buffer, property_size, NULL);
                        // save hardware id
                        std::wstring hardware_id (reinterpret_cast<wchar_t*>(buffer));
                        device->Set (String::NewFromUtf8 (isolate, "hardwareId"), String::NewFromUtf8 (isolate, std::string (hardware_id.begin (), hardware_id.end ()).c_str ()));
                        // release buffer memory
                        delete[] buffer;

                        // save device
                        devices->Set (Integer::New (isolate, counter), device);
                        // increment counter
                        counter++;
                     }

                     // increment device index
                     device_index++;
                     // attempt to get next device
                     SetupDiEnumDeviceInfo (device_info, device_index, &device_info_data);
                     // check to see if we should be done
                     keep_processing = !(GetLastError () == ERROR_NO_MORE_ITEMS);
                  }
               }
            }
         }
      }
      catch (std::exception& exception)
      {
         isolate->ThrowException (Exception::Error (String::NewFromUtf8 (isolate, exception.what ())));
      }

      // free dynamic memory
      if (all_device_classes)
      {
         SetupDiDestroyDeviceInfoList (all_device_classes);
      }

      // return devices array, which may be empty if there were failures above
      return devices;
   }
}
