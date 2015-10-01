// 
// Purpose: Defines the addon that connects C++ to node's javascript.
//

#include "DeviceListAddon.hpp"

// Include the node definitions
#include <node.h>

// Called automatically by node.
void InitAll(v8::Handle<v8::Object> target)
{
   DeviceListAddon::init(target);
}

// Do all the magic to make this module accessible by node/javascript
// THE FIRST PARAMETER TO THE MACRO BELOW MUST MATCH THE MODULE FILENAME
NODE_MODULE(DeviceListAddon, InitAll)
