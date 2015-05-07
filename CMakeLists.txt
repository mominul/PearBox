cmake_minimum_required (VERSION 2.8.8)

project (PearBox)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

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

add_executable(PearBox pearbox.cpp configuration.cpp createimage.cpp configparser.cc)
qt5_use_modules(PearBox Widgets)
target_link_libraries(PearBox libtools)
