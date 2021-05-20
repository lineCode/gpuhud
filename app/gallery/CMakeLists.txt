add_executable(

    gallery 
    
    "source/main.cpp"

)
set_property(TARGET gallery PROPERTY CXX_STANDARD 20)

add_custom_command(
        TARGET gallery POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/app/gallery/assets/style.css
                ${CMAKE_CURRENT_BINARY_DIR}/style.css)
target_link_libraries(gallery PRIVATE lib_gpuhud glad glm)
