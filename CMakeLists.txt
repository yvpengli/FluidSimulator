cmake_minimum_required(VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE D:/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
message(VCPKG_ROOT = "${CMAKE_TOOLCHAIN_FILE}")
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/include)

project(OpenGL)

#find source files
file(GLOB_RECURSE SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h 
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(${PROJECT_NAME} ${INCLUDE_DIR})

find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h")

target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_INCLUDE_DIRS})