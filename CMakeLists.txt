# project name is not mandatory but should be used
#------------------------------------------------------------------

SET(APP_NAME bouncy)
project(${APP_NAME})

# states the minimum version required 
#------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)

# include the directory itself as a path to include directories
# a directory is a file system cataloging structure which 
# contains references to other computer files
#------------------------------------------------------------------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# create a variable called .._SOURCES containing all .cpp files
#------------------------------------------------------------------
set(bouncy_SOURCES main.cpp)

# for a large number of source files it can be created using
# file(GLOB bouncy_SOURCES *.cpp)

# find packages that is required
#------------------------------------------------------------------
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

# get variable GLFW_INCLUDE_DIRS when searching module it contains 
# paths to directories to the header files we want to include
#------------------------------------------------------------------
include_directories(${GLFW_INCLUDE_DIRS})

# create an executable file from sources, create it first,
# then link the libraries
#------------------------------------------------------------------
add_executable(${APP_NAME} ${bouncy_SOURCES})

# GLFW_STATIC_LIBRARIES is also retrived when running search module
# it contains all the external libraries that are needed
#------------------------------------------------------------------
target_link_libraries(${APP_NAME} 

    ${GLFW_STATIC_LIBRARIES}

)

