add_executable(

    gallery 
    
    "source/main.cpp"

)

target_link_libraries(gallery PRIVATE lib_gpuhud glad glm)
