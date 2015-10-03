// 
// Purpose: Defines the node addon that deals with the Windows device list
//

#include "DeviceListAddon.hpp"

// the node libraries
#include <node.h>

#define WINVER _WIN32_WINNT_WINBLUE
#define _WIN32_WINNT _WIN32_WINNT_WINBLUE

// windows API headers
#include <Windows.h>
#include <SetupAPI.h>

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

Persistent<Function> DeviceListAddon::constructor;

// constructor
DeviceListAddon::DeviceListAddon() {}

// destructor
DeviceListAddon::~DeviceListAddon() {}

// gets process list from Windows
Local<Array> DeviceListAddon::getDevices()
{
   Isolate* isolate = Isolate::GetCurrent();

   Local<Array> devices = Array::New(isolate);

   HDEVINFO deviceInfoSet;

   // Create a HDEVINFO with all present devices.
   deviceInfoSet = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);

   if (deviceInfoSet == INVALID_HANDLE_VALUE)
   {
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Unable to get device info set.")));
   }
   else
   {
      SP_DEVINFO_DATA deviceInfoData;
      ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
      deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
      DWORD deviceIndex = 0;

      while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData))
      {
         // I don't know what to do here.

         deviceIndex++;
      }
   }

   // free deviceInfoSet memory
   if (deviceInfoSet)
   {
      SetupDiDestroyDeviceInfoList(deviceInfoSet);
   }

   // return devices array
   return devices;
}

// init function called by the main init routine.
void DeviceListAddon::init(Handle<Object> target)
{
   Isolate* isolate = Isolate::GetCurrent();

   // Prepare constructor template
   Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, create);
   tpl->SetClassName(String::NewFromUtf8(isolate, "DeviceListAddon"));
   tpl->InstanceTemplate()->SetInternalFieldCount(1);

   // Set up function prototypes
   NODE_SET_PROTOTYPE_METHOD(tpl, "getDevices", getDevices);

   constructor.Reset(isolate, tpl->GetFunction());
   target->Set(String::NewFromUtf8(isolate, "DeviceListAddon"), tpl->GetFunction());
}

// create a new instance and wrap it up to be passed back to node
void DeviceListAddon::create(const FunctionCallbackInfo<Value>& args)
{
   Isolate* isolate = args.GetIsolate();

   if (args.IsConstructCall())
   {
      // invoked as constructor: 'new Object(...)'
      DeviceListAddon* deviceListAddon = new DeviceListAddon();
      deviceListAddon->Wrap(args.This());

      args.GetReturnValue().Set(args.This());
   }
   else
   {
      // invoked as a plain function 'Object(...)', turn into function call
      const int argc = 1;
      Local<Value> argv [argc] = {args [0]};
      Local<Function> ctor = Local<Function>::New(isolate, constructor);
      args.GetReturnValue().Set(ctor->NewInstance(argc, argv));
   }
}

// gets a list of all processes with their associated IDs
void DeviceListAddon::getDevices(const FunctionCallbackInfo<Value>& args)
{
   // unwrap object so we can call the correct function on the instance
   DeviceListAddon* deviceListAddon(ObjectWrap::Unwrap<DeviceListAddon>(args.Holder()));
   // return process list to caller
   args.GetReturnValue().Set(deviceListAddon->getDevices());
}
