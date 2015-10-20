// 
// Purpose: Declares the base class for Loki's node addons
//

#pragma once

// the Node headers
#include <node.h>
#include <node_object_wrap.h>

class LokiAddonBase : public node::ObjectWrap
{
   public:
   // constructor
   LokiAddonBase();
   // destructor
   virtual ~LokiAddonBase();
   // the v8 constructor
   static v8::Persistent<v8::Function> constructor;
};
