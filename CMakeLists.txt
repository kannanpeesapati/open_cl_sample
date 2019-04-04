
cmake_minimum_required( VERSION 3.5.1 ) 
set (CMAKE_CXX_STANDARD 11)

# Build type
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Debug' as none was specified.")
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif ()

project( openCL_sample ) 

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")
set (opencl_example_src src/openCL_sample.cpp)


find_package(OpenCV REQUIRED)
find_package( OpenCL REQUIRED )

 
include_directories( ${OpenCV_INCLUDE_DIRS})
include_directories( ${OPENCL_INCLUDE_DIR} )

if (CMAKE_COMPILER_IS_GNUCC)
    set (CMAKE_CXX_FLAGS "-D_REETRANT -Wall -Wextra -pedantic -Wno-long-long")
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   	    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0")
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
	    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -O3 -fno-strict-aliasing")
	endif ()
endif (CMAKE_COMPILER_IS_GNUCC)

 
add_executable (openCL_sample ${opencl_example_src})
target_link_libraries(openCL_sample OpenCL)
target_link_libraries( openCL_sample ${OpenCV_LIBS}) 


