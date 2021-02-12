cmake_minimum_required(VERSION 3.0)
project(Simulation)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmakeModules")
set(CMAKE_CXX_STANDARD 17)

# Includes
include(CommonMacros)

# setup ROOT includes and libraries
message(STATUS "")
message(STATUS ">>> Setting up ROOT.")
find_package(ROOT 5.34.04 REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

# Geant4
message(STATUS "")
message(STATUS ">>> Setting up G4.")
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    message(STATUS "With Visualization.")
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    message(STATUS "No Visualization.")
    find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})
include_directories(${Geant4_INCLUDE_DIRS})

# Source files
file(GLOB SIMULATION_SOURCES src/*.cc)
file(GLOB SIMULATION_HEADERS include/*.hh)
include_directories(include)

# Library
set(SIMULATION_LIB "simulation")
make_shared_library("simulation" "${SIMULATION_SOURCES}" "${ROOT_LIBS}" "${Geant4_LIBRARIES}")

# Executable
message(STATUS "")
make_executable(Simulation Simulation.cc ${SIMULATION_LIB})
