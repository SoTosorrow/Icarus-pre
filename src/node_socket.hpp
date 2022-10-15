#pragma once
#include"imgui.h"
#include"utils.hpp"
#include"node.hpp"
#include<memory>
#include<vector>
#include<list>

/*
@todo 
    list instead of vector
*/
struct Scene;
struct NodeSocket
{
    Idtype id;
    ImVec2 size;
    ImVec2 pos;
    int socket_index;

    std::weak_ptr<Scene> scene;
    std::shared_ptr<Context> context;
    Idtype node_id;

    /* 
        input_socket - 1 link(default) & ouput_socket - N link
        node_link_ids.begin()++ != node_link_ids.end() to judge if only 1 node_link
    */
    std::list<Idtype> node_link_ids;    

    ElementType element_type = ElementType::ElementNodeSocket;
    NodeSocketType socket_type;

    NodeSocket(
        std::weak_ptr<Scene> _scene,
        std::shared_ptr<Context> _context, 
        Idtype _node_id, 
        NodeSocketType _socket_type, 
        int _socket_index);
    void draw();
    void addNodeLink(Idtype node_link_id);
    void delNodeLink(Idtype node_link_id);

    float socket_padding = 10.0f;
    float socket_margin = 15.0f;
    int socket_radius = 5;
    
    ImU32 socket_color = IM_COL32(255, 0, 0, 255);
};
