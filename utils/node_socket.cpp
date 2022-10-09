#pragma once

#include"node_socket.hpp"
#include"fmt/core.h"

//@todo socket index should be increment
//@todo default constructor
NodeSocket::NodeSocket(
    std::shared_ptr<Context> _context,
    std::weak_ptr<Node> _node,
    NodeSocketType _socket_type,
    int _socket_index
):context(_context),
    node(_node),
    socket_type(_socket_type),
    socket_index(_socket_index)
{
    this->id = _context->id++;
    // this->socket_index = this->node.lock()->input_sockets.size()++
}

void NodeSocket::draw() {
    auto draw_list = ImGui::GetWindowDrawList();
    // 加上viewport的全局位置
    this->pos = this->node.lock()->pos + 
        ImVec2(((this->socket_type==NodeSocketType::Input)? 0.0f : node.lock()->size.x), 
        socket_index * socket_margin + socket_padding) + this->context->viewport_translate;
        
    draw_list->AddCircleFilled(
        this->pos, 
        socket_radius, 
        socket_color
    );
}