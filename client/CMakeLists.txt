cmake_minimum_required(VERSION 3.1)

project(Server_SHH VERSION 0.1)

find_package(OpenSSL REQUIRED)

set(CMAKE_C_FLAGS "-lm -lpam -lpam_misc")

set(EXEC_CLIENT "client")

message(huy ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(${EXEC_CLIENT} ${SOURCES})

target_link_libraries(${EXEC_CLIENT} OpenSSL::SSL OpenSSL::Crypto)

target_include_directories(${EXEC_CLIENT} PRIVATE headers)
