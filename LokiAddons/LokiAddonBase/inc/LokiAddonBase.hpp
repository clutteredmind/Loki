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
      // destructor
      virtual ~LokiAddonBase() = default;
      // Get the last error string
      bool GetLastErrorString(std::string& error_string);

      protected:
      // constructor
      LokiAddonBase() = default;
      // Set the last error string
      void SetLastErrorString(std::string& error_string);

      private:
      // The last error message
      std::string last_error_string;
   };
}


