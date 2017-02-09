//
// LokiModuleBaseTests.cpp : Defines tests for the LokiModuleBase class
//

#include "gtest\gtest.h"

#include "LokiModuleBase.hpp"

using namespace Loki;

TEST (LokiModuleBaseTests, Instantiation)
{
   LokiModuleBase base_class;
}

class LokiModuleBaseTest : public ::testing::Test
{
public:
   class DerivedLokiModuleBase : public LokiModuleBase
   {
   public:
      // accessor for protected SetLastErrorString method
      bool SetLastError (std::string& error_string) { return SetLastErrorString (error_string); }
   };
   DerivedLokiModuleBase test_class;
};

TEST_F (LokiModuleBaseTest, GetLastErrorStringShouldReturnFalseWhenObjectIsFirstInitialized)
{
   std::string error_string;
   EXPECT_FALSE (test_class.GetLastErrorString (error_string));
}

TEST_F (LokiModuleBaseTest, SetLastErrorStringShouldReturnFalseWithEmptyArgument)
{
   EXPECT_FALSE (test_class.SetLastError (std::string ("")));
}

TEST_F (LokiModuleBaseTest, SetLastErrorStringShouldReturnTrueWithANonEmptyStringArgument)
{
   EXPECT_TRUE (test_class.SetLastError (std::string ("Some arbitrary error string.")));
}

TEST_F (LokiModuleBaseTest, GetLastErrorStringShouldReturnTrueAfterErrorStringHasBeenSet)
{
   std::string test_error_string = "Some arbitrary error string.";
   test_class.SetLastError (test_error_string);
   std::string retrieved_error_string;
   EXPECT_TRUE (test_class.GetLastErrorString (retrieved_error_string));
}

TEST_F (LokiModuleBaseTest, GetLastErrorStringShouldReturnTheErrorStringAfterBeingSet)
{
   std::string test_error_string = "Some arbitrary error string.";
   test_class.SetLastError (test_error_string);
   std::string retrieved_error_string;
   test_class.GetLastErrorString (retrieved_error_string);
   EXPECT_EQ (test_error_string, retrieved_error_string);
}
