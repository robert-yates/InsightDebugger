
cmake_minimum_required(VERSION 3.14)

project(tl)

if(POLICY CMP0135)
	cmake_policy(SET CMP0135 NEW)
	set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

include(FetchContent)

FetchContent_Declare(
  expected
  GIT_REPOSITORY https://github.com/TartanLlama/expected.git
  GIT_TAG        3f0ca7b19253129700a073abfa6d8638d9f7c80c
)

FetchContent_MakeAvailable(expected)

if(NOT TARGET expected)
  add_library(expected INTERFACE)
  target_include_directories(expected INTERFACE ${expected_SOURCE_DIR}/include)
  target_include_directories(tl PUBLIC ${expected_SOURCE_DIR}/include)
endif()
