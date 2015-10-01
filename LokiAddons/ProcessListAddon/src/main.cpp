// 
// Purpose: Defines the module that connects C++ to node's javascript.
//

#include "ProcessListAddon.hpp"

// Include the node definitions
#include <node.h>

// Called automatically by node.
void InitAll(v8::Handle<v8::Object> target)
{
   ProcessListAddon::init(target);
}

// Do all the magic to make this module accessible by node/javascript
// THE FIRST PARAMETER TO THE MACRO BELOW MUST MATCH THE MODULE FILENAME
NODE_MODULE(ProcessListAddon, InitAll)
