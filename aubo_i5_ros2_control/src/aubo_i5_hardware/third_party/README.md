# AUBO vendor SDK

Place the unmodified, matching AUBO Linux SDK in this directory.  The required
layout is:

```text
third_party/aubo_sdk/
├── include/
└── lib/
    ├── libaubo_sdk.so
    └── cmake/aubo_sdk/aubo_sdkConfig.cmake
```

For the current driver, use the SDK version validated with the reference
driver (`0.24.1-rc.3`, x86_64 for a normal Ubuntu PC).  Do not rename the SDK
root directory: it must be exactly `aubo_sdk`.

The SDK is vendor-provided binary software.  Keep its original license files
with it and do not commit the SDK binaries to a public Git repository unless
the vendor license explicitly permits redistribution.  To move to another
computer, copy this `third_party/aubo_sdk` directory together with the ROS 2
workspace, then rebuild the package.

During installation, CMake copies the SDK shared libraries to
`install/aubo_i5_hardware/lib/aubo_i5_hardware/aubo_sdk` and records a relative
runtime search path in the hardware plugin.  Therefore the installed package
does not require an `/opt/aubo-sdk` directory or a manual `LD_LIBRARY_PATH`.
