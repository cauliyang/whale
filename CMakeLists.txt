cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(whale LANGUAGES CXX)

find_package(fmt CONFIG REQUIRED)
find_package(armadillo CONFIG REQUIRED)

add_subdirectory(third-party/json)
add_subdirectory(third-party/spdlog)
add_subdirectory(third-party/outcome EXCLUDE_FROM_ALL)

function(add_executable_with_properties TARGET_NAME)
  add_executable(${TARGET_NAME} ${ARGN})
  target_link_libraries(
    ${TARGET_NAME} PUBLIC fmt armadillo omp nlohmann_json::nlohmann_json
                          outcome::hl)
  set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 20
                                                  CXX_STANDARD_REQUIRED ON)
  target_compile_options(${TARGET_NAME} PUBLIC -fopenmp)
endfunction()

# if compiler is gcc or clang then add -march=native
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-march=native)
endif()

add_executable_with_properties(ens ens.cpp)
add_executable_with_properties(mlpack mlpack.cpp)
add_executable_with_properties(movie movie.cpp)
add_executable_with_properties(graph graph.cpp)
add_executable_with_properties(arma arma.cpp)
