#
# The MIT License (MIT)
# 
# Copyright (c) 2012-2017 Fabio Lourencao De Giuli (http://degiuli.github.io)
# Copyright (c) 1998-2017 De Giuli Informatica Ltda. (http://www.degiuli.com.br)
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

cmake_minimum_required (VERSION 3.3)
if (NOT CMAKE_BUILD_TYPE)
		message(STATUS "No build type selected, default to Release")
		set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()
MESSAGE( STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE} )

project (MultiTypedQueue)
set(PROJECT_URL "https://github.com/degiuli/MultiTypedQueue")
set(PROJECT_DESCRIPTION "MultiTypedQueue")

# The version number.
set (MultiTypedQueue_VERSION_MAJOR 1)
set (MultiTypedQueue_VERSION_MINOR 0)
set (MultiTypedQueue_VERSION_PATCH 1)

# Debugging Options: 1 for debugging, 0 for release
set (CMAKE_VERBOSE_MAKEFILE 1)

MESSAGE( STATUS "CMAKE_CXX_COMPILER_ID: " ${CMAKE_CXX_COMPILER_ID} )
MESSAGE( STATUS "CMAKE_C_COMPILER: " ${CXX_COMPILER_VERSION} )
MESSAGE( STATUS "CXX_COMPILER_VERSION: " ${CXX_COMPILER_VERSION} )

# Compiler flags
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS_RELEASE "-Wall -Wcast-align -Ofast -DNDEBUG -std=c++11 -march=native")
    set (CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wcast-align -ggdb  -std=c++11 -march=native")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    set (CMAKE_CXX_FLAGS_RELEASE "-Wall -Ofast -DNDEBUG -std=c++11 -march=native")
    set (CMAKE_CXX_FLAGS_DEBUG   "-Wall -ggdb  -std=c++11 -march=native")
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS_RELEASE "-Wall -Wcast-align -O3 -DNDEBUG -std=c++11 -march=native")
    set (CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wcast-align -ggdb  -std=c++11 -march=native")
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    set (CMAKE_CXX_FLAGS_RELEASE "/Wall /DNDEBUG")
    set (CMAKE_CXX_FLAGS_DEBUG   "/Wall")
elseIf("${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang")
    set (CMAKE_CXX_FLAGS_RELEASE "-Wall -Wcast-align -O3 -DNDEBUG -std=c++11 -march=native")
    set (CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wcast-align -ggdb  -std=c++11 -march=native")
else()
  message(WARNING "Unknown compiler '${CMAKE_C_COMPILER_ID}'!")
endif()

MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG: " ${CMAKE_CXX_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE} )

include_directories("${PROJECT_SOURCE_DIR}")
 
# Executable
add_executable(MultiTypedQueue MultiTypedQueue.cpp)

enable_testing()
add_test(NAME MultiTypedQueue COMMAND bin)
