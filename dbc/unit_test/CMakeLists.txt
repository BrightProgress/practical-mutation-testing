FIND_PACKAGE( Boost 1.54 COMPONENTS log REQUIRED )

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIR})
include_directories(..)
link_directories(${gtest_BINARY_DIR}/src)
link_libraries(gtest_main)
link_libraries(${Boost_LOG_LIBRARY})
link_libraries(dbc)

add_definitions(-DBOOST_LOG_DYN_LINK)
add_definitions(-std=c++11 -Wall)
add_definitions(-DDBC_ENABLED)
add_executable(dbc_unit  dbcunit.cpp)

add_test(dbcUnit dbc_unit)
