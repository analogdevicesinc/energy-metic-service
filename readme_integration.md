# Integrating the MetIC Service

## Overview

The MetIC service provides a streamlined interface for communicating with the ADE9178 device, offering APIs for initialization, configuration, register access, and waveform streaming. Source files are located in the [`source`](source) directory, with headers in [`include`](include). Register definitions for the ADE9178 can be found in the [`ade_registers/ade9178`](https://github.com/analogdevicesinc/energy-ade-registers/tree/main/ade9178/include) directory.

A 16-bit CRC routine is required for data integrity. You can implement this using the provided CRC service code. See the [adapter file](interface/source/metic_service_adapter.c) for an example implementation.

A sample [`CMakeLists.txt`](source/CMakeLists.txt) is available in the [`source`](source) directory, demonstrating the necessary sources and includes. Projects using CMake can add the MetIC service by including the [`source`](source) directory as a subdirectory with the `add_subdirectory` command. Refer to the [example CMakeLists.txt](example/project/CMakeLists.txt) for additional guidance.

## Interface Files

The `interface` folder contains source files that abstract the interaction with the ADE9178 IC using the MetIC Service APIs. Each file has a specific role:

- **metic_service_init_interface**: Handles initialization of the ADE9178 device via MetIC Service APIs.
- **metic_service_run_interface**: Manages the main execution loop and device operation.
- **metic_service_adapter**: Integrates external module dependencies required by the MetIC Service, such as function pointers that must be set before calling any APIs. This file is application-dependent; refer to the API documentation for details.

These files provide a modular structure, simplifying configuration, initialization, and operation of the ADE9178 in your applications.

**Note:** The interface files are designed to enable integration with the ADE9178 without dependencies on external modules, allowing for flexible adaptation to different projects.

## Quick Start

To integrate the MetIC service into your project, follow these steps:

1. **Add Source Files:**  
    Include all source files from [`source`](source) and [`interface/source`](interface/source) in your build system.

2. **Configure Include Paths:**  
    Add the following directories to your project's include paths:
    - [`include`](include)
    - [`ade_registers/ade9178/include`](https://github.com/analogdevicesinc/energy-ade-registers/tree/main/ade9178/include)
    - [`interface/include`](interface/include)

3. **CRC Implementation:**  
    Implement or integrate 16-bit CRC routines as required by the service. The provided CRC service code can be used as a reference.

4. **Initialize Service and Hardware:**  
    Initialize the MetIC service and configure the hardware interface (e.g., SPI, UART) as needed. 

5. **Use the APIs:**  
    Use the provided APIs for device configuration, register access, and waveform data acquisition.

