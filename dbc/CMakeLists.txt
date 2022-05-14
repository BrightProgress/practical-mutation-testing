FIND_PACKAGE( Boost COMPONENTS log REQUIRED )

add_definitions(-std=c++11 -Wall)
include_directories(${Boost_INCLUDE_DIR})
link_libraries(${Boost_LOG_LIBRARY})
add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(dbc dbc.cpp dbc.hpp)
target_include_directories(dbc PUBLIC ${PROJECT_SOURCE_DIR}/dbc ${PROJECT_BINARY_DIR}/dbc)
add_subdirectory(unit_test)