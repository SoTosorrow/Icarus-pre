cmake_minimum_required(VERSION 3.0)
project(graph)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_BUILD_TYPE Release)

add_subdirectory(extern/imgui)

set(MAIN 
    main.cpp
    utils/scene.cpp
    utils/node.cpp
    utils/node_socket.cpp)
    
set(current ${CMAKE_CURRENT_SOURCE_DIR})

if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

find_package(OpenGL   REQUIRED)
find_package(Threads  REQUIRED)
find_package(OpenMP   REQUIRED)
find_package(glfw3    REQUIRED)
find_package(TBB      REQUIRED)
find_package(fmt      REQUIRED)
find_package(range-v3 REQUIRED)

add_executable(a ${MAIN})

include_directories(${current}/extern/glad/include)
include_directories(/usr/local/include/openvdb)
include_directories(/usr/local/include/range)
include_directories(${current}/extern/imgui)


target_link_libraries(a OpenGL)
target_link_libraries(a pthread)
target_link_libraries(a OpenMP::OpenMP_CXX)
target_link_libraries(a glfw)
target_link_libraries(a tbb)
target_link_libraries(a fmt)
target_link_libraries(a /usr/local/lib/libopenvdb.so)
target_link_libraries(a ${current}/extern/imgui/build/libimgui.a)
target_link_libraries(a dl)
