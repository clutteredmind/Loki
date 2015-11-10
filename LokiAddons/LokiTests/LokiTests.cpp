// LokiTests.cpp : Defines the entry point for the console application.
//

#include "gtest/gtest.h"
#include "LokiAddonBase.hpp"
#include "LokiAddonDescriptor.hpp"
#include "LokiAddon.hpp"
#include "DeviceListAddon.hpp"
#include "ProcessListAddon.hpp"
#include "ScreenshotAddon.hpp"

using namespace Loki;

TEST(DeviceListAddonTests, instantiation)
{
   DeviceListAddon device_list_addon;
}

TEST(ProcessListAddonTests, instantiation)
{
   ProcessListAddon process_list_addon;
}

TEST(ScreenshotAddonTests, instantiation)
{
   ScreenshotAddon screenshot_addon;
}

