include(GoogleTest)

macro(package_add_test_with_libraries TESTNAME FILES LIBRARIES)
    add_executable(${TESTNAME} ${FILES})
    target_link_libraries(${TESTNAME} gtest gmock gtest_main ${LIBRARIES})
    gtest_discover_tests(${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER unittest)
endmacro()

package_add_test_with_libraries(DictionaryUtilTests testUtils.cpp dict_cpp dbc)
package_add_test_with_libraries(DictionaryTests testDictionary.cpp dict_cpp dbc)
package_add_test_with_libraries(RequirementBasedTests testDictionaryRequirements.cpp dict_cpp dbc)