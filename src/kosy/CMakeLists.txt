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

kosy_nb_library(
  NAME
    polynomial_ext
  SRCS
    polynomial_nb.cpp
  DEPS
    polynomial
)
