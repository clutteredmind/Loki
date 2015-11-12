// 
// Purpose: Declares the class that describes a particular Loki addon
//

#pragma once

#include <node.h>

#include <string>
#include <vector>
#include <utility>

// convenience macros
#define NO_PARAMETERS {}
#define LOKI_PARAMETER(param_type, param_name) {std::make_pair(param_type, param_name)}
#define RETURNS ParameterType::
#define RETURNS_A ParameterType::
#define RETURNS_AN ParameterType::

namespace Loki
{
   // The types of parameters that can be accepted by a Loki addon's function
   enum ParameterType
   {
      UNDEFINED = 0,
      BOOL,
      STRING,
      NUMBER,
      FUNCTION,
      ARRAY,
      OBJECT,
      DATE,
      BUFFER
   };

   // Pairs a parameter type with a name
   struct Parameter
   {
      // The parameter's type and name
      std::pair<ParameterType, std::string> parameter;
   };

   // Represents a single function exposed to JavaScript by a Loki addon
   struct LokiFunction
   {
      LokiFunction(const std::string& name,
         const v8::FunctionCallback callback,
         const std::string& description,
         const std::vector<Parameter> parameters,
         const ParameterType return_type) :
         name(name), callback(callback), description(description), parameters(parameters), return_type(return_type)
      {
      }
      // the name of the function
      std::string name;
      // the v8 callback that will be used to perform the work
      v8::FunctionCallback callback;
      // a description of the function
      std::string description;
      // the function's parameters
      std::vector<Parameter> parameters;
      // the return type of the function
      ParameterType return_type;
   };

   class LokiAddonDescriptor
   {
      public:
      // Constructor
      LokiAddonDescriptor();
      // Destructor
      ~LokiAddonDescriptor() = default;
      // Converts an array of integers to a version number string
      static std::string GetVersionStringFromArray(const int version_numbers [3]);
      // Returns the string representation of a parameter type
      static std::string ConvertParameterTypeToString(const ParameterType parameter_type);
      // Setter for the addon's name
      void SetName(const std::string& name);
      // Setter for the addon's version number
      void SetVersion(const std::string& version);
      // Setter for the addon's description
      void SetDescription(const std::string& description);
      // Adds an exported function description
      bool AddFunction(const LokiFunction new_function);
      // Adds an exported function description
      bool AddFunction(const std::string& name,
         const v8::FunctionCallback callback,
         const std::string& description,
         const std::vector<Parameter> parameters = NO_PARAMETERS,
         const ParameterType return_type = ParameterType::UNDEFINED);
      // Removes a function description by index
      bool RemoveFunction(const unsigned index);
      // Accessor for the addon's name
      std::string GetName();
      // Accessor for the addon's version number
      std::string GetVersion();
      // Accessor for the addon's description
      std::string GetDescription();
      // Accessor for the Node version against which this addon was built
      std::string GetNodeVersion();
      // Accessor for the function list
      std::vector<LokiFunction> GetFunctions();

      private:
      // The addon's name
      std::string name;
      // The addon's version number
      std::string version;
      // The addon's description
      std::string description;
      // The addon's functions
      std::vector<LokiFunction> functions;
   };
}