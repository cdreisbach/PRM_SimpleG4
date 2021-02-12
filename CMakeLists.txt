cmake_minimum_required(VERSION 3.0)
project(PRMSIMPLEG4)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmakeModules")
set(CMAKE_CXX_STANDARD 17)

# redirect output files
message(STATUS "")
message(STATUS ">>> Setting up output paths.")

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
message(STATUS "Using library output path '${LIBRARY_OUTPUT_PATH}'.")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
message(STATUS "Using executable output path '${EXECUTABLE_OUTPUT_PATH}'.")

# Scripts - Simulation
add_subdirectory(simulation)

