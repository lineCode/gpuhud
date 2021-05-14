project ("gpuhud")
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

#
# glad:
include_directories(lib_gpuhud PUBLIC "extern/glad/include")
add_subdirectory(extern/glad EXCLUDE_FROM_ALL)

#
# glfw
add_subdirectory(extern/glfw EXCLUDE_FROM_ALL)
#
# glm
add_subdirectory(extern/glm EXCLUDE_FROM_ALL)

add_library(lib_gpugraph
  "src/gpugraph/RenderTarget.cpp" 
  "src/gpugraph/Node.h" 
  "src/gpugraph/Node.cpp" 
  "src/gpugraph/Context.cpp" 
  "src/gpugraph/Context.cpp"
  "src/gpugraph/Intermediate.cpp"  
  "src/gpugraph/Program.cpp"
  "src/gpugraph/util.cpp"
  )
target_include_directories(lib_gpugraph PUBLIC src/)
target_link_libraries(lib_gpugraph PUBLIC glad)
target_link_libraries(lib_gpugraph PUBLIC glfw)
target_link_libraries(lib_gpugraph PUBLIC glm)
set_property(TARGET lib_gpugraph PROPERTY CXX_STANDARD 17)

add_library(lib_gpuhud
  "src/gpuhud/subsystem/GlfwSubsystem.cpp"
  "src/gpuhud/Window.cpp"
  
  "src/gpuhud/Widget.cpp"
  "src/gpuhud/Container.cpp")
target_include_directories(lib_gpuhud PUBLIC src/)
target_link_libraries(lib_gpuhud PUBLIC lib_gpugraph)
set_property(TARGET lib_gpuhud PROPERTY CXX_STANDARD 17)

add_executable(window_create "examples/window/main.cpp" "src/gpugraph/RenderTarget.cpp" "src/gpugraph/Node.h" "src/gpuhud/Widget.cpp" "src/gpugraph/Context.h" "src/gpugraph/Context.cpp" "src/gpugraph/attributes.h" "src/gpuhud/Container.h")
target_link_libraries(window_create PRIVATE lib_gpuhud glad glm)
