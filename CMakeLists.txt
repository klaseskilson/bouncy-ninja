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

# create an executable file from sources, create it first,
# then link the libraries
#------------------------------------------------------------------
add_executable(${APP_NAME} ${bouncy_SOURCES})

# for a large number of source files it can be created using
# file(GLOB bouncy_SOURCES *.cpp)

# find packages that is required
# pkg-config is a helper tool that helps you inser the correct
# compiler options
#------------------------------------------------------------------
find_package(OpenGL REQUIRED)

#------------------------------------------------------------------
# if WIN32 we use a custom FindGLFW.cmake and assign the
# variables/paths manually
if (WIN32)
	find_package(GLFW REQUIRED)
	SET(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
	SET(GLFW_STATIC_LIBRARIES ${OPENGL_LIBRARY} ${GLFW_LIBRARIES})
else()
  #If not WIN32 we use PkgConfig to find GLFW
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
endif()

#------------------------------------------------------------------
# OSX-specific requirements
if(APPLE)
  # use c++11
  SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
  set(CMAKE_OSX_ARCHITECTURES "x86_64")
  target_link_libraries(${APP_NAME} "-framework OpenGL")

  if(CMAKE_GENERATOR STREQUAL Xcode)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.8")
  endif()
endif()

# get variable GLFW_INCLUDE_DIRS when searching module it contains
# paths to directories to the header files we want to include
#------------------------------------------------------------------
include_directories(${GLFW_INCLUDE_DIRS})

# GLFW_STATIC_LIBRARIES is also retrived when running search module
# it contains all the external libraries that are needed
#------------------------------------------------------------------
target_link_libraries(${APP_NAME} ${GLFW_STATIC_LIBRARIES})

