add_subdirectory(utils)

kosy_cxx_library(
  NAME
    polynomial
  HDRS
    "polynomial.hpp"
  DEPS
    OpenMP::OpenMP_CXX
    aligned_allocator
    concepts
)

nanobind_add_module(_core
  STABLE_ABI
  NB_STATIC
  _core.cpp
  polynomial_nb.cpp
)
target_include_directories(_core
  PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
)
target_link_libraries(_core PRIVATE kosy::polynomial)
set_property(TARGET _core PROPERTY FOLDER ${KOSY_IDE_FOLDER}/python)
set_target_properties(_core PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

nanobind_add_stub(_core_stub
  MODULE _core
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/_core.pyi"
  PYTHON_PATH "${CMAKE_CURRENT_SOURCE_DIR}"
  DEPENDS _core
)

file(TOUCH "${CMAKE_CURRENT_SOURCE_DIR}/py.typed")

if(KOSY_ENABLE_INSTALL)
  install(TARGETS _core LIBRARY DESTINATION kosy)
  install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/_core.pyi"
    "${CMAKE_CURRENT_SOURCE_DIR}/py.typed"
    "${CMAKE_CURRENT_SOURCE_DIR}/__init__.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/polynomial.py"
    DESTINATION kosy
  )
endif()
