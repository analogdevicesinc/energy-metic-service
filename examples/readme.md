# ADE9178 Example

The example provided demonstrates how ADC samples can be collected using ADC service. First few samples collected are printed to the terminal.

### Hardware Setup and Connections

Refer to [board support readme](https://github.com/analogdevicesinc/energy-board-support/blob/main/max/eval_ade9178/readme.md) for required HW setup.

### Static Configuration

Example includes headers [app_cfg.h](projects/config/app_cfg.h)  for static configurations.


### Building, Running, and Debugging Examples

- [`CMakeLists.txt`](projects/CMakeLists.txt) is provided for building the example
- A [VS Code workspace](projects/frame_format_example.code-workspace) is also given as part of the example.
- See the [board support readme](https://github.com/analogdevicesinc/energy-board-support/blob/main/max/eval_ade9178/readme.md) for instructions on building, running, and debugging the example.

### Board Support Functions

The following functions from the [board_support repo](https://github.com/analogdevicesinc/energy-board-support/tree/main/generic/include) are used in this example. Users must implement these functions to ensure the example builds and operates correctly:

- `EvbInit`
- `EvbInitMessageBuffer`
- `EvbResetAdcs`
- `EvbDelayMs`
- `EvbEnableDreadyIrq`
- `EvbAdeSpiTransceive`
- `EvbFlushMessages`
