cmake_minimum_required(VERSION 3.12)
project(
  m2tp-e2e-leader
  VERSION 1.0.0
  LANGUAGES CXX
)

# Compile output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out")

# Executable source code
add_executable(
  ${PROJECT_NAME}
  "main.cpp"
)

# Link to glue and M2TP Shared Library Object (.so or .dll)
target_link_libraries(
  ${PROJECT_NAME}
  "m2tp-leader"
)

# Include M2TP headers
include_directories(
  ${PROJECT_NAME} PUBLIC
  "../../include/"
)

# Include glue headers
include_directories(
  ${PROJECT_NAME} PUBLIC
  "../glue/include/"
)
