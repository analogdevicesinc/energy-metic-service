# ADE9178 Evaluation CLI 

This application provides  command line interface to communicate with the ADE9178 using the MetIC Service. 

```
├── source/                 Source files for CLI framework
├── include/                Header files
├── project/
│   ├── .vscode/            VSCode project files
│   ├── CMakeLists.txt      CMake build configuration
│   ├── CMakePresets.json   CMake build presets
├── readme.md               Evaluation firmware documentation
```

### Hardware Setup and Connections

Refer to [board support readme](https://github.com/analogdevicesinc/energy-board-support/blob/main/max/eval_ade9178/readme.md) for required HW setup.

### Static Configuration

The application includes headers [app_cfg.h](project/config/app_cfg.h)  for static configurations.


### Building, Running, and Debugging Examples

- [`CMakeLists.txt`](project/CMakeLists.txt) is provided for building the firmware.
- A [VS Code workspace](project/cli_firmware.code-workspace) is also given as part of the example.
- See the [board support readme](https://github.com/analogdevicesinc/energy-board-support/blob/main/max/eval_ade9178/readme.md) for instructions on building, running, and debugging the example.


For supported CLI commands and features, refer to the [ADE9178 Example User Guide](../docs/ADE9178ExampleUserGuide.pdf).


