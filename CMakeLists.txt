# Set the name and the supported language of the project
#project(pearpcgui)
# Set the minimum version of cmake required to build this project
#cmake_minimum_required(VERSION 2.6)
# Use the package PkgConfig to detect GTK+ headers/library files
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
#include_directories(${GTK3_INCLUDE_DIRS})
#link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
#add_definitions(${GTK3_CFLAGS_OTHER})
# Add an executable compiled from hello.c
#add_executable(PearPCGUI main.c)
# Link the target to the GTK+ libraries
#target_link_libraries(PearPCGUI ${GTK3_LIBRARIES})

cmake_minimum_required (VERSION 2.6)
project (PearPC_Virtual_Machine_Manager)
include (CheckIncludeFiles)
check_include_files (stdint.h HAVE_STDINT_H)
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_SOURCE_DIR}/config.h"
  )
include_directories(tools)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/tools)
add_subdirectory(tools)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
add_executable(PearPCVMM pearpcgui.cpp configuration.cpp configparser.cc)
target_link_libraries(PearPCVMM libtools)
