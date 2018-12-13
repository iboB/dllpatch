cmake_minimum_required(VERSION 2.8.1)

project(dllpatch)

if(NOT WIN32)
    message(FATAL_ERROR "dllpatch is a Windows-only program")
endif()

option(DLLPATCH_DEMO "Build dllpatch demo project" OFF)

add_executable(dllpatch dllpatch.cpp)

install(TARGETS dllpatch
    RUNTIME DESTINATION bin
)

if(DLLPATCH_DEMO)
    add_subdirectory(demo)
endif()
