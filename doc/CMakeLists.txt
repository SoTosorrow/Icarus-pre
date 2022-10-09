cmake_minimum_required(VERSION 3.0)
project(graph)

add_library(
    imgui STATIC
    imconfig.h
    imgui.h
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    
    imgui_impl_glfw.h
    imgui_impl_glfw.cpp
    imgui_impl_opengl3_loader.h
    imgui_impl_opengl3.h
    imgui_impl_opengl3.cpp

)