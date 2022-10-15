#pragma once

#include<memory>
#include<map>
#include "utils.hpp"

struct NodeLink
{
    Idtype id;
    Idtype input_socket_id;
    Idtype ouput_socket_id;
    bool enable = true;
    bool is_check = false;

    NodeLink(std::shared_ptr<Context> _context, 
        Idtype _input_socket_id, 
        Idtype _output_socket_id)
        :input_socket_id(_input_socket_id),
        ouput_socket_id(_output_socket_id)
    {
        this->id = _context->genId();
    }

    void draw(){}
};
