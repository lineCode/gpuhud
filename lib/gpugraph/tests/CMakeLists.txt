add_executable(
    
    gpugraph_tests 
    
    "source/TestStringUtils.cpp"
    "source/TestXmlLoader.cpp"
    
    "source/main.cpp"
    "source/TestStyling.cpp"
 )
set_property(TARGET gpugraph_tests PROPERTY CXX_STANDARD 20)

add_test(NAME gpugraph_tests COMMAND gpugraph_tests)

target_link_libraries(gpugraph_tests PUBLIC gtest)
target_link_libraries(gpugraph_tests PRIVATE lib_gpugraph)
