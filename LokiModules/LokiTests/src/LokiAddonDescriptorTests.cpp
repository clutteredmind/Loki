//
// LokiModuleDescriptorTests.cpp : Defines tests for the LokiModuleBase class
//

#include "gtest\gtest.h"

#include "node_version.h"

#include "LokiModuleDescriptor.hpp"

using namespace Loki;

TEST (LokiModuleDescriptorTests, Instantiation)
{
   LokiModuleDescriptor descriptor;
}

class LokiModuleDescriptorTest : public testing::Test
{
public:
   // the descriptor to test
   LokiModuleDescriptor descriptor;
   // stub module values
   const std::string MODULE_NAME = "DescriptorName";
   const std::string MODULE_DISPLAY_NAME = "Descriptor Name";
   const std::string MODULE_VERSION = "1.2.3";
   const std::string MODULE_DESCRIPTION = "Module description string.";
   const int MODULE_VERSION_NUMBER_ARRAY[3] = { 3, 2, 1 };
   // stub function values
   const std::string FUNCTION_NAME = "stubCallback";
   const std::string FUNCTION_DESCRIPTION = "A description of the stub function.";
   const ParameterType FUNCTION_FIRST_PARAMETER_TYPE = ParameterType::STRING;
   const std::string FUNCTION_FIRST_PARAMETER_NAME = "a_string";
   const ParameterType FUNCTION_RETURN_TYPE = ParameterType::BOOLEAN;
   static void StubCallback (const v8::FunctionCallbackInfo<v8::Value>& args) {}
   static void StubCallbackTwo (const v8::FunctionCallbackInfo<v8::Value>& args) {}
   static void StubCallbackThree (const v8::FunctionCallbackInfo<v8::Value>& args) {}
};

TEST_F (LokiModuleDescriptorTest, NameShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ (0, descriptor.GetName ().length ());
}

TEST_F (LokiModuleDescriptorTest, VersionShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ ("", descriptor.GetVersion ());
}

TEST_F (LokiModuleDescriptorTest, DescriptionShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ (0, descriptor.GetDescription ().length ());
}

TEST_F (LokiModuleDescriptorTest, NodeVersionShouldBePulledFromNodeHeaderFile)
{
   std::string node_version = std::to_string (NODE_MAJOR_VERSION) + '.' + std::to_string (NODE_MINOR_VERSION) + '.' + std::to_string (NODE_PATCH_VERSION);
   EXPECT_EQ (node_version, descriptor.GetNodeVersion ());
}

TEST_F (LokiModuleDescriptorTest, FunctionListShouldBeEmptyWhenDescriptorIsCreated)
{
   EXPECT_EQ (0, descriptor.GetFunctions ().size ());
}

TEST_F (LokiModuleDescriptorTest, SetNameShouldSetTheName)
{
   descriptor.SetName (MODULE_NAME);
   EXPECT_EQ (MODULE_NAME, descriptor.GetName ());
}

TEST_F (LokiModuleDescriptorTest, SetDisplayNameShouldSetTheDisplayName)
{
   descriptor.SetDisplayName (MODULE_DISPLAY_NAME);
   EXPECT_EQ (MODULE_DISPLAY_NAME, descriptor.GetDisplayName ());
}

TEST_F (LokiModuleDescriptorTest, SetVersionShouldSetTheVersion)
{
   descriptor.SetVersion (MODULE_VERSION);
   EXPECT_EQ (MODULE_VERSION, descriptor.GetVersion ());
}

TEST_F (LokiModuleDescriptorTest, GetVersionStringFromArrayShouldConvertAnArrayToAString)
{
   EXPECT_EQ ("3.2.1", LokiModuleDescriptor::GetVersionStringFromArray (MODULE_VERSION_NUMBER_ARRAY));
}

TEST_F (LokiModuleDescriptorTest, SetDescriptionShouldSetTheDescription)
{
   descriptor.SetDescription (MODULE_DESCRIPTION);
   EXPECT_EQ (MODULE_DESCRIPTION, descriptor.GetDescription ());
}

TEST_F (LokiModuleDescriptorTest, SetMetadataShouldSetAllMetadata)
{
   descriptor.SetMetadata (MODULE_NAME, MODULE_DISPLAY_NAME, MODULE_VERSION, MODULE_DESCRIPTION);
   EXPECT_EQ (MODULE_NAME, descriptor.GetName ());
   EXPECT_EQ (MODULE_DISPLAY_NAME, descriptor.GetDisplayName ());
   EXPECT_EQ (MODULE_VERSION, descriptor.GetVersion ());
   EXPECT_EQ (MODULE_DESCRIPTION, descriptor.GetDescription ());
}

TEST_F (LokiModuleDescriptorTest, AddFunctionWithAConstructedFunctionShouldAddAFunction)
{
   LokiFunction function (FUNCTION_NAME, StubCallback, FUNCTION_DESCRIPTION, { LOKI_PARAMETER (FUNCTION_FIRST_PARAMETER_TYPE, FUNCTION_FIRST_PARAMETER_NAME) }, FUNCTION_RETURN_TYPE);
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.AddFunction (function));
   ASSERT_EQ (1, descriptor.GetFunctions ().size ());
   EXPECT_EQ (FUNCTION_NAME, descriptor.GetFunctions ().front ().name);
   EXPECT_EQ (&StubCallback, descriptor.GetFunctions ().front ().callback);
   EXPECT_EQ (FUNCTION_DESCRIPTION, descriptor.GetFunctions ().front ().description);
   EXPECT_EQ (FUNCTION_FIRST_PARAMETER_TYPE, descriptor.GetFunctions ().front ().parameters.front ().parameter.first);
   EXPECT_EQ (FUNCTION_FIRST_PARAMETER_NAME, descriptor.GetFunctions ().front ().parameters.front ().parameter.second);
   EXPECT_EQ (FUNCTION_RETURN_TYPE, descriptor.GetFunctions ().front ().return_type);
}

