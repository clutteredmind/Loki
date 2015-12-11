// 
// Purpose: Defines the class that describes a particular Loki module
//

#include "LokiModuleDescriptor.hpp"

// the node version header
#include <node_version.h>

#include <algorithm>

using namespace v8;

namespace Loki
{
   // Constructor
   LokiModuleDescriptor::LokiModuleDescriptor() :
      name(), version(), description(), functions()
   {
   }

   // Converts an array of integers to a version number string
   std::string LokiModuleDescriptor::GetVersionStringFromArray(const int version_numbers [3])
   {
      return (std::to_string(version_numbers [0]) + '.' + std::to_string(version_numbers [1]) + '.' + std::to_string(version_numbers [2]));
   }

   // Converts a ParameterType to its string representation
   std::string LokiModuleDescriptor::ConvertParameterTypeToString(const ParameterType parameter_type)
   {
      std::string type_as_string = "undefined";

      switch (parameter_type)
      {
         case ParameterType::BOOLEAN:
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
         case ParameterType::BUFFER:
            type_as_string = "buffer";
            break;
      }

      return type_as_string;
   }

   // Setter for the module's name
   void LokiModuleDescriptor::SetName(const std::string& name)
   {
      this->name = name;
   }

   // setter for the module's display name
   void LokiModuleDescriptor::SetDisplayName (const std::string& display_name)
   {
      this->display_name = display_name;
   }

   // Setter for the module's version number
   void LokiModuleDescriptor::SetVersion(const std::string& version)
   {
      this->version = version;
   }

   // Setter for the module's description
   void LokiModuleDescriptor::SetDescription(const std::string& description)
   {
      this->description = description;
   }

   // Adds an exported function description
   bool LokiModuleDescriptor::AddFunction(const LokiFunction new_function)
   {
      bool result = false;
      // only add the function if it's not already in the list
      auto iterator = std::find_if(functions.begin(), functions.end(), [&new_function] (const LokiFunction& function)->bool { return new_function.name.compare(function.name) == 0; });
      if (iterator == functions.end())
      {
         functions.push_back(new_function);
         result = true;
      }
      return result;
   }

   // Adds an exported function description
   bool LokiModuleDescriptor::AddFunction(const std::string& name,
      const FunctionCallback callback,
      const std::string& description,
      const std::vector<Parameter> parameters,
      const ParameterType return_type)
   {
      LokiFunction function(name, callback, description, parameters, return_type);
      return AddFunction(function);
   }

   // Removes a function description by index
   bool LokiModuleDescriptor::RemoveFunction(const unsigned index)
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

   // Accessor for the module's name
   std::string LokiModuleDescriptor::GetName()
   {
      return name;
   }

   // Accessor for the module's display name
   std::string LokiModuleDescriptor::GetDisplayName ()
   {
      return display_name;
   }

   // Accessor for the module's version number
   std::string LokiModuleDescriptor::GetVersion()
   {
      return version;
   }

   // Accessor for the module's description
   std::string LokiModuleDescriptor::GetDescription()
   {
      return description;
   }

   // Accessor for the Node version against which this module was built
   std::string LokiModuleDescriptor::GetNodeVersion()
   {
      return (std::to_string(NODE_MAJOR_VERSION) + '.' + std::to_string(NODE_MINOR_VERSION) + '.' + std::to_string(NODE_PATCH_VERSION));
   }

   // Accessor for the function list
   std::vector<LokiFunction> LokiModuleDescriptor::GetFunctions()
   {
      return functions;
   }

   // Validates the type and number of parameters passed to a function
   bool LokiModuleDescriptor::ValidateParameters(const FunctionCallback callback, const FunctionCallbackInfo<Value>& args, std::string& error_message)
   {
      auto parameters_are_valid = true;

      // find the correct function
      auto function_iterator = std::find_if(functions.begin(), functions.end(), [&callback] (const LokiFunction& function)->bool { return callback == function.callback; });
      if (function_iterator == functions.end())
      {
         error_message = "Could not validate parameters. Callback not found.";
         parameters_are_valid = false;
      }
      else
      {
         // verify number of parameters
         if (args.Length() != function_iterator->parameters.size())
         {
            error_message = function_iterator->name
               + ": Expected "
               + std::to_string(function_iterator->parameters.size())
               + " parameter"
               + (function_iterator->parameters.size() != 1 ? "s" : "")
               + " and got "
               + std::to_string(args.Length())
               + ".";
            parameters_are_valid = false;
         }
         else
         {
            // verify types of parameters
            int arg_index = 0;
            for (auto iterator = function_iterator->parameters.begin(); iterator != function_iterator->parameters.end() && parameters_are_valid; iterator++)
            {
               auto expected_parameter_type = iterator->parameter.first;
               std::string parameter_article = "";
               switch (expected_parameter_type)
               {
                  case ParameterType::UNDEFINED:
                     if (!args[arg_index]->IsUndefined())
                     {
                        parameter_article = "";
                        parameters_are_valid = false;
                     }
                     break;
                  case ParameterType::BOOLEAN:
                     if (!args [arg_index]->IsBoolean())
                     {
                        parameter_article = "a ";
                        parameters_are_valid = false;
                     }
                     break;
                  case ParameterType::STRING:
                     if (!args [arg_index]->IsString())
                     {
                        parameter_article = "a ";
                        parameters_are_valid = false;
                     }
                     break;
                  case ParameterType::NUMBER:
                     if (!args [arg_index]->IsNumber())
                     {
                        parameter_article = "a ";
                        parameters_are_valid = false;
                     }
                     break;
                  case ParameterType::FUNCTION:
                     if (!args [arg_index]->IsFunction())
                     {
                        parameter_article = "a ";
                        parameters_are_valid = false;
                     }
                     break;
                  case ParameterType::ARRAY:
                     if (!args [arg_index]->IsArray())
                     {
                        parameter_article = "an ";
                        parameters_are_valid = false;
                     }
                     break;
                  case ParameterType::OBJECT:
                     if (!args [arg_index]->IsObject())
                     {
                        parameter_article = "an ";
                        parameters_are_valid = false;
                     }
                     break;
                  case ParameterType::DATE:
                     if (!args [arg_index]->IsDate())
                     {
                        parameter_article = "a ";
                        parameters_are_valid = false;
                     }
                     break;
                  case ParameterType::BUFFER:
                     if (!args [arg_index]->IsArrayBuffer())
                     {
                        parameter_article = "a ";
                        parameters_are_valid = false;
                     }
                     break;
               }
               if (!parameters_are_valid)
               {
                  // construct error message
                  auto parameter_name = iterator->parameter.second;
                  error_message = function_iterator->name
                     + ": Expected parameter "
                     + std::to_string(arg_index)
                     + " ("
                     + parameter_name
                     + ") to be "
                     + parameter_article
                     + LokiModuleDescriptor::ConvertParameterTypeToString(expected_parameter_type)
                     + ".";
               }
               arg_index++;
            }
         }
      }

      return parameters_are_valid;
   }
}