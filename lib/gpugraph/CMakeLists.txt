add_library(

    lib_gpugraph

    "source/gpugraph/RenderTarget.cpp" 
    "source/gpugraph/Node.cpp"
    "source/gpugraph/NodeState.cpp"
    "source/gpugraph/Context.cpp" 
    "source/gpugraph/Intermediate.cpp"  
    "source/gpugraph/Program.cpp"
    "source/gpugraph/util.cpp"
    "source/gpugraph/SkiaRenderTarget.cpp"
    "source/gpugraph/XmlLoader.cpp"
    
    "source/gpugraph/Style.cpp"
    "source/gpugraph/StyleCompiler.cpp" 
    "source/gpugraph/StyleSelector.cpp"

)

set_property(TARGET lib_gpugraph PROPERTY CXX_STANDARD 20)

target_include_directories(lib_gpugraph PUBLIC source/)

target_include_directories(lib_gpugraph PUBLIC extern/skia)
target_link_libraries(lib_gpugraph PUBLIC skia)

target_link_libraries(lib_gpugraph PUBLIC glad)
target_link_libraries(lib_gpugraph PUBLIC glfw)
target_link_libraries(lib_gpugraph PUBLIC glm)
target_link_libraries(lib_gpugraph PUBLIC lib_css)
target_link_libraries(lib_gpugraph PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(lib_gpugraph PUBLIC tinyxml2)


# target_include_directories(lib_gpugraph PUBLIC extern/skia/out/Release/gen)
