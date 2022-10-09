#pragma once
#include<vector>
#include<memory>
#include<unordered_map>
#include <math.h>
#include"imgui.h"
#include"utils.hpp"
#include"fmt/core.h"

#include"node.hpp"
#include"node_link.hpp"

struct Scene{
    using SocketMap = std::unordered_map<std::string, std::unique_ptr<NodeSocket>>;

    std::shared_ptr<Context> context;
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
    std::unordered_map<std::string, std::shared_ptr<NodeLink>> node_links;
    bool open_menu = false;

    Scene();

    void addNodeLink();
    void addNode(const std::string &name="hello", ImVec2 pos=ImVec2(300,300));

    void drawGrid();
    void drawNodes();
    void drawNodeInputSockets(std::shared_ptr<Node> node);
    void drawNodeOutputSockets(std::shared_ptr<Node> node);
    void drawNodeLinks();
    auto checkNodeLink(std::shared_ptr<Node> node, std::unique_ptr<NodeSocket> socket)->std::unique_ptr<NodeSocket> ;


    void executeEvent();

    void test();

    void Show();

    ImU32 scene_grid_color = IM_COL32(200, 200, 200, 40);
    ImU32 scene_link_color = IM_COL32(200, 200, 100, 255);
    float scene_grid_sz = 64.0f;
};


/*
@todo
删除节点时删除相关的连线
拖拽视图的错位
缩放视图


*/