#pragma once
#include "imgui.h"
// #include<mutex>

// namespace graph {


static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2 operator/(const ImVec2& lhs, const int& v) { return ImVec2(lhs.x/v, lhs.y/v); }
static inline ImVec2 operator*(const ImVec2& lhs, const int& v) { return ImVec2(lhs.x*2, lhs.y*2); }

struct Context{
    int id=0;
    ImVec2 viewport_translate =ImVec2(0.0f, 0.0f);
    ImVec2 viewport_scale;

    int input_node_id = -1;
    int input_socket_id = -1;
    int output_node_id = -1;
    int output_socket_id = -1;
    int last_click_node = -1;
    int link_state=0;   // 0无状态，1连线中
    // int safeAddId() {
    //     std::mutex m;
    //     m.lock();
    //     this->id++;
    //     yield_co id;
    //     m.unlock();
    // }

    void clear(){
        this->input_node_id = -1;
        this->input_socket_id = -1;
        this->output_node_id = -1;
        this->output_socket_id = -1;
        int last_click_node = -1;
    }

};

enum class NodeSocketType{
    Input,
    Output,
};

// enum class DataType{

// };

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


// }