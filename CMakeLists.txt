cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(eigens LANGUAGES CXX)

find_package(fmt CONFIG REQUIRED)
find_package(armadillo CONFIG REQUIRED)

function(add_executable_with_properties TARGET_NAME)
  add_executable(${TARGET_NAME} ${ARGN})
  target_link_libraries(${TARGET_NAME} PUBLIC fmt armadillo omp)
  set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 20
                                                  CXX_STANDARD_REQUIRED ON)
  target_compile_options(${TARGET_NAME} PUBLIC -fopenmp)
endfunction()

add_executable_with_properties(eigen eigen.cpp)
add_executable_with_properties(ens ens.cpp)
add_executable_with_properties(mlpack mlpack.cpp)
add_executable_with_properties(movie movie.cpp)
