cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(whale LANGUAGES CXX)

find_package(armadillo CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH
    SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

find_package(fmt)
if(NOT fmt_FOUND)
  message("fmt not found, using cpm to download fmt")
  include(cmake/CPM.cmake)
  cpmaddpackage(
    NAME
    fmt
    GITHUB_REPOSITORY
    fmtlib/fmt
    VERSION
    10.1.1
    OPTIONS
    "FMT_TEST OFF"
    "FMT_DOC OFF"
    "FMT_INSTALL OFF")

endif()

find_package(spdlog)
if(NOT spdlog_FOUND)
  message("spdlog not found, using cpm to download spdlog")
  cpmaddpackage(
    NAME
    spdlog
    GITHUB_REPOSITORY
    gabime/spdlog
    VERSION
    1.12.0
    OPTIONS
    "SPDLOG_BUILD_EXAMPLE OFF"
    "SPDLOG_BUILD_TESTS OFF"
    "SPDLOG_BUILD_BENCH OFF"
    "SPDLOG_BUILD_TESTS_HO OFF")
endif()

cpmaddpackage("gh:nlohmann/json@3.11.2")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES
                                         "GNU|Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined,leak)")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")
endif()

function(add_executable_with_properties TARGET_NAME)
  add_executable(${TARGET_NAME} ${ARGN})
  target_include_directories(
    ${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third-party/CXXGraph/include
                          include)
  target_link_libraries(
    ${TARGET_NAME} PUBLIC fmt::fmt armadillo omp nlohmann_json::nlohmann_json
                          spdlog)
  set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 20
                                                  CXX_STANDARD_REQUIRED ON)

  # if compiler is gcc or clang then add -march=native
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${TARGET_NAME} PUBLIC -fopenmp -march=native)
  endif()
endfunction()

add_executable_with_properties(ens source/ens.cpp)
add_executable_with_properties(mlpack source/mlpack.cpp)
add_executable_with_properties(movie source/movie.cpp)
add_executable_with_properties(arma source/arma.cpp)
add_executable_with_properties(nlgraph source/nlgraph.cpp)

option(ENABLE_TEST "Enable test" ON)

if(ENABLE_TEST)
  enable_testing()

  find_package(doctest)
  if(NOT doctest_FOUND)
    message("doctest not found, using cpm to download doctest")
    cpmaddpackage(
      NAME
      doctest
      GITHUB_REPOSITORY
      onqtam/doctest
      VERSION
      2.4.11
      OPTIONS
      "DOCTEST_WITH_TESTS OFF"
      "DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF")
  endif()

  add_subdirectory(tests/test_nlgraph)
endif()
