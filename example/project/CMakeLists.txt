cmake_minimum_required(VERSION 3.22)

add_definitions(-DTARGET_REV=0x4131)
add_definitions(-DTARGET=MAX32670)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -u _printf_float -u _scanf_float")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=nosys.specs")

# Define MaximSDK path if not already set
set(MaximSDK C:/MaximSDK CACHE PATH "Path to MaximSDK directory")
option(USE_CLI "Compile CLI sources" ON)
option(USE_CRC "Compile CRC sources" ON)
option(USE_NVM_FLASH "Compile NVM sources" ON)

# Set Target
set(BUILD_TARGET "max32670" CACHE STRING "Target")
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build mode")

# Set the project name
set(CMAKE_PROJECT_NAME ade9178_example)
# Base directory for METEX Directory
set(METEX_BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../)

# Explicitly set the toolchain path to avoid conflicts
set(TOOLCHAIN_PATH "C:/MaximSDK/Tools/GNUTools/10.3/bin")
set(TOOLCHAIN_PREFIX "${TOOLCHAIN_PATH}/arm-none-eabi-")


# Select the ld file
# Set MCU specific flags
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${METEX_BASE_DIR}/board_support/max/eval_ade9178/max32670.ld\"")
# MCU specific flagss
set(TARGET_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} ${TARGET_FLAGS}")


# Include toolchain file
include(${METEX_BASE_DIR}/board_support/cmake/gcc-arm-none-eabi.cmake)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
# Enable CMake support for ASM and C languages
enable_language(C ASM)


# Print Target Info
message("Build type: " ${CMAKE_BUILD_TYPE})
message(STATUS "TARGET_VALUE: ${BUILD_TARGET}")

# Create an executable object type
add_executable(${PROJECT_NAME})

set(TARGET_EXECUTABLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME})

# Add adc_service sources
add_subdirectory(
  ${METEX_BASE_DIR}/source
  ${CMAKE_BINARY_DIR}/source
)

# Add firmware_service sources
add_subdirectory(
  ${METEX_BASE_DIR}/firmware_services
  ${CMAKE_BINARY_DIR}/firmware_services
)

# Add boardS_support sources
add_subdirectory(
  ${METEX_BASE_DIR}/board_support
  ${CMAKE_BINARY_DIR}/board_support
)
# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        ${METEX_BASE_DIR}/example/source/main.c
        ${METEX_BASE_DIR}/example/source/nvm_reg.c
        ${METEX_BASE_DIR}/example/source/pulse_count.c
        ${METEX_BASE_DIR}/example/source/cli_commands.c
        ${METEX_BASE_DIR}/example/source/metic_config_groups.c
        ${METEX_BASE_DIR}/example/source/metic_example.c
        ${METEX_BASE_DIR}/example/source/error_display.c
        ${METEX_BASE_DIR}/example/source/example_display.c
        ${METEX_BASE_DIR}/example/source/cli_service_interface.c
        ${METEX_BASE_DIR}/example/source/nvm_service_interface.c
        ${METEX_BASE_DIR}/example/source/interface/metic_service_init_interface.c
        ${METEX_BASE_DIR}/example/source/interface/metic_service_run_interface.c
        ${METEX_BASE_DIR}/example/source/interface/metic_service_adapter.c
        ${METEX_BASE_DIR}/example/source/metic_service_max3267x.c
        ${METEX_BASE_DIR}/example/source/metic_service_no_os_interface.c

)

file(GLOB MAXIM_FILES "${MaximSDK}/Libraries/PeriphDrivers/Source/**/*.c")
set_source_files_properties(
  ${MAXIM_FILES}
  PROPERTIES
  COMPILE_FLAGS "-w"
)

set(APP_CFG_INCLUDE ${METEX_BASE_DIR}/example/include/config)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${METEX_BASE_DIR}/ade_registers/ade9178/
        ${METEX_BASE_DIR}/example/include
        ${METEX_BASE_DIR}/example/include/interface
        ${APP_CFG_INCLUDE}
)

target_include_directories(metic_service PUBLIC ${APP_CFG_INCLUDE})

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
metic_service firmware_services board_support
)

# Create hex file
add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${TOOLCHAIN_PREFIX}objcopy -O ihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex
)
