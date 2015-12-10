//
// LokiModuleDescriptorTests.cpp : Defines tests for the LokiModuleBase class
//

#include "gtest\gtest.h"

#include "node_version.h"

#include "LokiModuleDescriptor.hpp"

using namespace Loki;

TEST(LokiModuleDescriptorTests, Instantiation)
{
   LokiModuleDescriptor descriptor;
}

class LokiModuleDescriptorTest : public testing::Test
{
   public:
   // the descriptor to test
   LokiModuleDescriptor descriptor;
   // stub module values
   const std::string module_name = "Descriptor Name";
   const std::string module_version = "1.2.3";
   const std::string module_description = "Module description string.";
   const int module_version_number_array [3] = {3, 2, 1};
   // stub function values
   const std::string function_name = "stubCallback";
   const std::string function_description = "A description of the stub function.";
   const ParameterType function_first_parameter_type = ParameterType::STRING;
   const std::string function_first_parameter_name = "a_string";
   const ParameterType function_return_type = ParameterType::BOOLEAN;
   static void StubCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {}
   static void StubCallbackTwo(const v8::FunctionCallbackInfo<v8::Value>& args) {}
   static void StubCallbackThree(const v8::FunctionCallbackInfo<v8::Value>& args) {}
};

TEST_F(LokiModuleDescriptorTest, NameShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ(0, descriptor.GetName().length());
}

TEST_F(LokiModuleDescriptorTest, VersionShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ("", descriptor.GetVersion());
}

TEST_F(LokiModuleDescriptorTest, DescriptionShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ(0, descriptor.GetDescription().length());
}

TEST_F(LokiModuleDescriptorTest, NodeVersionShouldBePulledFromNodeHeaderFile)
{
   std::string node_version = std::to_string(NODE_MAJOR_VERSION) + '.' + std::to_string(NODE_MINOR_VERSION) + '.' + std::to_string(NODE_PATCH_VERSION);
   EXPECT_EQ(node_version, descriptor.GetNodeVersion());
}

TEST_F(LokiModuleDescriptorTest, FunctionListShouldBeEmptyWhenDescriptorIsCreated)
{
   EXPECT_EQ(0, descriptor.GetFunctions().size());
}

TEST_F(LokiModuleDescriptorTest, SetNameShouldSetTheName)
{
   descriptor.SetName(module_name);
   EXPECT_EQ(module_name, descriptor.GetName());
}

TEST_F(LokiModuleDescriptorTest, SetVersionShouldSetTheVersion)
{
   descriptor.SetVersion(module_version);
   EXPECT_EQ(module_version, descriptor.GetVersion());
}

TEST_F(LokiModuleDescriptorTest, GetVersionStringFromArrayShouldConvertAnArrayToAString)
{
   EXPECT_EQ("3.2.1", LokiModuleDescriptor::GetVersionStringFromArray(module_version_number_array));
}

TEST_F(LokiModuleDescriptorTest, SetDescriptionShouldSetTheDescription)
{
   descriptor.SetDescription(module_description);
   EXPECT_EQ(module_description, descriptor.GetDescription());
}

TEST_F(LokiModuleDescriptorTest, AddFunctionWithAConstructedFunctionShouldAddAFunction)
{
   LokiFunction function(function_name, StubCallback, function_description, {LOKI_PARAMETER(function_first_parameter_type, function_first_parameter_name)}, function_return_type);
   ASSERT_EQ(0, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.AddFunction(function));
   ASSERT_EQ(1, descriptor.GetFunctions().size());
   EXPECT_EQ(function_name, descriptor.GetFunctions().front().name);
   EXPECT_EQ(&StubCallback, descriptor.GetFunctions().front().callback);
   EXPECT_EQ(function_description, descriptor.GetFunctions().front().description);
   EXPECT_EQ(function_first_parameter_type, descriptor.GetFunctions().front().parameters.front().parameter.first);
   EXPECT_EQ(function_first_parameter_name, descriptor.GetFunctions().front().parameters.front().parameter.second);
   EXPECT_EQ(function_return_type, descriptor.GetFunctions().front().return_type);
}

