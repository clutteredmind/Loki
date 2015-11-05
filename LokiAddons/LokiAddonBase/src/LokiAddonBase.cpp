// 
// Purpose: Defines the base class for Loki's node addons
//

#include "LokiAddonBase.hpp"

namespace Loki
{
   // Get the last error string
   bool LokiAddonBase::GetLastErrorString(std::string& error_string)
   {
      bool result = false;

      if (!last_error_string.empty())
      {
         error_string = last_error_string;
         result = true;
      }

      return result;
   }

   // Set the last error string
   void LokiAddonBase::SetLastErrorString(std::string& error_string)
   {
      if (!error_string.empty())
      {
         last_error_string = error_string;
      }
   }
}

