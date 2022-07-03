cmake_minimum_required(VERSION 3.16)

project(Dictionary 
    VERSION 0.1.0
    LANGUAGES CXX)

# Use Ctest
include(CTest)

# enable use of googletest and google-benchmark
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(BENCHMARK_ENABLE_TESTING NO)
FetchContent_Declare(
    googlebench
    URL https://github.com/google/benchmark/archive/refs/tags/v1.6.1.zip
)
FetchContent_MakeAvailable(googlebench)

FetchContent_Declare(
  dbc 
  URL https://github.com/BrightProgress/dbc/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(dbc)

add_subdirectory(Dictionary)
