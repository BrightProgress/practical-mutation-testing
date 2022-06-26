# Optionally glob, but only for CMake 3.12 or later:
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Dictionary/include/*.hpp")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Dictionary/src/*.?pp")

add_definitions(-DDBC_ENABLED)

# Make an automatic library - will be static or dynamic based on user setting
add_library(dict_cpp ${SOURCE_LIST} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(dict_cpp PUBLIC ${PROJECT_SOURCE_DIR}/Dictionary/include PRIVATE ${dbc_SOURCE_DIR})
target_link_libraries(dict_cpp dbc)

# All users of this library will need at least C++20
target_compile_features(dict_cpp PUBLIC cxx_std_20)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/Dictionary/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
