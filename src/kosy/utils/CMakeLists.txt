kosy_cxx_library(
  NAME
    logging
  HDRS
    "logging.hpp"
  DEPS
    spdlog::spdlog_header_only
)

kosy_cxx_library(
  NAME
    exec_on_exit
  HDRS
    "exec_on_exit.hpp"
)

kosy_cxx_library(
  NAME
    chrono_inspector
  HDRS
    "chrono_inspector.hpp"
  DEPS
    logging
)

kosy_cxx_library(
  NAME
    aligned_allocator
  HDRS
    "aligned_allocator.hpp"
)

kosy_cxx_library(
  NAME
    aligned_memory_resource
  HDRS
    "aligned_memory_resource.hpp"
  DEPS
    Boost::serialization
)

kosy_cxx_library(
  NAME
    type_traits
  HDRS
    "type_traits.hpp"
)

kosy_cxx_library(
  NAME
    concepts
  HDRS
    "concepts.hpp"
  DEPS
    type_traits
)
