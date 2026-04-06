cmake_minimum_required(VERSION 3.31.0)

project(kosy
  VERSION
    0.1.0
  DESCRIPTION
    "Funny Math Problems."
  HOMEPAGE_URL
    https://github.com/houchen-li/kosy
  LANGUAGES
    C
    CXX
)

set(CMAKE_C_STANDARD 23 CACHE INTERNAL "C standard")
set(CMAKE_CXX_STANDARD 23 CACHE INTERNAL "C++ standard")
set(CMAKE_COMPILE_WARNING_AS_ERROR True CACHE INTERNAL "Treat warnings as errors")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")

option(CMAKE_CXX_EXTENSIONS "Enable C++ extensions" ON)
option(CMAKE_UNITY_BUILD "Enable unity build" OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(KOSY_BUILD_TESTING "Enable testing" OFF)
option(KOSY_ENABLE_INSTALL "Enable install" ON)

set(KOSY_SOVERSION 0 CACHE INTERNAL "kosy shared library version")

if(NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(KOSY_ENABLE_INSTALL OFF)
endif()

set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/third_party CACHE PATH "CPM source cache")

add_subdirectory(cmake/third_party)
include(cmake/utils.cmake)

add_subdirectory(src)
add_subdirectory(tests)

if(KOSY_ENABLE_INSTALL)
  include(cmake/export.cmake)
endif()

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  include(cmake/compile_commands.cmake)
endif()
