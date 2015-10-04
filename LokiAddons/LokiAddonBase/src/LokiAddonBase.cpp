// 
// Purpose: Defines the base class for Loki's node addons
//

#include "LokiAddonBase.hpp"

// the node libraries
#include <node.h>

// use the namespace so we don't have to have v8:: everywhere
using namespace v8;

// forward declare constructor
Persistent<Function> LokiAddonBase::constructor;
// forward declare v8 function template
Local<FunctionTemplate> LokiAddonBase::tpl;

// constructor
LokiAddonBase::LokiAddonBase() { }

// destructor
LokiAddonBase::~LokiAddonBase() { }

// create a new instance and wrap it up to be passed back to node
void LokiAddonBase::create(const FunctionCallbackInfo<Value>& args)
{
   Isolate* isolate = args.GetIsolate();

   if (args.IsConstructCall())
   {
      // invoked as constructor: 'new Object(...)'
      LokiAddonBase* lokiAddonBase = new LokiAddonBase();
      lokiAddonBase->Wrap(args.This());

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
