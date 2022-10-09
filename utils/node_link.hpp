#pragma once

#include<memory>
#include "utils.hpp"

struct NodeLink
{
    int id;
    int input_node_id;
    int output_node_id;
    int input_socket_id;
    int output_socket_id;

    // NodeLink(const Context &_context)
    NodeLink(std::shared_ptr<Context> _context, 
        int _input_node_id, 
        int _input_socket_id, 
        int _output_node_id, 
        int _output_socket_id)
        :input_node_id(_input_node_id),
        output_node_id(_output_node_id),
        input_socket_id(_input_socket_id),
        output_socket_id(_output_socket_id)
    {
        this->id = _context->id++;
    }
    void draw(){}
};
