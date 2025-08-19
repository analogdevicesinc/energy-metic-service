<p align="center">
    <img src="docs/diagrams/adi_logo.png" width="250" alt="ADI Logo" /><br>
</p>

# ADE9178 Example & MetIC Service

## Table of Contents

- [Introduction](#introduction)
- [Directory Structure](#directory-structure)
- [Example Directory Structure](#example-directory-structure)
- [Building, Running, and Debugging Examples](#building-running-and-debugging-examples)
- [Porting MetIC Service to Different Applications](#porting-metic-service-to-different-applications)
- [Building API Documentation](#building-api-documentation)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)
- [References](#references)

## Introduction

The MetIC Service provides APIs for communicating with ADI metrology ICs and collecting metrology parameters over a 4-wire SPI interface. This repository demonstrates how to use the MetIC service and CLI to interact with the ADE9178 IC. For low-level driver examples, refer to the [ADE registers repository](https://github.com/analogdevicesinc/energy-ade-registers). This release includes a CLI interface for reading and writing register values from the ADE9178 IC and ADE9113/ADE9103 ADCs, as well as an example showcasing the MetIC interface.

## Directory Structure

```
docs/                API documentation and release notes
eval_firmware/       ADE9178 metrology example projects using the CLI interface
examples/            ADE9178 metrology example showcasing basic register read and write using MetIC service APIs
include/             MetIC service library headers
source/              MetIC service library sources
interface/           Interface files for ADE9178 interaction
ade_registers/       Submodule - ADE9178, ADE9113, ADE9103 IC header files
firmware_services/   Submodule - Support modules (CLI, NVM)
board_support/       Submodule - Evaluation board support functions
```

## Example Directory Structure



### Eval Firmware Directory

This directory provides a more comprehensive evaluation firmware for the ADE9178, utilizing the MetIC Service along with additional modules for user interaction. See the directory's `readme.md` for further information.


## Integrating MetIC Service to the application

See the [integration guide](readme_integration.md) for instructions on integrating the MetIC service to the applications

## Building API Documentation

API documentation for the MetIC service can be generated as follows:

1. Install [Doxygen](https://www.doxygen.nl/download.html) (v1.9.3) and [Graphviz](https://graphviz.gitlab.io/download/) (v2.38).
2. Ensure both tools are in your PATH.
3. Run:
    ```sh
    doxygen ade9178_example_doxy_config
    ```
4. The generated documentation will be available in the `html` folder; open `index.html` to view it.

The output directory is specified in the Doxygen configuration file.

## Contributing

Contributions are welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for details.

## License

Licensed under the [Apache 2.0 License](LICENSE).

## Contact

For questions or support, open an issue on [GitHub](https://github.com/analogdevicesinc/energy-metic-service/issues) or contact the maintainers listed in the repository.

## References

- [Doxygen Documentation](https://www.doxygen.nl/manual/docblocks.html)
