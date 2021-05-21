add_library(

    lib_gpuhud

    "source/gpuhud/subsystem/GlfwSubsystem.cpp"
    "source/gpuhud/Window.cpp"
    "source/gpuhud/ChildWindow.cpp"
    "source/gpuhud/Widget.cpp"
    "source/gpuhud/Container.cpp"
    "source/gpuhud/NodeTree.cpp"
    
)

target_include_directories(lib_gpuhud PUBLIC source)
target_link_libraries(lib_gpuhud PUBLIC lib_gpugraph)

set_property(TARGET lib_gpugraph PROPERTY CXX_STANDARD 20)
