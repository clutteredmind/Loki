//
// LokiAddonDescriptorTests.cpp : Defines tests for the LokiAddonBase class
//

#include "gtest/gtest.h"

#include "node_version.h"
#include "LokiAddonDescriptor.hpp"

using namespace Loki;

TEST(LokiAddonDescriptorTests, Instantiation)
{
   LokiAddonDescriptor descriptor;
}

class LokiAddonDescriptorTest : public testing::Test
{
   public:
   // the descriptor to test
   LokiAddonDescriptor descriptor;
   // test values
   const std::string test_name = "Descriptor Name";
   const std::string test_version = "1.2.3";
   const std::string test_description = "Addon description string.";
   const int test_version_number_array [3] = {3, 2, 1};
   static void StubCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {}
};

TEST_F(LokiAddonDescriptorTest, NameShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ(0, descriptor.GetName().length());
}

TEST_F(LokiAddonDescriptorTest, VersionShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ("", descriptor.GetVersion());
}

TEST_F(LokiAddonDescriptorTest, DescriptionShouldBeBlankWhenDescriptorIsCreated)
{
   EXPECT_EQ(0, descriptor.GetDescription().length());
}

TEST_F(LokiAddonDescriptorTest, NodeVersionShouldBePulledFromNodeHeaderFile)
{
   std::string node_version = std::to_string(NODE_MAJOR_VERSION) + '.' + std::to_string(NODE_MINOR_VERSION) + '.' + std::to_string(NODE_PATCH_VERSION);
   EXPECT_EQ(node_version, descriptor.GetNodeVersion());
}

TEST_F(LokiAddonDescriptorTest, FunctionListShouldBeEmptyWhenDescriptorIsCreated)
{
   EXPECT_EQ(0, descriptor.GetFunctions().size());
}

TEST_F(LokiAddonDescriptorTest, SetNameShouldSetTheName)
{
   descriptor.SetName(test_name);
   EXPECT_EQ(test_name, descriptor.GetName());
}

TEST_F(LokiAddonDescriptorTest, SetVersionShouldSetTheVersion)
{
   descriptor.SetVersion(test_version);
   EXPECT_EQ(test_version, descriptor.GetVersion());
}

TEST_F(LokiAddonDescriptorTest, GetVersionStringFromArrayShouldConvertAnArrayToAString)
{
   EXPECT_EQ("3.2.1", LokiAddonDescriptor::GetVersionStringFromArray(test_version_number_array));
}

TEST_F(LokiAddonDescriptorTest, SetDescriptionShouldSetTheDescription)
{
   descriptor.SetDescription(test_description);
   EXPECT_EQ(test_description, descriptor.GetDescription());
}
