#include "scene.hpp"
#include "utils.hpp"

#include<iostream>

Scene::Scene() {
    this->context = std::make_shared<Context>();
}

void Scene::init(){
     std::cout<<"test"<<std::endl;
     std::cout<<this->scene_grid_sz<<std::endl;
    std::cout<<this->weak_from_this().lock()->scene_grid_sz<<std::endl;
    this->addNode();
}

void Scene::checkNodeLink(std::shared_ptr<NodeSocket> socket) 
{
    // this->context->last_click_node = node->id;
    if(socket->socket_type == NodeSocketType::Input) {
        this->context->input_socket_id = socket->id;

        if(this->context->ouputNotEmpty()) {
            this->addNodeLink();
        }
    }
    if(socket->socket_type == NodeSocketType::Ouput) {
        this->context->ouput_socket_id = socket->id;
    }
}

void Scene::addNodeLink(){
    auto temp_link = std::make_shared<NodeLink>(
            this->context,
            this->context->input_socket_id,
            this->context->ouput_socket_id
        );
    this->map_nodelinks.insert({
        this->context->genId(),
        std::move(temp_link)
    });
}

void Scene::drawNodes() {
    auto draw_list = ImGui::GetWindowDrawList();
    ImGuiIO& io = ImGui::GetIO();

    for(auto &[node_id, node] : map_nodes) {
        ImGui::PushID(ImGui::GetID(node_id.c_str()));
        // draw nodes
        node->draw();
        this->drawNodeInputSockets(node);
        this->drawNodeOuputSockets(node);


        // 变化坐标系
        ImGui::SetCursorScreenPos(node->pos + this->context->vp_trans);
        // 建立一个虚拟按钮充当节点的Item
        ImGui::InvisibleButton("node", node->size);
        // 点击事件
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
        {
            fmt::print("click Node:{}\n", node->id);
            this->context->last_selected_node_id = node->id;
        }
        // 拖拽事件
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            node->pos = node->pos + io.MouseDelta;
        }

        ImGui::PopID();
    }
}

void Scene::addNode(const std::string& name, ImVec2 pos){
    auto temp_id = this->context->genId();
    auto node = std::make_shared<Node>(this->weak_from_this(),this->context, name, pos);
    node->init();
    this->map_nodes.insert({std::move(temp_id), std::move(node)});
}

void Scene::drawNodeInputSockets(std::shared_ptr<Node> node){
    auto draw_list = ImGui::GetWindowDrawList();
    for(const auto& [socket_index, socket_id] : node->input_socket_ids) {
        ImGui::PushID(ImGui::GetID(socket_id.c_str()));

        auto socket = this->map_sockets[socket_id];

        socket->draw();

        // socket visible button
        auto socket_width = ImVec2(socket->socket_radius, socket->socket_radius);
        ImGui::SetCursorScreenPos(socket->pos - socket_width);
        ImGui::InvisibleButton("socket", socket_width*2);

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            this->checkNodeLink(socket);      
        }
        // for test
        if (ImGui::IsItemHovered())
        {
            draw_list->AddRectFilled(socket->pos - ImVec2(5.0f,5.0f), socket->pos + ImVec2(10.0f,10.0f), IM_COL32(0,255,0,255));
        }

        ImGui::PopID();
    }
}

void Scene::drawNodeOuputSockets(std::shared_ptr<Node> node){
    auto draw_list = ImGui::GetWindowDrawList();
    for(const auto& [socket_index, socket_id] : node->ouput_socket_ids) {
        ImGui::PushID(ImGui::GetID(socket_id.c_str()));

        auto socket = this->map_sockets[socket_id];

        socket->draw();

        // socket visible button
        auto socket_width = ImVec2(socket->socket_radius, socket->socket_radius);
        ImGui::SetCursorScreenPos(socket->pos - socket_width);
        ImGui::InvisibleButton("socket", socket_width*2);

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            this->checkNodeLink(socket);      
        }
        // for test
        if (ImGui::IsItemHovered())
        {
            draw_list->AddRectFilled(socket->pos - ImVec2(5.0f,5.0f), socket->pos + ImVec2(10.0f,10.0f), IM_COL32(0,255,0,255));
        }

        ImGui::PopID();
    }
}

void Scene::drawGrid() {
    // draw Grid
    auto draw_list = ImGui::GetWindowDrawList();
    
    ImVec2 win_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetWindowSize();
    for (float x = fmodf(this->context->vp_trans.x, scene_grid_sz); x < canvas_sz.x; x += scene_grid_sz)
        draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, scene_grid_color);
    for (float y = fmodf(this->context->vp_trans.y, scene_grid_sz); y < canvas_sz.y; y += scene_grid_sz)
        draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, scene_grid_color);
}

void Scene::Show() {
    ImGui::Begin("hello");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    this->drawGrid();
    this->drawNodes();
    // this->drawNodeLinks();
    // this->executeEvent();


    ImGui::End();
};