//
// Purpose: Declares a Node module
//

#pragma once

#include "LokiModule.hpp"

namespace Loki
{
   class $LokiModuleName$ : public LokiModule<$LokiModuleName$>
   {
      public:
      // Constructor
      $LokiModuleName$() = default;
      // destructor
      virtual ~$LokiModuleName$() override = default;
      // Initialization function used by Node to set up this module.
      static void Initialize(v8::Handle <v8::Object> target);
      // TODO: add your exported methods

      protected:
      // TODO: add your private methods
   };
}
