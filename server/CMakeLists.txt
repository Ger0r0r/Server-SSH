cmake_minimum_required(VERSION 3.1)

project(Server_SHH VERSION 0.1)

#set(CMAKE_C_FLAGS " -lm -lpam -lpam_misc")

find_package(OpenSSL REQUIRED)
#find_package(PAM REQUIRED)

set(EXEC_CLIENT "server")

message(huy ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(${EXEC_CLIENT} ${SOURCES})

target_link_libraries(${EXEC_CLIENT} OpenSSL::SSL OpenSSL::Crypto pam pam_misc)

target_include_directories(${EXEC_CLIENT} PRIVATE headers)
