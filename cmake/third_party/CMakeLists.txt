if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  if(WIN32)
    find_package(LLVM 20.1.0 REQUIRED CONFIG)
  elseif(APPLE)
    find_package(LLVM 22.1.0 REQUIRED CONFIG)
  else()
    find_package(LLVM 22.1.0 REQUIRED CONFIG)
  endif()
endif()

find_package(Threads REQUIRED MODULE GLOBAL)

find_package(OpenMP REQUIRED MODULE GLOBAL)

if (WIN32)
  set(Python_EXECUTABLE ${CMAKE_SOURCE_DIR}/.venv/Scripts/python.exe CACHE FILEPATH "Python executable")
else()
  set(Python_EXECUTABLE ${CMAKE_SOURCE_DIR}/.venv/bin/python CACHE FILEPATH "Python executable")
endif()

find_package(Python 3.14 REQUIRED COMPONENTS Interpreter Development)

if(NOT Python_INCLUDE_DIRS)
  execute_process(
    COMMAND "${Python_EXECUTABLE}" -c "import sysconfig; print(sysconfig.get_path('include'))"
    OUTPUT_VARIABLE Python_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()

set(Python_INCLUDE_DIRS "${Python_INCLUDE_DIRS}" CACHE INTERNAL "Python include directories")

add_subdirectory(cpm)
add_subdirectory(cxxopts)
add_subdirectory(doctest)
add_subdirectory(matplotplusplus)
add_subdirectory(nanobind)
add_subdirectory(spdlog)
