cmake_minimum_required(VERSION 3.15)

project(FastQuery)

file(GLOB SRCFILES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRCFILES})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
