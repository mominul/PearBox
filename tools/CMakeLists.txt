# Add PearPC tools stuff to compile
#include_directories(. ../.. ../debug ../includes ${SDL2_INCLUDE_DIR}) 

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-write-strings")

add_library(libtools
	atom.cc data.cc debug.cc except.cc file.cc snprintf.cc
	str.cc stream.cc strtools.cc sys.cc sysfile.cc
	)
