cmake_minimum_required(VERSION 3.14)
project(
  m2tp
  VERSION 1.0.0
  LANGUAGES C CXX
)

# Disable non ANSI C related warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")

# Compile output directory (./out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out")

# M2TP Common Sources
file(GLOB_RECURSE M2TP_COMMON_SOURCE_PATH "src/common/*")
list(FILTER M2TP_COMMON_SOURCE_PATH EXCLUDE REGEX ".+\\.test\\.cpp$")

# M2TP Member Shared Library
file(GLOB_RECURSE M2TP_MEMBER_SOURCE_PATH "src/member/*")
list(FILTER M2TP_MEMBER_SOURCE_PATH EXCLUDE REGEX ".+\\.test\\.cpp$")
add_library(
  ${PROJECT_NAME}-member SHARED
  ${M2TP_COMMON_SOURCE_PATH}
  ${M2TP_MEMBER_SOURCE_PATH}
)
include_directories(
  ${PROJECT_NAME}-member PUBLIC
  "include/"
)

# M2TP Leader Shared Library
file(GLOB_RECURSE M2TP_LEADER_SOURCE_PATH "src/leader/*")
list(FILTER M2TP_LEADER_SOURCE_PATH EXCLUDE REGEX ".+\\.test\\.cpp$")
add_library(
  ${PROJECT_NAME}-leader SHARED
  ${M2TP_COMMON_SOURCE_PATH}
  ${M2TP_LEADER_SOURCE_PATH}
)
include_directories(
  ${PROJECT_NAME}-leader PUBLIC
  "include/"
)

# Allow only M2TP Shared Library to be installed
install(
  TARGETS
    ${PROJECT_NAME}-member
    ${PROJECT_NAME}-leader
)

# Also install the headers from `./include` for other projects
install(
  DIRECTORY "include/"
  TYPE INCLUDE
)

#####################################################
#       Unit Test with Google Test Framework        #
#####################################################

# Download and unpack googletest at configure time
configure_file(CMakeLists.unit.test.txt googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(WARNING "Cannot download GoogleTest with error: ${result}\r\nYou can't run unit test for now until you're online")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(WARNING "Failed when rebuilding GoogleTest with error: ${result}\r\nPlease check internet connection then delete \"${CMAKE_CURRENT_BINARY_DIR}\" directory")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Test target
file(GLOB_RECURSE M2TP_COMMON_TEST_SOURCE_PATH "src/common/*.test.cpp")
file(GLOB_RECURSE M2TP_MEMBER_TEST_SOURCE_PATH "src/member/*.test.cpp")
file(GLOB_RECURSE M2TP_LEADER_TEST_SOURCE_PATH "src/leader/*.test.cpp")

# Common Unit Test
add_executable(
  ${PROJECT_NAME}-common-test-unit
  ${M2TP_COMMON_SOURCE_PATH}
  ${M2TP_MEMBER_SOURCE_PATH}
  ${M2TP_COMMON_TEST_SOURCE_PATH}
)
include_directories(
  ${PROJECT_NAME}-common-test-unit PUBLIC
  "include/"
)
target_link_libraries(
  ${PROJECT_NAME}-common-test-unit
  gtest_main
)
add_test(
  NAME ${PROJECT_NAME}-common-test-unit
  COMMAND ${PROJECT_NAME}-common-test-unit
)

# Member Unit Test
add_executable(
  ${PROJECT_NAME}-member-test-unit
  ${M2TP_COMMON_SOURCE_PATH}
  ${M2TP_MEMBER_SOURCE_PATH}
  ${M2TP_MEMBER_TEST_SOURCE_PATH}
)
include_directories(
  ${PROJECT_NAME}-member-test-unit PUBLIC
  "include/"
)
target_link_libraries(
  ${PROJECT_NAME}-member-test-unit
  gtest_main
)
add_test(
  NAME ${PROJECT_NAME}-member-test-unit
  COMMAND ${PROJECT_NAME}-member-test-unit
)

# Leader Unit Test
add_executable(
  ${PROJECT_NAME}-leader-test-unit
  ${M2TP_COMMON_SOURCE_PATH}
  ${M2TP_LEADER_SOURCE_PATH}
  ${M2TP_LEADER_TEST_SOURCE_PATH}
)
include_directories(
  ${PROJECT_NAME}-leader-test-unit PUBLIC
  "include/"
)
target_link_libraries(
  ${PROJECT_NAME}-leader-test-unit
  gtest_main
)
add_test(
  NAME ${PROJECT_NAME}-leader-test-unit
  COMMAND ${PROJECT_NAME}-leader-test-unit
)

#####################################################
#                End-to-End Testing                 #
#####################################################

add_subdirectory(
  "test/e2e/glue"
  "${PROJECT_SOURCE_DIR}/test/e2e/glue/.cmake"
)

add_subdirectory(
  "test/e2e/leader"
  "${PROJECT_SOURCE_DIR}/test/e2e/leader/.cmake"
)

add_subdirectory(
  "test/e2e/member_1"
  "${PROJECT_SOURCE_DIR}/test/e2e/member_1/.cmake"
)
