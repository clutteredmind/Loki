// 
// Purpose: Declares the base class for Loki's node modules
//

#pragma once

// standard includes
#include <string>

namespace Loki
{
   class LokiModuleBase
   {
      public:
      // constructor
      LokiModuleBase();
      // destructor
      virtual ~LokiModuleBase() = default;
      // Get the last error string
      bool GetLastErrorString(std::string& error_string);

      protected:
      // Set the last error string
      bool SetLastErrorString(std::string& error_string);

      private:
      // The last error message
      std::string last_error_string;
   };
}


