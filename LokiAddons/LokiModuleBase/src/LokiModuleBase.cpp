// 
// Purpose: Defines the base class for Loki's node modules
//

#include "LokiModuleBase.hpp"

namespace Loki
{
   // Constructor
   LokiModuleBase::LokiModuleBase() :
      last_error_string()
   {
   }

   // Get the last error string
   bool LokiModuleBase::GetLastErrorString(std::string& error_string)
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
   bool LokiModuleBase::SetLastErrorString(std::string& error_string)
   {
      bool result = false;

      if (!error_string.empty())
      {
         last_error_string = error_string;
         result = true;
      }

      return result;
   }
}

