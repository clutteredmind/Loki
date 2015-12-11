//
// Purpose: Declares the template class for Loki's Node modules
//

#pragma once

// the Node headers
#include <node.h>
#include <node_object_wrap.h>

#include "LokiModuleBase.hpp"
#include "LokiModuleDescriptor.hpp"

#include <string>

namespace Loki
{
   template<typename TDerivedLokiModule>
   class LokiModule : public node::ObjectWrap,
                     public LokiModuleBase
   {
      public:
      // Registers this module with Node
      static void Register(v8::Handle <v8::Object> target)
      {
         auto isolate = target->GetIsolate();

         // Prepare constructor template
         auto function_template = v8::FunctionTemplate::New(isolate, Create);
         function_template->SetClassName(v8::String::NewFromUtf8(isolate, descriptor.GetName().c_str()));
         function_template->InstanceTemplate()->SetInternalFieldCount(1);

         // add GetModuleInfo function
         descriptor.AddFunction("getModuleInfo", GetModuleInfo, "Retrieves framework information about this module.", NO_PARAMETERS, RETURNS_AN OBJECT);

         // Set up exported function prototypes
         for (auto function : descriptor.GetFunctions())
         {
            NODE_SET_PROTOTYPE_METHOD(function_template, function.name.c_str(), function.callback);
         }

         constructor.Reset(isolate, function_template->GetFunction());
         target->Set(v8::String::NewFromUtf8(isolate, descriptor.GetName().c_str()), function_template->GetFunction());
      }

      // Creates a new instance of this class
      static void Create(const v8::FunctionCallbackInfo<v8::Value>& args)
      {
         auto isolate = args.GetIsolate();
         v8::HandleScope scope(isolate);

         if (args.IsConstructCall())
         {
            // invoked as constructor: 'new Object(...)'
            auto module = new TDerivedLokiModule();
            module->Wrap(args.This());

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

      // Retrieve this module's information. Exposed to JavaScript
      static void GetModuleInfo(const v8::FunctionCallbackInfo<v8::Value>& args)
      {
         auto isolate = args.GetIsolate();
         HandleScope scope(isolate);

         // validate parameters
         std::string error_string;
         if (descriptor.ValidateParameters(GetModuleInfo, args, error_string))
         {
            // unwrap object so we can call the correct function on the instance
            auto module = ObjectWrap::Unwrap<TDerivedLokiModule>(args.Holder());
            // return info object
            args.GetReturnValue().Set(module->getModuleInfo(isolate));
         }
         else
         {
            // if parameter validation failed for whatever reason, report the error
            isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, error_string.c_str())));
         }
      }

      protected:
      // the v8 constructor
      static v8::Persistent<v8::Function> constructor;
      // This module's descriptor
      static LokiModuleDescriptor descriptor;
      // Retrieve this module's information.
      v8::Handle<v8::Object> getModuleInfo(v8::Isolate* isolate)
      {
         v8::HandleScope scope(isolate);

         // create array to hold module information
         auto descriptor_object = v8::Object::New(isolate);
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "name"), v8::String::NewFromUtf8(isolate, descriptor.GetName().c_str()));
         descriptor_object->Set (v8::String::NewFromUtf8 (isolate, "displayName"), v8::String::NewFromUtf8 (isolate, descriptor.GetDisplayName ().c_str ()));
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "version"), v8::String::NewFromUtf8(isolate, descriptor.GetVersion().c_str()));
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "description"), v8::String::NewFromUtf8(isolate, descriptor.GetDescription().c_str()));
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "nodeVersion"), v8::String::NewFromUtf8(isolate, descriptor.GetNodeVersion().c_str()));
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
               parameter->Set(v8::String::NewFromUtf8(isolate, "type"), v8::String::NewFromUtf8(isolate, LokiModuleDescriptor::ConvertParameterTypeToString(function_parameter.parameter.first).c_str()));
               parameters->Set(parameter_counter, parameter);
               parameter_counter++;
            }
            exported_function->Set(v8::String::NewFromUtf8(isolate, "parameters"), parameters);

            // return type
            exported_function->Set(v8::String::NewFromUtf8(isolate, "returnType"), v8::String::NewFromUtf8(isolate, LokiModuleDescriptor::ConvertParameterTypeToString(function.return_type).c_str()));

            exported_functions->Set(function_counter, exported_function);
            function_counter++;
         }
         descriptor_object->Set(v8::String::NewFromUtf8(isolate, "functions"), exported_functions);

         return descriptor_object;
      }
   };
}