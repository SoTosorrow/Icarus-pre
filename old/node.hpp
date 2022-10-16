#pragma once
#include<unordered_map>
#include<any>
#include<memory>
#include"imgui.h"
#include"utils.hpp"
#include"node_socket.hpp"

struct NodeSocket;
struct Node : public std::enable_shared_from_this<Node> {
    int id;
    ImVec2 pos;
    ImVec2 size = ImVec2(150, 60);
    std::string name;
    std::shared_ptr<Context> context;
    ElementType element_type = ElementType::ElementNode;

    int input_sockets_num = 3;
    int output_sockets_num = 3;
    std::unordered_map<std::string, std::unique_ptr<NodeSocket>> input_sockets;
    std::unordered_map<std::string, std::unique_ptr<NodeSocket>> output_sockets;
    // std::unordered_map<std::string, std::any> inputs;

    Node(std::shared_ptr<Context> _context, const std::string &_name, ImVec2 _pos);
    void setSocketsNum(int _input_sockets_num, int _output_sockets_num){
        this->input_sockets_num = _input_sockets_num;
        this->output_sockets_num = _output_sockets_num;
    }
    void setNodeSize(ImVec2 _size){
        this->size = _size;
    }
    
    void init();
    void draw();
    void drawDebug(ImDrawList* draw_list, ImVec2 node_start_pos);
    void update(){}

    ImU32 node_body_color = IM_COL32(150, 150, 150, 150);
    bool is_debug_mode = true;

};