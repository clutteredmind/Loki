// 
// Purpose: Declares the base class for Loki's node addons
//

#pragma once

// the Node headers
#include <node.h>
#include <node_object_wrap.h>

// standard includes
#include <string>

namespace Loki
{
   class LokiAddonBase : public node::ObjectWrap
   {
      public:
      // constructor
      LokiAddonBase() = default;
      // destructor
      virtual ~LokiAddonBase() = default;
      // Get the last error string
      bool GetLastErrorString(std::string& error_string);
      // Set the last error string
      void SetLastErrorString(std::string& error_string);

      protected:
      std::string last_error_string;
   };
}