TEST_F (LokiModuleDescriptorTest, AddFunctionWithParametersShouldAddAFunction)
{
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.AddFunction (FUNCTION_NAME, StubCallback, FUNCTION_DESCRIPTION, { LOKI_PARAMETER (FUNCTION_FIRST_PARAMETER_TYPE, FUNCTION_FIRST_PARAMETER_NAME) }, FUNCTION_RETURN_TYPE));
   ASSERT_EQ (1, descriptor.GetFunctions ().size ());
   EXPECT_EQ (FUNCTION_NAME, descriptor.GetFunctions ().front ().name);
   EXPECT_EQ (&StubCallback, descriptor.GetFunctions ().front ().callback);
   EXPECT_EQ (FUNCTION_DESCRIPTION, descriptor.GetFunctions ().front ().description);
   EXPECT_EQ (FUNCTION_FIRST_PARAMETER_TYPE, descriptor.GetFunctions ().front ().parameters.front ().parameter.first);
   EXPECT_EQ (FUNCTION_FIRST_PARAMETER_NAME, descriptor.GetFunctions ().front ().parameters.front ().parameter.second);
   EXPECT_EQ (FUNCTION_RETURN_TYPE, descriptor.GetFunctions ().front ().return_type);
}

TEST_F (LokiModuleDescriptorTest, AddingSameFunctionTwiceShouldFail)
{
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.AddFunction (FUNCTION_NAME, StubCallback, FUNCTION_DESCRIPTION, { LOKI_PARAMETER (FUNCTION_FIRST_PARAMETER_TYPE, FUNCTION_FIRST_PARAMETER_NAME) }, FUNCTION_RETURN_TYPE));
   ASSERT_EQ (1, descriptor.GetFunctions ().size ());
   EXPECT_FALSE (descriptor.AddFunction (FUNCTION_NAME, StubCallback, FUNCTION_DESCRIPTION, { LOKI_PARAMETER (FUNCTION_FIRST_PARAMETER_TYPE, FUNCTION_FIRST_PARAMETER_NAME) }, FUNCTION_RETURN_TYPE));
   ASSERT_EQ (1, descriptor.GetFunctions ().size ());
}

TEST_F (LokiModuleDescriptorTest, AddingFunctionWithNoParametersArgumentShouldAddTheDefault)
{
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.AddFunction (FUNCTION_NAME, StubCallback, FUNCTION_DESCRIPTION));
   ASSERT_EQ (1, descriptor.GetFunctions ().size ());
   // should have an empty parameter list
   EXPECT_EQ (0, descriptor.GetFunctions ().front ().parameters.size ());
}

TEST_F (LokiModuleDescriptorTest, AddingFunctionWithNoReturnTypeArgumentShouldDefaultToUndefined)
{
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.AddFunction (FUNCTION_NAME, StubCallback, FUNCTION_DESCRIPTION));
   ASSERT_EQ (1, descriptor.GetFunctions ().size ());
   ASSERT_EQ (ParameterType::UNDEFINED, descriptor.GetFunctions ().front ().return_type);
}

TEST_F (LokiModuleDescriptorTest, RemoveFunctionOnAnEmptyListShouldReturnFalse)
{
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
   EXPECT_FALSE (descriptor.RemoveFunction (0));
}

TEST_F (LokiModuleDescriptorTest, RemoveFunctionShouldRemoveAFunction)
{
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.AddFunction (FUNCTION_NAME, StubCallback, FUNCTION_DESCRIPTION));
   ASSERT_EQ (1, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.RemoveFunction (0));
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
}

TEST_F (LokiModuleDescriptorTest, RemoveFunctionShouldRemoveAFunctionAtSpecifiedIndex)
{
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.AddFunction ("stubCallback", StubCallback, FUNCTION_DESCRIPTION));
   EXPECT_TRUE (descriptor.AddFunction ("stubCallbackTwo", StubCallbackTwo, FUNCTION_DESCRIPTION));
   EXPECT_TRUE (descriptor.AddFunction ("stubCallbackThree", StubCallbackThree, FUNCTION_DESCRIPTION));
   ASSERT_EQ (3, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.RemoveFunction (1));
   EXPECT_EQ ("stubCallback", descriptor.GetFunctions ().front ().name);
   EXPECT_EQ ("stubCallbackThree", (descriptor.GetFunctions ().begin () + 1)->name);
}

TEST_F (LokiModuleDescriptorTest, RemoveFunctionAtAnInvalidIndexShouldReturnFalse)
{
   ASSERT_EQ (0, descriptor.GetFunctions ().size ());
   EXPECT_TRUE (descriptor.AddFunction (FUNCTION_NAME, StubCallback, FUNCTION_DESCRIPTION));
   ASSERT_EQ (1, descriptor.GetFunctions ().size ());
   EXPECT_FALSE (descriptor.RemoveFunction (15));
}
