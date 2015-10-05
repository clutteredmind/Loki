// 
// Purpose: Declares the base class for Loki's node addons
//

#pragma once

#include <node.h>
#include <node_object_wrap.h>

#include <string>
#include <vector>
#include <utility>

#define FUNCTION_PAIR(NAME) std::make_pair <const char *, v8::FunctionCallback>(#NAME, NAME)

using FunctionList = std::vector <std::pair<const char*, v8::FunctionCallback>>;

class LokiAddonBase : public node::ObjectWrap
{
   public:
   // Initialization.  This function is required by node.
   static void baseInit(v8::Handle <v8::Object>, std::string, FunctionList);

   // constructor
   LokiAddonBase();
   // destructor
   ~LokiAddonBase();

   // function template
   static v8::Local<v8::FunctionTemplate> tpl;
   // constructor
   static v8::Persistent<v8::Function> constructor;

#pragma region Node.js functions

   static void create(const v8::FunctionCallbackInfo<v8::Value>& args);

#pragma endregion // Node.js functions
};
