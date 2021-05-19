project ("gpuhud")
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")

#
#-------------------------------------------------------------------------------
#

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

#
# boost (spirit)
include_directories(extern/x3css/extern/boost)

#
#-------------------------------------------------------------------------------
#

#
#libraries
add_subdirectory(extern/x3css/lib/css)
add_subdirectory(extern/x3css/lib/css/tests)

add_subdirectory(lib/gpugraph)
add_subdirectory(lib/gpugraph/tests)

add_subdirectory(lib/gpuhud)

#
# applications
add_subdirectory(app/gallery)
