// 
// Purpose: Defines the base class for Loki's node addons
//

#include "LokiAddonBase.hpp"

// the node libraries
#include <node.h>

// use the namespace so we don't have to have v8:: everywhere
using namespace v8;

// constructor
LokiAddonBase::LokiAddonBase() { }

// destructor
LokiAddonBase::~LokiAddonBase() { }

// builds a descriptive JSON data structure about this addon
Local<Array> LokiAddonBase::describe()
{
   Isolate* isolate = Isolate::GetCurrent();

   auto description = Array::New(isolate);

   return description;
}

// describe this addon's methods to JavaScript
void LokiAddonBase::describe(const FunctionCallbackInfo<Value>& args)
{
   // unwrap object so we can call the correct function on the instance
   auto lokiAddonBase(ObjectWrap::Unwrap<LokiAddonBase>(args.Holder()));
   // return process list to caller
   args.GetReturnValue().Set(lokiAddonBase->describe());
}