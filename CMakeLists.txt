cmake_minimum_required(VERSION 3.31...4.3)

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
set(KOSY_SOVERSION 0 CACHE INTERNAL "kosy shared library version")

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

option(CMAKE_UNITY_BUILD "Enable unity build" OFF)
option(KOSY_BUILD_TESTING "Enable testing" OFF)
option(KOSY_ENABLE_INSTALL "Enable install" ON)

if(NOT PROJECT_IS_TOP_LEVEL)
  set(KOSY_ENABLE_INSTALL OFF)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/third_party CACHE PATH "CPM source cache")

if(KOSY_BUILD_TESTING)
  include(CTest)
  enable_testing()
endif()

include(cmake/utils.cmake)
add_subdirectory(cmake/third_party)

add_subdirectory(src)
add_subdirectory(tests)

if(KOSY_ENABLE_INSTALL)
  include(cmake/export.cmake)
endif()

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  include(cmake/compile_commands.cmake)
endif()