TEST_F(LokiModuleDescriptorTest, AddFunctionWithParametersShouldAddAFunction)
{
   ASSERT_EQ(0, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.AddFunction(function_name, StubCallback, function_description, {LOKI_PARAMETER(function_first_parameter_type, function_first_parameter_name)}, function_return_type));
   ASSERT_EQ(1, descriptor.GetFunctions().size());
   EXPECT_EQ(function_name, descriptor.GetFunctions().front().name);
   EXPECT_EQ(&StubCallback, descriptor.GetFunctions().front().callback);
   EXPECT_EQ(function_description, descriptor.GetFunctions().front().description);
   EXPECT_EQ(function_first_parameter_type, descriptor.GetFunctions().front().parameters.front().parameter.first);
   EXPECT_EQ(function_first_parameter_name, descriptor.GetFunctions().front().parameters.front().parameter.second);
   EXPECT_EQ(function_return_type, descriptor.GetFunctions().front().return_type);
}

TEST_F(LokiModuleDescriptorTest, AddingSameFunctionTwiceShouldFail)
{
   ASSERT_EQ(0, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.AddFunction(function_name, StubCallback, function_description, {LOKI_PARAMETER(function_first_parameter_type, function_first_parameter_name)}, function_return_type));
   ASSERT_EQ(1, descriptor.GetFunctions().size());
   EXPECT_FALSE(descriptor.AddFunction(function_name, StubCallback, function_description, {LOKI_PARAMETER(function_first_parameter_type, function_first_parameter_name)}, function_return_type));
   ASSERT_EQ(1, descriptor.GetFunctions().size());
}

TEST_F(LokiModuleDescriptorTest, AddingFunctionWithNoParametersArgumentShouldAddTheDefault)
{
   ASSERT_EQ(0, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.AddFunction(function_name, StubCallback, function_description));
   ASSERT_EQ(1, descriptor.GetFunctions().size());
   // should have an empty parameter list
   EXPECT_EQ(0, descriptor.GetFunctions().front().parameters.size());
}

TEST_F(LokiModuleDescriptorTest, AddingFunctionWithNoReturnTypeArgumentShouldDefaultToUndefined)
{
   ASSERT_EQ(0, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.AddFunction(function_name, StubCallback, function_description));
   ASSERT_EQ(1, descriptor.GetFunctions().size());
   ASSERT_EQ(ParameterType::UNDEFINED, descriptor.GetFunctions().front().return_type);
}

TEST_F(LokiModuleDescriptorTest, RemoveFunctionOnAnEmptyListShouldReturnFalse)
{
   ASSERT_EQ(0, descriptor.GetFunctions().size());
   EXPECT_FALSE(descriptor.RemoveFunction(0));
}

TEST_F(LokiModuleDescriptorTest, RemoveFunctionShouldRemoveAFunction)
{
   ASSERT_EQ(0, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.AddFunction(function_name, StubCallback, function_description));
   ASSERT_EQ(1, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.RemoveFunction(0));
   ASSERT_EQ(0, descriptor.GetFunctions().size());
}

TEST_F(LokiModuleDescriptorTest, RemoveFunctionShouldRemoveAFunctionAtSpecifiedIndex)
{
   ASSERT_EQ(0, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.AddFunction("stubCallback", StubCallback, function_description));
   EXPECT_TRUE(descriptor.AddFunction("stubCallbackTwo", StubCallbackTwo, function_description));
   EXPECT_TRUE(descriptor.AddFunction("stubCallbackThree", StubCallbackThree, function_description));
   ASSERT_EQ(3, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.RemoveFunction(1));
   EXPECT_EQ("stubCallback", descriptor.GetFunctions().front().name);
   EXPECT_EQ("stubCallbackThree", (descriptor.GetFunctions().begin() + 1)->name);
}

TEST_F(LokiModuleDescriptorTest, RemoveFunctionAtAnInvalidIndexShouldReturnFalse)
{
   ASSERT_EQ(0, descriptor.GetFunctions().size());
   EXPECT_TRUE(descriptor.AddFunction(function_name, StubCallback, function_description));
   ASSERT_EQ(1, descriptor.GetFunctions().size());
   EXPECT_FALSE(descriptor.RemoveFunction(15));
}
