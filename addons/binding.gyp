{
    "targets": [
        {
            "target_name": "DeviceListAddon",
            "include_dirs": ["<!(node -p \"require('node-addon-api').include_dir\")"],
            "sources": ["DeviceListAddon/DeviceListAddon.cpp"],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
            "configurations": {
                "Release": {
                    "msvs_settings": {
                        "VCCLCompilerTool": {
                            "AdditionalOptions": ["/EHsc", "/INCREMENTAL"],
                            # "RuntimeLibrary": 0, # MultiThreaded (/MT)}
                            # "RuntimeLibrary": 1, # MultiThreadedDebug (/MTd)}
                            "RuntimeLibrary": 2,  # MultiThreadedDLL (/MD)}
                            # "RuntimeLibrary": 3, # MultiThreadedDebugDLL (/MDd)}
                        }
                    }
                }
            },
        },
        {
            "target_name": "ProcessListAddon",
            "include_dirs": ["<!(node -p \"require('node-addon-api').include_dir\")"],
            "sources": ["ProcessListAddon/ProcessListAddon.cpp"],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS", "_UNICODE", "UNICODE"],
            "configurations": {
                "Release": {
                    "msvs_settings": {
                        "VCCLCompilerTool": {
                            "AdditionalOptions": ["/EHsc", "/INCREMENTAL"],
                            # "RuntimeLibrary": 0, # MultiThreaded (/MT)}
                            # "RuntimeLibrary": 1, # MultiThreadedDebug (/MTd)}
                            "RuntimeLibrary": 2,  # MultiThreadedDLL (/MD)}
                            # "RuntimeLibrary": 3, # MultiThreadedDebugDLL (/MDd)}
                        }
                    }
                }
            },
        },
        {
            "target_name": "ScreenshotAddon",
            "include_dirs": ["<!(node -p \"require('node-addon-api').include_dir\")"],
            "sources": [
                "ScreenshotAddon/ScreenshotAddon.cpp",
                "ScreenshotAddon/jpge.cpp",
            ],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS", "_UNICODE", "UNICODE"],
            "configurations": {
                "Release": {
                    "msvs_settings": {
                        "VCCLCompilerTool": {
                            "AdditionalOptions": ["/EHsc", "/INCREMENTAL"],
                            # "RuntimeLibrary": 0, # MultiThreaded (/MT)}
                            # "RuntimeLibrary": 1, # MultiThreadedDebug (/MTd)}
                            "RuntimeLibrary": 2,  # MultiThreadedDLL (/MD)}
                            # "RuntimeLibrary": 3, # MultiThreadedDebugDLL (/MDd)}
                        }
                    }
                }
            },
        },
        {
            "target_name": "SystemInformationAddon",
            "include_dirs": ["<!(node -p \"require('node-addon-api').include_dir\")"],
            "sources": ["SystemInformationAddon/SystemInformationAddon.cpp"],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
            "configurations": {
                "Release": {
                    "msvs_settings": {
                        "VCCLCompilerTool": {
                            "AdditionalOptions": ["/EHsc", "/INCREMENTAL"],
                            # "RuntimeLibrary": 0, # MultiThreaded (/MT)}
                            # "RuntimeLibrary": 1, # MultiThreadedDebug (/MTd)}
                            "RuntimeLibrary": 2,  # MultiThreadedDLL (/MD)}
                            # "RuntimeLibrary": 3, # MultiThreadedDebugDLL (/MDd)}
                        }
                    }
                }
            },
        },
    ]
}
