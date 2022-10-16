#pragma once
#include <fmt/core.h>
#include <memory>
#include <unordered_map>
#include <any>
#include <functional>
#include "utils.hpp"

#include "node.hpp"
#include "node_link.hpp"
#include "node_socket.hpp"
struct Scene : public std::enable_shared_from_this<Scene> {

    std::shared_ptr<Context> context;

    std::unordered_map<Idtype, std::shared_ptr<Node>> map_nodes;
    std::unordered_map<Idtype, std::shared_ptr<NodeLink>> map_nodelinks;
    std::unordered_map<Idtype, std::shared_ptr<NodeSocket>> map_sockets;
    std::unordered_map<Idtype ,std::any> map_datas;

    bool open_menu = false;

    ImU32 scene_grid_color = IM_COL32(200, 200, 200, 40);
    ImU32 scene_link_color = IM_COL32(200, 200, 100, 255);
    float scene_grid_sz = 64.0f;

    Scene();
    void init();
    void checkNodeLink(std::shared_ptr<NodeSocket> socket) ;
    void addNodeLink();
    void drawNodes();
    void drawNodeLinks();
    /*
        add Node to Scene map
        @param f to set node properties and execute before node init
        @old template<class F>
             auto addNode(F const& f=[](std::shared_ptr<Node> node){}, const std::string& name="hello", ImVec2 pos=ImVec2(300,300))-> std::shared_ptr<Node>;
    */
    auto addNode(
        const std::function<void(std::shared_ptr<Node>)>& f = [](std::shared_ptr<Node> node){}, 
        const std::string& name = "hello", 
        ImVec2 pos=ImVec2(300,300))
            -> std::shared_ptr<Node>; 
    
    void addNodeTest();

    /*
        delelte node and the connected link of socket
        set disable in scene map_nodes/map_nodelinks
    */
    void delNode();
    void drawNodeInputSockets(std::shared_ptr<Node> node);
    void drawNodeOuputSockets(std::shared_ptr<Node> node);
    void drawGrid();
    void executeEvent();
    void debugInfo();
    void sortNodes();

    void Show();
};