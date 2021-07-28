cmake_minimum_required(VERSION 3.0)
project(
  m2tp-e2e-glue
  VERSION 1.0.0
  LANGUAGES C
)

# Compile output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out")

# Source codes
file(GLOB_RECURSE SOURCE_PATH "src/*")
add_library(
  ${PROJECT_NAME} SHARED
  ${SOURCE_PATH}
)

# Headers
include_directories(
  ${PROJECT_NAME} PUBLIC
  "include/"
)

# Link to M2TP Shared Library Object (.so or .dll)
target_link_libraries(
  ${PROJECT_NAME}
  "m2tp-leader"
  "m2tp-member"
)

# Include M2TP headers
include_directories(
  ${PROJECT_NAME} PUBLIC
  "../../include/"
)

# Use libpthread shared library
find_package(Threads REQUIRED)
target_link_libraries(
  ${PROJECT_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
)

# Allow install as Shared Library
install(TARGETS ${PROJECT_NAME})
