cmake_minimum_required(VERSION 3.0)
project(z1_sdk)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread")

set(EIGEN_PATH /usr/include/eigen3)
include_directories(
    include
    ${EIGEN_PATH}
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

link_directories(lib)

add_executable(highcmd_basic examples/highcmd_basic.cpp )
target_link_libraries(highcmd_basic Z1_SDK_Linux64)
add_executable(highcmd_development examples/highcmd_development.cpp )
target_link_libraries(highcmd_development Z1_SDK_Linux64)
add_executable(lowcmd_development examples/lowcmd_development.cpp )
target_link_libraries(lowcmd_development Z1_SDK_Linux64)

add_executable(test1 examples/test1.cpp )
target_link_libraries(test1 Z1_SDK_Linux64)

add_executable(armTest1 examples/armTest1.cpp )
target_link_libraries(armTest1 Z1_SDK_Linux64)

add_executable(soiltest1 examples/soiltest1.cpp )
target_link_libraries(soiltest1 Z1_SDK_Linux64)

add_executable(soiltest examples/soiltest.cpp )
target_link_libraries(soiltest Z1_SDK_Linux64)

add_executable(backToStart examples/backToStart.cpp )
target_link_libraries(backToStart Z1_SDK_Linux64)

add_executable(comparisionArm examples/comparisionArm.cpp )
target_link_libraries(comparisionArm Z1_SDK_Linux64)