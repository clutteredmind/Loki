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
LokiAddonBase::LokiAddonBase() {}

// destructor
LokiAddonBase::~LokiAddonBase() {}

// init function called by the main init routine.
void LokiAddonBase::baseInit(Handle<Object> target, std::string addonName, FunctionList list)
{
   Isolate* isolate = Isolate::GetCurrent();

   // prepare constructor template
   tpl = FunctionTemplate::New(isolate, create);
   tpl->SetClassName(String::NewFromUtf8(isolate, addonName.c_str()));
   tpl->InstanceTemplate()->SetInternalFieldCount(1);
   // register describe method
   NODE_SET_PROTOTYPE_METHOD(tpl, "describe", describe);

   // register functions with node
   for (std::pair<const char*, v8::FunctionCallback> cb : list)
   {
      NODE_SET_PROTOTYPE_METHOD(tpl, cb.first, cb.second);
   }

   constructor.Reset(isolate, tpl->GetFunction());
   target->Set(String::NewFromUtf8(isolate, addonName.c_str()), tpl->GetFunction());
}

// placeholder for derived versions of this function
Local<Object> LokiAddonBase::describe()
{
   throw std::runtime_error("LokiAddonBase does not implement this method.");
}

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

void LokiAddonBase::describe(const FunctionCallbackInfo<Value>& args)
{
   // unwrap object so we can call the correct function on the instance
   auto lokiAddonBase(ObjectWrap::Unwrap<LokiAddonBase>(args.Holder()));
   // return addon description to caller
   args.GetReturnValue().Set(lokiAddonBase->describe());
}
