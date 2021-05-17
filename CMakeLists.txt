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
include_directories(
    extern/skia/out/Release/gen
    extern/skia
)
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

add_subdirectory(lib/gpugraph)
add_subdirectory(lib/gpuhud)

add_subdirectory(tests)

# add_executable(window_create "examples/window/main.cpp" "lib/gpugraph/source/RenderTarget.cpp" "lib/gpugraph/source/Node.h" "lib/gpuhud/Widget.cpp" "lib/gpugraph/source/Context.h" "lib/gpugraph/source/Context.cpp"  "lib/gpuhud/Container.h")
# target_link_libraries(window_create PRIVATE lib_gpuhud glad glm)
