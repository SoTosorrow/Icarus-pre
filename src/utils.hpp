#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS // allow math op for ImVec
#include "imgui.h"
#include "imgui_internal.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <fmt/core.h>

/*
@todo 
    Idtype: string_view instead string
*/
// namespace Icarus{

using Idtype = std::string;

//struct Data
struct Context{

    Idtype id = std::to_string(0);
    Idtype input_socket_id = std::to_string(-1);
    Idtype ouput_socket_id = std::to_string(-1);
    Idtype last_selected_node_id = std::to_string(-1);
    bool is_in_link = false;


    // offset of scene
    ImVec2 vp_trans = ImVec2(0.0f, 0.0f);
    ImVec2 vp_scale = ImVec2(1.0f, 1.0f);


    Idtype genId(){
        auto id = std::stoi(this->id);
        id += 1;
        auto new_id = std::to_string(id);
        this->id = new_id;
        return new_id;
    }

    bool ouputNotEmpty(){
        return (this->ouput_socket_id != std::to_string(-1));
    }

    bool selectIsEmpty(){
        return (this->last_selected_node_id == std::to_string(-1));
    }

    void clearSelected(){
        this->last_selected_node_id = std::to_string(-1);
    }

    void clear(){
        this->input_socket_id = std::to_string(-1);
        this->ouput_socket_id = std::to_string(-1);
        this->last_selected_node_id = std::to_string(-1);
    }

    // void debugInfo(){
    //     fmt::print("Debug:Context use_count{}\n", this->shared_from_this().use_count());
        
    // }
};

enum class NodeSocketType{
    Input,
    Ouput,
};

enum class ElementType{
    ElementNode,
    ElementNodeSocket,
    ElementNodeLink,
};

enum class MouseType{
    MouseMoving,
    MouseDown,
    MouseUp,
    MouseDragingLink,
};

template<class T>
concept ShowAble = requires(T a) {
    a.Show();
};

// concept requires t.draw()

// }