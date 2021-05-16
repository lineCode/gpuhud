project ("gpuhud")
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

# OpenGL
find_package(OpenGL REQUIRED)

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

#
# skia (precompiled)
link_directories(extern/skia/out/Release)

#
# tinyxml2 (is there a better option?)
add_library(tinyxml2 "extern/tinyxml2/tinyxml2.cpp")
target_include_directories(tinyxml2 PUBLIC extern/tinyxml2/)

#
# googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(extern/googletest)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()


add_library(lib_gpugraph
  "src/gpugraph/RenderTarget.cpp" 
  "src/gpugraph/Node.cpp"
  "src/gpugraph/NodeState.cpp"
  "src/gpugraph/Context.cpp" 
  "src/gpugraph/Context.cpp"
  "src/gpugraph/Intermediate.cpp"  
  "src/gpugraph/Program.cpp"
  "src/gpugraph/util.cpp"
  "src/gpugraph/SkiaRenderTarget.cpp" "src/gpugraph/XmlLoader.cpp" "src/gpugraph/NodeHash.cpp" "src/gpugraph/Selector.cpp")
target_include_directories(lib_gpugraph PUBLIC extern/skia)
target_include_directories(lib_gpugraph PUBLIC extern/skia/out/Release/gen)
target_include_directories(lib_gpugraph PUBLIC src/)
target_link_libraries(lib_gpugraph PUBLIC glad)
target_link_libraries(lib_gpugraph PUBLIC glfw)
target_link_libraries(lib_gpugraph PUBLIC glm)
target_link_libraries(lib_gpugraph PUBLIC skia)
target_link_libraries(lib_gpugraph PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(lib_gpugraph PUBLIC tinyxml2)
set_property(TARGET lib_gpugraph PROPERTY CXX_STANDARD 17)

add_library(lib_gpuhud
  "src/gpuhud/subsystem/GlfwSubsystem.cpp"
  "src/gpuhud/Window.cpp"
  "src/gpuhud/ChildWindow.cpp"
  "src/gpuhud/Widget.cpp"
  "src/gpuhud/Container.cpp")
target_include_directories(lib_gpuhud PUBLIC src/)
target_link_libraries(lib_gpuhud PUBLIC lib_gpugraph)
set_property(TARGET lib_gpuhud PROPERTY CXX_STANDARD 17)

add_subdirectory(tests)

add_executable(window_create "examples/window/main.cpp" "src/gpugraph/RenderTarget.cpp" "src/gpugraph/Node.h" "src/gpuhud/Widget.cpp" "src/gpugraph/Context.h" "src/gpugraph/Context.cpp"  "src/gpuhud/Container.h")
target_link_libraries(window_create PRIVATE lib_gpuhud glad glm)
