
cmake_minimum_required( VERSION 3.5.1 ) 
set (CMAKE_CXX_STANDARD 11)

project( openCL_sample ) 

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")
set (opencl_example_src src/openCL_sample.cpp)

find_package(OpenCV REQUIRED)
find_package( OpenCL REQUIRED )
 
include_directories( ${OpenCV_INCLUDE_DIRS})
include_directories( ${OPENCL_INCLUDE_DIR} )

add_executable (openCL_sample ${opencl_example_src})
target_link_libraries(openCL_sample OpenCL)
target_link_libraries( openCL_sample ${OpenCV_LIBS}) 


