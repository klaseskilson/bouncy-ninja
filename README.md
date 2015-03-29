# bouncy-ninja

Bouncy ninja is a soft body simulation implemented in C++ and OpenGL using a mass-spring-dampening-system.

## Getting started

### Requirements

This projects depend on:

* [CMake](http://www.cmake.org/install/)
* OpenGL
* GLFW (version `3.0.4 +`)
* GLM
* GLEW

#### Mac OS X

We recommend you to install the dependencies using [Homebrew](http://brew.sh/). Simply run these 
commands, and you should be good to go:

```bash
brew install cmake
brew install glfw3
brew install glm
brew install glew
```

## Running

Cmake into your preferred project file. The [`CMakeLists.txt`](CMakeLists.txt)
file has been tested to work and run on Linux (Mint) and Mac OS X.
**Compile and run!**

To build and run the project, run these commands in your terminal:

```bash 
mkdir build
cd build
cmake ..
make 
../bin/bouncy
```
