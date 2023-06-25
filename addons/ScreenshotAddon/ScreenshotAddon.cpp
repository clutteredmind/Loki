/*
 * File: ScreenshotAddon.cpp
 * Created Date: Sunday, June 25th 2023
 * Author: Aaron Hall
 * -----
 * Last Modified: Sunday, 25th June 2023 8:42:25 am
 * Modified By: Aaron Hall
 * -----
 * Copyright (c) 2020 - 2023
 */
#include <napi.h>

#include "jpge.h" // for JPEG compression

#include <Windows.h>

namespace ScreenshotAddon
{
  const int MODULE_VERSION[3]{1 /*major*/, 0 /*minor*/, 0 /*patch*/};

  Napi::String GetAddonVersion(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    return Napi::String::New(env, std::to_string(MODULE_VERSION[0]) +
                                      '.' +
                                      std::to_string(MODULE_VERSION[1]) +
                                      '.' +
                                      std::to_string(MODULE_VERSION[2]));
  }

  Napi::Value CaptureScreen(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();
    Napi::Value result;

    if (!info[0].IsString())
    {
      Napi::TypeError::New(env, "Incorrect arguments. Expected a string path with filename.").ThrowAsJavaScriptException();
      result = env.Null();
    }
    else
    {
      std::string outputPath(info[0].As<Napi::String>());

      // Get display dimensions.
      int screenX = GetSystemMetrics(SM_XVIRTUALSCREEN);
      int screenY = GetSystemMetrics(SM_YVIRTUALSCREEN);
      int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
      int screenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

      // Objects for working with the screen through the Windows API.
      HDC screenDeviceContext = NULL;
      HDC compatibleDeviceContext = NULL;
      HBITMAP bitmapHandle = NULL;
      // Bitmap format info. Used by GetDIBits.
      LPBITMAPINFO bitmapInfo = NULL;
      // Memory buffer for JPEG compression.
      uint8_t *imageData = NULL;

      try
      {
        // Get a device context for the whole screen.
        screenDeviceContext = CreateDC(L"DISPLAY", NULL, NULL, 0);
        if (screenDeviceContext == NULL)
        {
          throw std::exception("CaptureScreen: Unable to get screen device context");
        }

        // Get a memory device context compatible with the DC acquired above.
        compatibleDeviceContext = CreateCompatibleDC(screenDeviceContext);
        if (compatibleDeviceContext == NULL)
        {
          throw std::exception("CaptureScreen: Unable to create compatible device context");
        }

        // Create a bitmap compatible with the device that is associated with the specified device context.
        bitmapHandle = CreateCompatibleBitmap(screenDeviceContext, screenWidth, screenHeight);
        if (bitmapHandle == NULL)
        {
          throw std::exception("CaptureScreen: Unable to create compatible bitmap");
        }

        // Select the bitmap into the device context.
        if (SelectObject(compatibleDeviceContext, bitmapHandle) == NULL)
        {
          throw std::exception("CaptureScreen: Unable to select bitmap object");
        }

        // Copy color data from the screen DC into the compatible DC.
        if (BitBlt(compatibleDeviceContext, 0, 0, screenWidth, screenHeight, screenDeviceContext, screenX, screenY, SRCCOPY) == NULL)
        {
          throw std::exception("CaptureScreen: Unable to capture the screen");
        }

        // Reserve memory for bitmap info (BITMAPINFOHEADER + largest possible palette):
        bitmapInfo = reinterpret_cast<LPBITMAPINFO>(new char[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)]);
        if (bitmapInfo == NULL)
        {
          throw std::exception("CaptureScreen: Failed to allocate memory for bitmap");
        }
        ZeroMemory(&bitmapInfo->bmiHeader, sizeof(BITMAPINFOHEADER));
        bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

        // GetDIBits requires format info about the bitmap.
        // We can have GetDIBits fill a structure with that info if we pass NULL for lpvBits.
        if (!GetDIBits(compatibleDeviceContext, bitmapHandle, 0, screenHeight /* lpvBits */, NULL, bitmapInfo, DIB_RGB_COLORS))
        {
          throw std::exception("CaptureScreen: Unable to get bitmap information");
        }

        // The raw screen buffer. This will contain RGB+alpha information.
        std::vector<uint8_t> rawScreenBuffer;
        rawScreenBuffer.resize(screenWidth * screenHeight * 4); // Width * Pixels * 4 bytes per pixel, RGB + alpha
                                                                    // Now that we have all of the bitmap's information, we pass a pointer to the beginning of
                                                                    // rawScreenBuffer for GetDIBits to fill it in with bitmap data
        if (!GetDIBits(compatibleDeviceContext, bitmapHandle, 0, screenHeight, &rawScreenBuffer[0], bitmapInfo, DIB_RGB_COLORS))
        {
          throw std::exception("CaptureScreen: Unable to copy screen capture bitmap");
        }

        // Calculate the size that the image buffers need to be.
        int bufferSize = screenWidth * screenHeight * 3;
        // Image_data needs to be big enough to hold all the bitmap data.
        imageData = new uint8_t[bufferSize];
        // The final screen buffer. This will omit the alpha channel, so we allocate a bit less space.
        std::vector<uint8_t> finalScreenBuffer;
        finalScreenBuffer.resize(bufferSize);
        // Convert the raw screen buffer to the final buffer.
        for (int line = 0; line < screenHeight; line++)
        {
          for (int column = 0; column < screenWidth; column++)
          {
            int rawPixel = (line * screenWidth) + column;
            int finalPixel = ((screenHeight - line - 1) * screenWidth) + column;
            imageData[finalPixel * 3 + 2] = finalScreenBuffer[finalPixel * 3] = rawScreenBuffer[rawPixel * 4];
            imageData[finalPixel * 3 + 1] = finalScreenBuffer[finalPixel * 3 + 1] = rawScreenBuffer[rawPixel * 4 + 1];
            imageData[finalPixel * 3] = finalScreenBuffer[finalPixel * 3 + 2] = rawScreenBuffer[rawPixel * 4 + 2];
          }
        }
        // compress_image_to_jpeg_file will attempt to write a file to disk at the given path.
        if (!jpge::compress_image_to_jpeg_file(outputPath.c_str(), screenWidth, screenHeight, 3, imageData))
        {
          throw std::exception("CaptureScreen: Unable to save JPEG");
        }
        std::string successMessage("Wrote screenshot to ");
        result = Napi::String::New(env, successMessage + outputPath);
      }
      catch (std::exception &exception)
      {
        Napi::Error::New(env, Napi::String::New(env, exception.what())).ThrowAsJavaScriptException();
        result = Napi::String::New(env, "Screen capture failed.");
      }

      // Clean up memory.
      if (NULL != compatibleDeviceContext)
      {
        DeleteDC(compatibleDeviceContext);
      }
      if (NULL != bitmapHandle)
      {
        DeleteObject(bitmapHandle);
      }
      if (NULL != bitmapInfo)
      {
        delete bitmapInfo;
      }
      if (NULL != imageData)
      {
        delete imageData;
      }
    }

    return result;
  }

  Napi::Object Init(Napi::Env env, Napi::Object exports)
  {
    exports.Set(Napi::String::New(env, "CaptureScreen"), Napi::Function::New(env, CaptureScreen));
    exports.Set(Napi::String::New(env, "GetAddonVersion"), Napi::Function::New(env, GetAddonVersion));
    return exports;
  }

  NODE_API_MODULE(DeviceListAddon, Init)
}