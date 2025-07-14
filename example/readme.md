# ADE9178 Example Build Instructions

## Overview

This repository provides example applications demonstrating how to interact with the ADE9178 energy metering IC using MetIC Service APIs. There are two main examples:

### Simple Example

A minimal application showing how to use MetIC service APIs to communciate with the ADE9178.  
See the [Simple Example Readme](simple_example/readme.md) for details.

### CLI Interface

A command line interface application that communicates with the ADE9178 and collects samples every 20 ms.  
See the [CLI Example Readme](project/readme.md) for details.

## Building the Examples

A `CMakeLists.txt` is provided for building. Ensure the following tools are installed and available in your `PATH`:

- [CMake](https://cmake.org/download) (version 3.22 or newer)
- `make` (e.g., via [MinGW](https://sourceforge.net/projects/mingw/) on Windows)
- [MaximSDK GitHub](https://github.com/analogdevicesinc/msdk/tree/v2024_10)  (v2024_10)

### Building from the Command Line

1. Open a terminal in the directory containing `CMakeLists.txt`.
2. Configure and build the project (output will be in `build/Release`):

    ```sh
    cmake -S . -B build/Release -G "MinGW Makefiles" -DSDK_PATH=/path/to/sdk
    cmake --build build/Release
    ```

    You can specify additional SDK paths using the `-D` option. Refer to the `CMakeLists.txt` files for available variables.

## Flashing the Hex File

You can flash the generated `.hex` files to the MAX32670 board using OpenOCD. One way to get the OpenOCD is from [MaximSDK Installer](https://analogdevicesinc.github.io/msdk//USERGUIDE/#installation).

1. **Get the Debug Adapter Unique ID:**
   - Connect the board to your PC.
   - Open the DAPLINK drive in File Explorer.
   - Open `DETAILS.TXT` and copy the Unique ID.

2. **Flash the Hex File:**
   - Open a terminal or command prompt.
   - Run the following command, replacing `<YOUR_UNIQUE_ID>` and the hex file path:

     ```sh
     "C:\MaximSDK\Tools\OpenOCD\openocd.exe" -s "C:\MaximSDK\Tools\OpenOCD\scripts" -f interface\cmsis-dap.cfg -f target\max32670.cfg -c "cmsis_dap_serial <YOUR_UNIQUE_ID>; program \"C:/Example/ade9178_example.hex\" reset exit"
     ```

## VS Code Workspace

A pre-configured VS Code workspace is included for easy building and debugging.

1. The default CMake preset loads automatically when opening the workspace.
2. If not, open the command palette, search for `CMake: Select Configure Preset`, and choose the appropriate preset.
3. Use the command palette to run `Tasks: Run Task` and select a build, clean, or flash task.

### Debugging

1. Open the "Run and Debug" sidebar in VS Code.
2. Click "Start Debugging" to launch the debugger and step through your code or reset the device.

## Interface Files

The `interface` folder contains source files that abstract the interaction with the ADE9178 IC using the MetIC Service APIs. Each file serves a specific purpose:

- **init**: Handles initialization of the ADE9178 device via MetIC Service APIs.
- **run**: Manages the main execution loop and device operation.
- **adapter**: Integrates external module dependencies required by the examples.

These files provide a modular structure, making it easier to configure, initialize, and operate the ADE9178 in your applications.

## References

- [CMake Documentation](https://cmake.org/documentation/)
- [ADE9178 Product Page](https://www.analog.com/en/products/ade9178.html)
- [ADE9178 Datasheet (PDF)](https://www.analog.com/media/en/technical-documentation/data-sheets/ade9178.pdf)
- [ADE9178 Evaluation Board](https://www.analog.com/en/resources/evaluation-hardware-and-software/evaluation-boards-kits/eval-ade9178.html)
- [MAX32670 Product Page](https://www.maximintegrated.com/en/products/microcontrollers/MAX32670.html)
- [MaximSDK GitHub](https://github.com/analogdevicesinc/msdk/tree/v2024_10)
- [MaximSDK Installer](https://analogdevicesinc.github.io/msdk//USERGUIDE/#installation)
