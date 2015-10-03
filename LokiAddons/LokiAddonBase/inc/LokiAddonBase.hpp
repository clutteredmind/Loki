// 
// Purpose: Declares the base class for Loki's node addons
//

#pragma once

#include <node.h>
#include <node_object_wrap.h>

class LokiAddonBase : public node::ObjectWrap
{
   public:
   // constructor
   LokiAddonBase();
   // destructor
   ~LokiAddonBase();

   // returns a JSON structure that describes this addon
   v8::Local<v8::Array> describe();

   // describe function exposed to JavaScript
   static void describe(const v8::FunctionCallbackInfo<v8::Value>& args);
};