// 
// Purpose: Declares the base class for Loki's node addons
//

#pragma once

#include <node.h>
#include <node_object_wrap.h>

#include <string>
#include <vector>
#include <utility>

using FunctionList = std::vector <std::pair<const char*, v8::FunctionCallback>>;

class LokiAddonBase : public node::ObjectWrap
{
   public:
   // constructor
   LokiAddonBase();
   // destructor
   ~LokiAddonBase();

   // function template
   static v8::Local<v8::FunctionTemplate> tpl;
   // constructor
   static v8::Persistent<v8::Function> constructor;
   // Initialization.  This function is required by node.
   static void baseInit(v8::Handle <v8::Object>, std::string, FunctionList);
   // describes this object in JSON
   virtual v8::Local<v8::Object> describe();

#pragma region Node.js functions
   // handles JavaScript object creation
   static void create(const v8::FunctionCallbackInfo<v8::Value>& args);
   // advertise the derived class's capabilities
   static void describe(const v8::FunctionCallbackInfo<v8::Value>& args);

#pragma endregion // Node.js functions
};
