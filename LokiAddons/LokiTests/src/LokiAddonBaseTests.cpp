//
// LokiAddonBaseTests.cpp : Defines tests for the LokiAddonBase class
//

#include "gtest/gtest.h"
#include "LokiAddonBase.hpp"

using namespace Loki;

TEST(LokiAddonBaseTests, Instantiation)
{
   LokiAddonBase base_class;
}

// test fixture that inherits from both gtest's Test and also from LokiAddonBase,
// so we gain access to the protected SetLastErrorString method
class LokiAddonBaseTest : public ::testing::Test, public LokiAddonBase
{
   public:
   // an instance of the base class to use for testing
   LokiAddonBase base_class;
};

TEST_F(LokiAddonBaseTest, GetLastErrorStringShouldReturnFalseWhenObjectIsFirstInitialized)
{
   std::string error_string;
   EXPECT_FALSE(base_class.GetLastErrorString(error_string));
}

TEST_F(LokiAddonBaseTest, SetLastErrorStringShouldReturnFalseWithEmptyArgument)
{
   EXPECT_FALSE(this->SetLastErrorString(std::string("")));
}

TEST_F(LokiAddonBaseTest, SetLastErrorStringShouldReturnTrueWithANonEmptyStringArgument)
{
   EXPECT_TRUE(this->SetLastErrorString(std::string("Some arbitrary error string.")));
}

TEST_F(LokiAddonBaseTest, GetLastErrorStringShouldReturnTrueAfterErrorStringHasBeenSet)
{
   std::string test_error_string = "Some arbitrary error string.";
   this->SetLastErrorString(test_error_string);
   std::string retrieved_error_string;
   EXPECT_TRUE(this->GetLastErrorString(retrieved_error_string));
}

TEST_F(LokiAddonBaseTest, GetLastErrorStringShouldReturnTheErrorStringAfterBeingSet)
{
   std::string test_error_string = "Some arbitrary error string.";
   this->SetLastErrorString(test_error_string);
   std::string retrieved_error_string;
   this->GetLastErrorString(retrieved_error_string);
   EXPECT_EQ(test_error_string, retrieved_error_string);
}
