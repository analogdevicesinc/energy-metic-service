cmake_minimum_required(VERSION 3.22)

# Define an option for the build type
option(ZEPHYR_BUILD "Build app with Zephyr" OFF)

project(met_ic_service C)

set(METIC_SERVICE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

set(SRC # ADC Service sources
        ${METIC_SERVICE_DIR}/source/metic_scomm_91xx_private.c
        ${METIC_SERVICE_DIR}/source/metic_output_groups.c
        ${METIC_SERVICE_DIR}/source/adi_metic.c
        ${METIC_SERVICE_DIR}/source/adi_metic_convert.c
        ${METIC_SERVICE_DIR}/source/adi_metic_wfs_receive.c
)

set(INCLUDE # ADC application includes
			${METIC_SERVICE_DIR}/include
      ${METIC_SERVICE_DIR}/ade_registers/ade9178/include)

include(CMakePrintHelpers)
cmake_print_variables(ZEPHYR_BUILD CMAKE_CURRENT_LIST_DIR CMAKE_BINARY_DIR)

  # Set the project name
  project(metic_service)
  # Baremetal build
  # Add the executable
  add_library(metic_service ${SRC})

#Provide include path of app_cfg to mathop
target_include_directories(metic_service PUBLIC ${INCLUDE})

if (ZEPHYR_BUILD)
#This is required to pass correct build flags for library
target_link_libraries(metic_service PUBLIC zephyr_interface)
endif()

