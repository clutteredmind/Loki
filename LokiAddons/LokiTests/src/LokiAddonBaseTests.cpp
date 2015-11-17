//
// LokiAddonBaseTests.cpp : Defines tests for the LokiAddonBase class
//

#include "gtest\gtest.h"

#include "LokiAddonBase.hpp"

using namespace Loki;

TEST(LokiAddonBaseTests, Instantiation)
{
   LokiAddonBase base_class;
}

class LokiAddonBaseTest : public ::testing::Test
{
   public:
   class DerivedLokiAddonBase : public LokiAddonBase
   {
      public:
      // accessor for protected SetLastErrorString method
      bool SetLastError(std::string& error_string) { return SetLastErrorString(error_string); }
   };
   DerivedLokiAddonBase test_class;
};

TEST_F(LokiAddonBaseTest, GetLastErrorStringShouldReturnFalseWhenObjectIsFirstInitialized)
{
   std::string error_string;
   EXPECT_FALSE(test_class.GetLastErrorString(error_string));
}

TEST_F(LokiAddonBaseTest, SetLastErrorStringShouldReturnFalseWithEmptyArgument)
{
   EXPECT_FALSE(test_class.SetLastError(std::string("")));
}

TEST_F(LokiAddonBaseTest, SetLastErrorStringShouldReturnTrueWithANonEmptyStringArgument)
{
   EXPECT_TRUE(test_class.SetLastError(std::string("Some arbitrary error string.")));
}

TEST_F(LokiAddonBaseTest, GetLastErrorStringShouldReturnTrueAfterErrorStringHasBeenSet)
{
   std::string test_error_string = "Some arbitrary error string.";
   test_class.SetLastError(test_error_string);
   std::string retrieved_error_string;
   EXPECT_TRUE(test_class.GetLastErrorString(retrieved_error_string));
}

TEST_F(LokiAddonBaseTest, GetLastErrorStringShouldReturnTheErrorStringAfterBeingSet)
{
   std::string test_error_string = "Some arbitrary error string.";
   test_class.SetLastError(test_error_string);
   std::string retrieved_error_string;
   test_class.GetLastErrorString(retrieved_error_string);
   EXPECT_EQ(test_error_string, retrieved_error_string);
}
