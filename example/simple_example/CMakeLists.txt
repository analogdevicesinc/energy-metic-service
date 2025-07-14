cmake_minimum_required(VERSION 3.22)

# Base directory for SMLIB software
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# FreeRTOS support is not added.
option(USE_FREERTOS "Compile FREERTOS sources" OFF)
option(USE_DUMMY_EVB "Use dummy EVB" OFF)
set(BUILD_TARGET "max32670" CACHE STRING "Target")
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build mode")


if(NOT USE_DUMMY_EVB)

add_definitions(-DTARGET_REV=0x4131)
add_definitions(-DTARGET=MAX32670)

set(TOOLCHAIN_PATH "C:/MaximSDK/Tools/GNUTools/10.3/bin")
set(TOOLCHAIN_PREFIX "${TOOLCHAIN_PATH}/arm-none-eabi-")


set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${PROJECT_ROOT_DIR}/board_support/max/eval_ade9178/max32670.ld\"")
# MCU specific flagss
set(TARGET_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} ${TARGET_FLAGS}")

# Include toolchain file
include(${PROJECT_ROOT_DIR}/board_support/cmake/gcc-arm-none-eabi.cmake)

endif()

# Set the project name
project(ade9178_simple_example)

# Set the output file extension to .exe

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Add the executable
add_executable(ade9178_simple_example)

set(CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR}/../include/config)

# Add the source directory
add_subdirectory(../../source ${CMAKE_BINARY_DIR}/source)

# Define MaximSDK path if not already set
set(MaximSDK C:/MaximSDK CACHE PATH "Path to MaximSDK directory")
set(CRC_DIR ../../firmware_services/services/crc)
set(CRC_SRC ${CRC_DIR}/source/adi_crc_ccitt16.c)
set(CRC_INCLUDE ${CRC_DIR}/include)
set(METICIF_INCLUDE ../include/interface/)

set(METICIF_SRC  ../source/interface/metic_service_init_interface.c  ../source/interface/metic_service_adapter.c)

if(USE_FREERTOS)
set(APP_SRC ${CMAKE_CURRENT_LIST_DIR}/simple_example_freertos.c)
add_definitions(-DUSE_FREERTOS)
else()
set(APP_SRC ${CMAKE_CURRENT_LIST_DIR}/simple_example_noos.c)
endif()

if(NOT DEFINED BOARD_SUPPORT_DIR)
# BSP directory in firmware_services submodule
set(BOARD_SUPPORT_DIR ${PROJECT_ROOT_DIR}/board_support)
endif()

if(NOT USE_DUMMY_EVB)
file(GLOB MAXIM_FILES "${MaximSDK}/Libraries/PeriphDrivers/Source/**/*.c")
set_source_files_properties(
  ${MAXIM_FILES}
  PROPERTIES
  COMPILE_FLAGS "-w"
)
endif()

if(USE_DUMMY_EVB)
set(APP_SRC  ${APP_SRC} ${CMAKE_CURRENT_LIST_DIR}/../../board_support/dummy_board/adi_evb.c)
set(APP_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../../board_support/dummy_board/
${CMAKE_CURRENT_LIST_DIR}/../../board_support/dummy_board/config/)
else()
# Add boardS_support sources
add_subdirectory(
  ${PROJECT_ROOT_DIR}/board_support
  ${CMAKE_BINARY_DIR}/board_support
)
set(BOARD_SUPPORT_LIBS board_support)
endif()


set(APP_INCLUDE ${APP_INCLUDE} ${CONFIG_DIR})

include(CMakePrintHelpers)
cmake_print_variables(APP_INCLUDE)
cmake_print_variables(USE_DUMMY_EVB)
cmake_print_variables(BOARD_SUPPORT_LIBS)

target_include_directories(metic_service PUBLIC ${CONFIG_DIR} ${INCLUDE_DIR})

# Include the CRC headers
target_include_directories(ade9178_simple_example PRIVATE ${CRC_INCLUDE} ${METICIF_INCLUDE} ${APP_INCLUDE} )

# Add the CRC source file to the executable
target_sources(ade9178_simple_example PRIVATE ${CRC_SRC} ${METICIF_SRC} ${APP_SRC})

# Link the source files to the executable
target_link_libraries(ade9178_simple_example metic_service ${BOARD_SUPPORT_LIBS})

if(NOT USE_DUMMY_EVB)
# Create hex file
add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${TOOLCHAIN_PREFIX}objcopy -O ihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex
)
else()
add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_PROJECT_NAME}.exe ${CMAKE_PROJECT_NAME}.hex
)
endif()
