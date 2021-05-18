add_executable(
    
    gpugraph_tests 
    
    "source/TestStringUtils.cpp"
    "source/TestGrammar.cpp"
    "source/main.cpp"
)

add_test(NAME gpugraph_tests COMMAND gpugraph_tests)

target_link_libraries(gpugraph_tests PUBLIC gtest)
target_link_libraries(gpugraph_tests PRIVATE lib_gpugraph)
