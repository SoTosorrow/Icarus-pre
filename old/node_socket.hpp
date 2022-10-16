#pragma once
#include"imgui.h"
#include"utils.hpp"
#include"node.hpp"
#include<memory>


struct Node;
struct NodeSocket
{
    int id;
    int socket_index;
    ImVec2 size;
    ImVec2 pos;


    ElementType element_type = ElementType::ElementNodeSocket;
    NodeSocketType socket_type;

    std::shared_ptr<Context> context;
    std::weak_ptr<Node> node;

    NodeSocket(std::shared_ptr<Context> _context, std::weak_ptr<Node> _node, NodeSocketType _socket_type, int _socket_index);
    void draw();


    float socket_padding = 10.0f;
    float socket_margin = 15.0f;
    int socket_radius = 5;
    
    ImU32 socket_color = IM_COL32(255, 0, 0, 255);
};
