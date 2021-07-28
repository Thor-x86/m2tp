cmake_minimum_required(VERSION 3.0)
project(
  m2tp-e2e-member_1
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
  "m2tp-member"
  "m2tp-e2e-glue"
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
