//
// Purpose: Declares a Node addon
//

#pragma once

#include "LokiAddon.hpp"

namespace Loki
{
   class $LokiAddonName$ : public LokiAddon<$LokiAddonName$>
   {
      public:
      // Constructor
      $LokiAddonName$() = default;
      // destructor
      virtual ~$LokiAddonName$() override = default;
      // Initialization function used by Node to set up this addon.
      static void Initialize(v8::Handle <v8::Object> target);
      // TODO: add your exported methods

      protected:
      // TODO: add your private methods
   };
}
