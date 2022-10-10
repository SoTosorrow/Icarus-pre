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
    LinkMap link_map;
    bool open_menu = false;

    Scene();

    auto addNodeLink() -> std::shared_ptr<NodeLink>;
    void addNode(const std::string &name="hello", ImVec2 pos=ImVec2(300,300));

    void drawGrid();
    void drawNodes();
    void drawNodeInputSockets(std::shared_ptr<Node> node);
    void drawNodeOutputSockets(std::shared_ptr<Node> node);
    void drawNodeLinks();
    // auto checkNodeLink(std::shared_ptr<Node> node, std::unique_ptr<NodeSocket> socket)->std::unique_ptr<NodeSocket> ;
    void checkNodeLink(std::shared_ptr<Node> node, NodeSocket* socket);


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
节点遮挡，选择不到body内部
连线的逻辑，图算法
如何拓展节点
@! 检查link share的数量，确保没有内存泄露和指针死持有
addLink 不允许重复连接


*/