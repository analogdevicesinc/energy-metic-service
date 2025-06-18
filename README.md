<p align="center">
    <img src="docs/diagrams/adi_logo.png" width="250" alt="ADI Logo" /><br>
</p>

# ADE9178 Example & MetIC Service

## Table of Contents

- [Introduction](#introduction)
- [Directory Structure](#directory-structure)
- [Example Directory Structure](#example-directory-structure)
- [Building and Running Examples](#building-and-running-examples)
- [Porting Example to Different Boards](#porting-example-to-different-boards)
- [Building API Documentation](#building-api-documentation)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)
- [References](#references)

## Introduction

The MetIC Service provides APIs for communicating with ADI metrology ICs and collecting metrology parameters over a 4-wire SPI interface. This example uses the MetIC service and CLI to demonstrate various features of the ADE9178 IC. Refer to the [ade_registers](https://github.com/analogdevicesinc/energy-ade-registers) repository for low-level examples using the driver APIs directly. This release includes a CLI interface to read and write various register values from the ADE9178 IC and ADE9113/ADE9103 ADCs, as well as a simple example demonstrating the usage of the MetIC interface.

## Directory Structure

```
docs/                API documentation and release notes
examples/            ADE9178 Metrology example codes and projects
include/             MetIC service library headers
source/              MetIC service library sources
ade_registers/       Submodule - ADE9178 IC header files
firmware_services/   Submodule - Support modules (CLI, CRC, NVM)
board_support/       Submodule - Evaluation board support functions
```

## Example Directory Structure

```
example/
├── source/           Example source files to interact with ADE9178
├── include/          Example header files
├── simple_example/   Simple example showing how to use MetIC APIs
├── project/
│   ├── vscode/               VSCode project files
│   ├── CMakeLists.txt        CMake build configuration
│   ├── CMakePresets.json     CMake build presets
```

## Building and Running Examples

See the [example readme](example/readme.md) for instructions on building and running the example.

## Porting Example to Different Boards

Refer to the [porting guide](example/readme_porting.md) for intsruction to port the example to other boards

## Building API Documentation

API documentation for the MetIC service can be generated as follows:

1. Install [Doxygen](https://www.doxygen.nl/download.html) (v1.9.3) and [Graphviz](https://graphviz.gitlab.io/download/) (v2.38).
2. Ensure both tools are in your PATH.
3. Run:

    ```sh
    doxygen ade9178_example_doxy_config
    ```

The documentation will be generated in the output directory specified in the Doxygen configuration.


## Contributing

Contributions are welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for details.

## License

Licensed under the [Apache 2.0 License](LICENSE).

## Contact

For questions or support, open an issue on [GitHub](https://github.com/analogdevicesinc/energy-ade9178-example/issues) or contact the maintainers listed in the repository.

## References

- [CMake Documentation](https://cmake.org/documentation/)
- [Doxygen Documentation](https://www.doxygen.nl/manual/docblocks.html)