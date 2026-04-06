include(CMakeParseArguments)

if(KOSY_BUILD_TESTING)
  include(CTest)
  enable_testing()
endif()

if(NOT DEFINED KOSY_IDE_FOLDER)
  set(KOSY_IDE_FOLDER ${CMAKE_PROJECT_NAME})
endif()

function(kosy_cxx_library)
  cmake_parse_arguments(KOSY_CXX_LIB
    "DISABLE_INSTALL;PUBLIC;TESTONLY"
    "NAME"
    "HDRS;SRCS;COPTS;DEFINES;LINKOPTS;DEPS"
    ${ARGN}
  )

  set(_SRCS "${KOSY_CXX_LIB_SRCS}")
  list(FILTER _SRCS EXCLUDE REGEX ".*\\.(h|hpp|inc)")
  if(_SRCS STREQUAL "")
    set(_IS_INTERFACE 1)
  else()
    set(_IS_INTERFACE 0)
  endif()
  unset(_SRCS)

  if(NOT _IS_INTERFACE)
    add_library(${KOSY_CXX_LIB_NAME} "")
    set_property(TARGET ${KOSY_CXX_LIB_NAME} PROPERTY LINKER_LANGUAGE "CXX")
    target_sources(${KOSY_CXX_LIB_NAME}
      PRIVATE
        ${KOSY_CXX_LIB_SRCS}
      PUBLIC
        FILE_SET HEADERS
          TYPE HEADERS
          BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
          FILES ${KOSY_CXX_LIB_HDRS}
    )
    target_include_directories(${KOSY_CXX_LIB_NAME}
      PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
    target_compile_options(${KOSY_CXX_LIB_NAME} PRIVATE ${KOSY_CXX_LIB_COPTS})
    target_compile_definitions(${KOSY_CXX_LIB_NAME} PUBLIC ${KOSY_CXX_LIB_DEFINES})
    target_link_options(${KOSY_CXX_LIB_NAME} PRIVATE ${KOSY_CXX_LIB_LINKOPTS})
    target_link_libraries(${KOSY_CXX_LIB_NAME} PUBLIC ${KOSY_CXX_LIB_DEPS})

    if(KOSY_CXX_LIB_PUBLIC)
      set_property(TARGET ${KOSY_CXX_LIB_NAME} PROPERTY FOLDER ${KOSY_IDE_FOLDER})
    elseif(KOSY_CXX_LIB_TESTONLY)
      set_property(TARGET ${KOSY_CXX_LIB_NAME} PROPERTY FOLDER ${KOSY_IDE_FOLDER}/tests)
    else()
      set_property(TARGET ${KOSY_CXX_LIB_NAME} PROPERTY FOLDER ${KOSY_IDE_FOLDER}/internal)
    endif()

    if(WIN32 AND BUILD_SHARED_LIBS)
      set_target_properties(${KOSY_CXX_LIB_NAME} PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS TRUE
      )
    endif()

    if(KOSY_ENABLE_INSTALL)
      set_target_properties(${KOSY_CXX_LIB_NAME} PROPERTIES
        OUTPUT_NAME "kosy_${KOSY_CXX_LIB_NAME}"
        SOVERSION "${KOSY_SOVERSION}"
      )
    endif()
  else()
    add_library(${KOSY_CXX_LIB_NAME} INTERFACE)
    target_sources(${KOSY_CXX_LIB_NAME}
      INTERFACE
        FILE_SET HEADERS
          TYPE HEADERS
          BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
          FILES ${KOSY_CXX_LIB_HDRS}
    )
    target_include_directories(${KOSY_CXX_LIB_NAME}
      INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
    target_compile_definitions(${KOSY_CXX_LIB_NAME} INTERFACE ${KOSY_CXX_LIB_DEFINES})
    target_link_options(${KOSY_CXX_LIB_NAME} INTERFACE ${KOSY_CXX_LIB_LINKOPTS})
    target_link_libraries(${KOSY_CXX_LIB_NAME} INTERFACE ${KOSY_CXX_LIB_DEPS})
  endif()

  unset(_IS_INTERFACE)

  if(KOSY_ENABLE_INSTALL AND NOT KOSY_CXX_LIB_DISABLE_INSTALL)
    file(RELATIVE_PATH _REL_PATH "${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
    install(TARGETS ${KOSY_CXX_LIB_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
      FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${_REL_PATH}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    unset(_REL_PATH)
  endif()

  add_library(${CMAKE_PROJECT_NAME}::${KOSY_CXX_LIB_NAME} ALIAS ${KOSY_CXX_LIB_NAME})
endfunction()

function(kosy_cxx_module)
  cmake_parse_arguments(KOSY_CXX_MODULE
    "DISABLE_INSTALL;PUBLIC;TESTONLY"
    "NAME"
    "IXXS;SRCS;COPTS;DEFINES;LINKOPTS;DEPS"
    ${ARGN}
  )

  add_library(${KOSY_CXX_MODULE_NAME} "")
  target_sources(${KOSY_CXX_MODULE_NAME}
    PUBLIC
      FILE_SET CXX_MODULES
        TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${KOSY_CXX_MODULE_IXXS}
  )
  target_include_directories(${KOSY_CXX_MODULE_NAME}
    PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_DIR}/modules>"
  )
  target_compile_options(${KOSY_CXX_MODULE_NAME} PRIVATE ${KOSY_CXX_MODULE_COPTS})
  target_compile_definitions(${KOSY_CXX_MODULE_NAME} PUBLIC ${KOSY_CXX_MODULE_DEFINES})
  target_link_options(${KOSY_CXX_MODULE_NAME} PRIVATE ${KOSY_CXX_MODULE_LINKOPTS})
  target_link_libraries(${KOSY_CXX_MODULE_NAME} PUBLIC ${KOSY_CXX_MODULE_DEPS})

  if(KOSY_ENABLE_INSTALL)
    file(RELATIVE_PATH _REL_PATH "${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
    install(TARGETS ${KOSY_CXX_MODULE_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
      FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/${_REL_PATH}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    unset(_REL_PATH)
  endif()

  add_library(${CMAKE_PROJECT_NAME}::${KOSY_CXX_MODULE_NAME} ALIAS ${KOSY_CXX_MODULE_NAME})
endfunction()

function(kosy_nb_library)
  cmake_parse_arguments(KOSY_NB_LIB
    "DISABLE_INSTALL"
    "NAME"
    "SRCS;COPTS;DEFINES;LINKOPTS;DEPS"
    ${ARGN}
  )

  nanobind_add_module(${KOSY_NB_LIB_NAME} ${KOSY_NB_LIB_SRCS})
  target_include_directories(${KOSY_NB_LIB_NAME}
    PRIVATE
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
  )
  target_compile_options(${KOSY_NB_LIB_NAME} PRIVATE ${KOSY_NB_LIB_COPTS})
  target_compile_definitions(${KOSY_NB_LIB_NAME} PRIVATE ${KOSY_NB_LIB_DEFINES})
  target_link_options(${KOSY_NB_LIB_NAME} PRIVATE ${KOSY_NB_LIB_LINKOPTS})
  target_link_libraries(${KOSY_NB_LIB_NAME} PRIVATE ${KOSY_NB_LIB_DEPS})

  set_property(TARGET ${KOSY_NB_LIB_NAME} PROPERTY FOLDER ${KOSY_IDE_FOLDER}/python)

  if(KOSY_ENABLE_INSTALL AND NOT KOSY_NB_LIB_DISABLE_INSTALL)
    install(TARGETS ${KOSY_NB_LIB_NAME} LIBRARY DESTINATION .)
  endif()

  add_library(${CMAKE_PROJECT_NAME}::${KOSY_NB_LIB_NAME} ALIAS ${KOSY_NB_LIB_NAME})
endfunction()

function(kosy_cxx_test)
  if(NOT (BUILD_TESTING AND KOSY_BUILD_TESTING))
    return()
  endif()

  cmake_parse_arguments(KOSY_CXX_TEST
    ""
    "NAME"
    "SRCS;COPTS;DEFINES;LINKOPTS;DEPS"
    ${ARGN}
  )

  add_executable(${KOSY_CXX_TEST_NAME} ${KOSY_CXX_TEST_SRCS})
  set_property(TARGET ${KOSY_CXX_TEST_NAME} PROPERTY FOLDER ${KOSY_IDE_FOLDER}/tests)
  target_include_directories(${KOSY_CXX_TEST_NAME}
    PRIVATE
      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  )
  target_compile_options(${KOSY_CXX_TEST_NAME} PRIVATE ${KOSY_CXX_TEST_COPTS})
  target_compile_definitions(${KOSY_CXX_TEST_NAME} PRIVATE ${KOSY_CXX_TEST_DEFINES})
  target_link_options(${KOSY_CXX_TEST_NAME} PRIVATE ${KOSY_CXX_TEST_LINKOPTS})
  target_link_libraries(${KOSY_CXX_TEST_NAME}
    PRIVATE
      cxxopts::cxxopts
      doctest::doctest
      Matplot++::matplot
      ${KOSY_CXX_TEST_DEPS}
  )

  add_test(NAME ${KOSY_CXX_TEST_NAME} COMMAND ${KOSY_CXX_TEST_NAME})
endfunction()
