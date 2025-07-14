cmake_minimum_required(VERSION 3.22)

# Project name and executable
set(CMAKE_PROJECT_NAME metic_service_example)

# Base directories (allow override from command line or parent project)
set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../" CACHE PATH "Project root directory")
set(BOARD_SUPPORT_DIR "${PROJECT_ROOT_DIR}/board_support" CACHE PATH "Board support directory")
set(ADE_REGISTERS_DIR "${PROJECT_ROOT_DIR}/ade_registers" CACHE PATH "Directory for ADE registers"  )
set(ADCSIF_DIR "${PROJECT_ROOT_DIR}/interface" CACHE PATH "Directory for ADC service interface")
set(METIC_LIB_DIR "${PROJECT_ROOT_DIR}/source" CACHE PATH "Directory for ADC service library")
set(CONFIG_DIR "${CMAKE_CURRENT_LIST_DIR}/config" CACHE PATH "Configuration directory")
set(FW_SERVICES_DIR "${PROJECT_ROOT_DIR}/firmware_services" CACHE PATH "Directory for firmware services")

# Build options
set(EVB "eval_ade9178" CACHE STRING "Name of evaluation board")
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type")

option(USE_CLI "Compile CLI sources" ON)
option(USE_NVM_FLASH "Compile NVM sources" ON)

# ------------------------------------------------------------------------------
# Toolchain & Project Setup
# ------------------------------------------------------------------------------
include(${BOARD_SUPPORT_DIR}/cmake/toolchain_wrapper.cmake)
project(${CMAKE_PROJECT_NAME})

# ------------------------------------------------------------------------------
# Source Files
# ------------------------------------------------------------------------------
set(ADCSIF_SRC
    ${ADCSIF_DIR}/source/metic_service_init_interface.c
    ${ADCSIF_DIR}/source/metic_service_adapter.c
    ${ADCSIF_DIR}/source/metic_service_run_interface.c
)
    
set(APP_SRC
      ${PROJECT_ROOT_DIR}/eval_firmware/source/main.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/nvm_reg.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/pulse_count.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/cli_commands.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/metic_config_groups.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/metic_example.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/error_display.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/example_display.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/cli_service_interface.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/nvm_service_interface.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/metic_service_max3267x.c
      ${PROJECT_ROOT_DIR}/eval_firmware/source/metic_service_no_os_interface.c
      ${ADE_REGISTERS_DIR}/crc/source/ade_crc.c
      ${ADCSIF_SRC}
)


# ------------------------------------------------------------------------------
# Include Directories
# ------------------------------------------------------------------------------
set(APP_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${ADE_REGISTERS_DIR}/ade9178/include
    ${ADE_REGISTERS_DIR}/ade91xx/include
    ${ADE_REGISTERS_DIR}/include
    ${ADE_REGISTERS_DIR}/crc/include
    ${PROJECT_ROOT_DIR}/include
    ${CONFIG_DIR}
    ${ADCSIF_DIR}/include
)

# ------------------------------------------------------------------------------
# Add Subdirectories
# ------------------------------------------------------------------------------
add_subdirectory(${BOARD_SUPPORT_DIR} ${CMAKE_BINARY_DIR}/board_support)
add_subdirectory(${METIC_LIB_DIR} ${CMAKE_BINARY_DIR}/adc_service)
add_subdirectory(${FW_SERVICES_DIR} ${CMAKE_BINARY_DIR}/firmware_services)
target_include_directories(metic_service PRIVATE ${CONFIG_DIR})



# ------------------------------------------------------------------------------
# Executable Target Setup
# ------------------------------------------------------------------------------
add_executable(${CMAKE_PROJECT_NAME})
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${APP_SRC})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${APP_INCLUDES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE metic_service firmware_services board_support)

# ------------------------------------------------------------------------------
# Post-build: Generate HEX file
# ------------------------------------------------------------------------------
add_hex_generation(${CMAKE_PROJECT_NAME})

