//
// Purpose: Declares the template class for Loki's Node addons
//

#pragma once

#include "LokiAddonBase.hpp"
#include "LokiAddonDescriptor.hpp"

#include <string>

namespace Loki
{
   template<typename TDerivedLokiAddon>
   class LokiAddon : public LokiAddonBase
   {
      public:
      // Destructor
      virtual ~LokiAddon() = default;
      // Creates a new instance of this class
      static void Create(const v8::FunctionCallbackInfo<v8::Value>& args)
      {
         auto isolate = args.GetIsolate();
         v8::HandleScope scope(isolate);

         if (args.IsConstructCall())
         {
            // invoked as constructor: 'new Object(...)'
            auto addon = new TDerivedLokiAddon();
            addon->Wrap(args.This());

            args.GetReturnValue().Set(args.This());
         }
         else
         {
            // invoked as a plain function 'Object(...)', turn into function call
            const int argc = 1;
            v8::Local<v8::Value> argv [argc] = {args [0]};
            auto ctor = v8::Local<v8::Function>::New(isolate, constructor);
            args.GetReturnValue().Set(ctor->NewInstance(argc, argv));
         }
      }
      // Retrieve this addon's information. Exposed to JavaScript
      static void GetAddonInfo(const v8::FunctionCallbackInfo<v8::Value>& args)
      {
         // unwrap object so we can call the correct function on the instance
         auto addon = ObjectWrap::Unwrap<TDerivedLokiAddon>(args.Holder());
         // return info object
         args.GetReturnValue().Set(addon->getAddonInfo(args.GetIsolate()));
      }

      protected:
      // Constructor
      LokiAddon() = default;
      // the v8 constructor
      static v8::Persistent<v8::Function> constructor;
      // This addon's descriptor
      static LokiAddonDescriptor descriptor;

      private:
      // Retrieve this addon's information.
      v8::Handle<v8::Object> getAddonInfo(v8::Isolate* isolate)
      {
         v8::HandleScope scope(isolate);

         // create array to hold addon information
         auto descriptor_object = v8::Object::New(isolate);
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "name"), v8::String::NewFromUtf8(isolate, descriptor.GetName().c_str()));
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "version"), v8::String::NewFromUtf8(isolate, descriptor.GetVersion().c_str()));
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "description"), v8::String::NewFromUtf8(isolate, descriptor.GetDescription().c_str()));
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "node_version"), v8::String::NewFromUtf8(isolate, descriptor.GetNodeVersion().c_str()));
         int function_counter = 0;
         auto exported_functions = v8::Array::New(isolate);
         for (auto function : descriptor.GetFunctions())
         {
            auto exported_function = v8::Object::New(isolate);

            // basic metadata
            exported_function->Set(v8::String::NewFromUtf8(isolate, "name"), v8::String::NewFromUtf8(isolate, function.name.c_str()));
            exported_function->Set(v8::String::NewFromUtf8(isolate, "description"), v8::String::NewFromUtf8(isolate, function.description.c_str()));

            // parameters
            int parameter_counter = 0;
            auto parameters = v8::Array::New(isolate);
            for (auto function_parameter : function.parameters)
            {
               auto parameter = v8::Object::New(isolate);
               parameter->Set(v8::String::NewFromUtf8(isolate, "name"), v8::String::NewFromUtf8(isolate, function_parameter.parameter.second.c_str()));
               parameter->Set(v8::String::NewFromUtf8(isolate, "type"), v8::String::NewFromUtf8(isolate, LokiAddonDescriptor::ConvertParameterTypeToString(function_parameter.parameter.first).c_str()));
               parameters->Set(parameter_counter, parameter);
               parameter_counter++;
            }
            exported_function->Set(v8::String::NewFromUtf8(isolate, "parameters"), parameters);

            // return type
            exported_function->Set(v8::String::NewFromUtf8(isolate, "return_type"), v8::String::NewFromUtf8(isolate, LokiAddonDescriptor::ConvertParameterTypeToString(function.return_type).c_str()));

            exported_functions->Set(function_counter, exported_function);
            function_counter++;
         }
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "functions"), exported_functions);

         return descriptor_object;
      }
   };
}