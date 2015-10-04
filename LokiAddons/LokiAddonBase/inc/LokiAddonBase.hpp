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
};