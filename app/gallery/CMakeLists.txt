add_executable(

    gallery 
    
    "source/main.cpp"

)
set_property(TARGET gallery PROPERTY CXX_STANDARD 20)


target_link_libraries(gallery PRIVATE lib_gpuhud glad glm)
