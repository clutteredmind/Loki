//
// Purpose: Defines a Node addon
//

#include "$LokiAddonName$.hpp"

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Tell node which function to use to set up this addon
NODE_MODULE($LokiAddonName$, Loki::$LokiAddonName$::Initialize)

namespace Loki
{
   // The v8 constructor from the template class
   Persistent<Function> $LokiAddonName$::constructor;

   // The template class's descriptor object
   LokiAddonDescriptor $LokiAddonName$::descriptor;

   // addon metadata
   const std::string addon_name = "$LokiAddonName$";
   const int addon_version [3] {$MajorVersionNumber$ /*major*/, $MinorVersionNumber$ /*minor*/, $PatchVersionNumber$ /*patch*/};
   const std::string addon_description = "$LokiAddonDescription$";

   // Pre-initialization.
   void $LokiAddonName$::Initialize(Handle<Object> target)
   {
      // set addon metadata
      descriptor.SetName(addon_name);
      descriptor.SetVersion(LokiAddonDescriptor::GetVersionStringFromArray(addon_version));
      descriptor.SetDescription(addon_description);
      // TODO: register this class's exported functions for the framework
      // Register addon with Node
      Register(target);
   }
}
