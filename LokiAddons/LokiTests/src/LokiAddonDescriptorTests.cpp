//
// LokiAddonDescriptorTests.cpp : Defines tests for the LokiAddonBase class
//

#include "gtest/gtest.h"
#include "LokiAddonDescriptor.hpp"

using namespace Loki;

// test LokiFunction
class LokiFunctionTest : public testing::Test
{
   public:
   const std::string function_name = "functionName";
   static void StubCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {}
   const std::string function_description = "Function description.";
   const std::vector<Parameter> parameters = {{std::make_pair(ParameterType::STRING, "some_string")}};
   const ParameterType return_type = ParameterType::UNDEFINED;
};

TEST_F(LokiFunctionTest, NameShouldBeSetAtCreation)
{
   LokiFunction function(function_name, StubCallback, function_description, parameters, return_type);
   // compare names
   EXPECT_EQ(function_name, function.name);
}

TEST_F(LokiFunctionTest, DescriptionShouldBeSetAtCreation)
{
   LokiFunction function(function_name, StubCallback, function_description, parameters, return_type);
   // compare descriptions
   EXPECT_EQ(function_description, function.description);
}

TEST_F(LokiFunctionTest, ParameterListShouldBeSetAtCreation)
{
   LokiFunction function(function_name, StubCallback, function_description, parameters, return_type);
   // compare parameter list
   EXPECT_EQ(parameters.size(), function.parameters.size());
   EXPECT_EQ(parameters.front().parameter.first, function.parameters.front().parameter.first);
   EXPECT_EQ(parameters.front().parameter.second, function.parameters.front().parameter.second);
}

TEST_F(LokiFunctionTest, ReturnTypeShouldBeSetAtCreation)
{
   LokiFunction function(function_name, StubCallback, function_description, parameters, return_type);
   // compare return types
   EXPECT_EQ(return_type, function.return_type);
}

// test LokiAddonDescriptor
TEST(LokiAddonDescriptorTests, Instantiation)
{
   LokiAddonDescriptor descriptor;
}

class LokiAddonDescriptorTest : public testing::Test
{
   public:
   LokiAddonDescriptor descriptor;
};

TEST_F(LokiAddonDescriptorTest, NameShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ(0, descriptor.GetName().length());
}

TEST_F(LokiAddonDescriptorTest, DescriptionShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ(0, descriptor.GetDescription().length());
}
