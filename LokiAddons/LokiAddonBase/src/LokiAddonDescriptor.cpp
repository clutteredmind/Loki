// 
// Purpose: Defines the class that describes a particular Loki addon
//

#include "LokiAddonDescriptor.hpp"

// the node version header
#include <node_version.h>

namespace Loki
{
   // Constructor
   LokiAddonDescriptor::LokiAddonDescriptor() :
      name(), version(), description(), functions()
   {

   }

   // Converts an array of integers to a version number string
   std::string LokiAddonDescriptor::GetVersionStringFromArray(const int version_numbers [3])
   {
      return (std::to_string(version_numbers [0]) + '.' + std::to_string(version_numbers [1]) + '.' + std::to_string(version_numbers [2]));
   }

   // Converts a ParameterType to its string representation
   std::string LokiAddonDescriptor::ConvertParameterTypeToString(const ParameterType parameter_type)
   {
      std::string type_as_string = "undefined";

      switch (parameter_type)
      {
         case ParameterType::BOOL:
            type_as_string = "bool";
            break;
         case ParameterType::STRING:
            type_as_string = "string";
            break;
         case ParameterType::NUMBER:
            type_as_string = "number";
            break;
         case ParameterType::FUNCTION:
            type_as_string = "function";
            break;
         case ParameterType::ARRAY:
            type_as_string = "array";
            break;
         case ParameterType::OBJECT:
            type_as_string = "object";
            break;
         case ParameterType::DATE:
            type_as_string = "date";
            break;
      }

      return type_as_string;
   }

   // Setter for the addon's name
   void LokiAddonDescriptor::SetName(const std::string& name)
   {
      this->name = name;
   }

   // Setter for the addon's version number
   void LokiAddonDescriptor::SetVersion(const std::string& version)
   {
      this->version = version;
   }

   // Setter for the addon's description
   void LokiAddonDescriptor::SetDescription(const std::string& description)
   {
      this->description = description;
   }

   // Adds an exported function description
   void LokiAddonDescriptor::AddFunction(const LokiFunction new_function)
   {
      functions.push_back(new_function);
   }

   // Adds an exported function description
   void LokiAddonDescriptor::AddFunction(const std::string& name,
      const v8::FunctionCallback callback,
      const std::string& description,
      const std::vector<Parameter> parameters,
      const ParameterType return_type)
   {
      LokiFunction function(name, callback, description, parameters, return_type);
      functions.push_back(function);
   }

   // Removes a function description by index
   bool LokiAddonDescriptor::RemoveFunction(const unsigned index)
   {
      auto result = false;

      if (functions.size() > 0 && (index >= 0 && index < functions.size()))
      {
         // remove function
         functions.erase(functions.begin() + index);
         result = true;
      }

      return result;
   }

   // Accessor for the addon's name
   std::string LokiAddonDescriptor::GetName()
   {
      return name;
   }

   // Accessor for the addon's version number
   std::string LokiAddonDescriptor::GetVersion()
   {
      return version;
   }

   // Accessor for the addon's description
   std::string LokiAddonDescriptor::GetDescription()
   {
      return description;
   }

   // Accessor for the Node version against which this addon was built
   std::string LokiAddonDescriptor::GetNodeVersion()
   {
      return (std::to_string(NODE_MAJOR_VERSION) + '.' + std::to_string(NODE_MINOR_VERSION) + '.' + std::to_string(NODE_PATCH_VERSION));
   }

   // Accessor for the function list
   std::vector<LokiFunction> LokiAddonDescriptor::GetFunctions()
   {
      return functions;
   }
}