//
// Purpose: Defines a Node module
//

#include "$LokiModuleName$.hpp"

// use the v8 namespace so we don't have to have v8:: everywhere
using namespace v8;

// Tell node which function to use to set up this module
NODE_MODULE($LokiModuleName$, Loki::$LokiModuleName$::Initialize)

namespace Loki
{
   // The v8 constructor from the template class
   Persistent<Function> $LokiModuleName$::constructor;

   // The template class's descriptor object
   LokiModuleDescriptor $LokiModuleName$::descriptor;

   // module metadata
   const std::string module_name = "$LokiModuleName$";
   const int module_version [3] {$MajorVersionNumber$ /*major*/, $MinorVersionNumber$ /*minor*/, $PatchVersionNumber$ /*patch*/};
   const std::string module_description = "$LokiModuleDescription$";

   // Pre-initialization.
   void $LokiModuleName$::Initialize(Handle<Object> target)
   {
      // set module metadata
      descriptor.SetName(module_name);
      descriptor.SetVersion(LokiModuleDescriptor::GetVersionStringFromArray(module_version));
      descriptor.SetDescription(module_description);
      // TODO: register this class's exported functions for the framework
      // Register module with Node
      Register(target);
   }
}
