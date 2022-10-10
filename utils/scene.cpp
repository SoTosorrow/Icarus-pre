#include"scene.hpp"

Scene::Scene() {
    this->context = std::make_shared<Context>();
    this->addNode();
}

auto Scene::addNodeLink() -> std::shared_ptr<NodeLink>{
    auto temp_link = std::make_shared<NodeLink>(
            this->context,
            this->context->input_node_id,
            this->context->input_socket_id,
            this->context->output_node_id,
            this->context->output_socket_id
        );
    this->node_links.insert({
        std::to_string(this->context->id),
        temp_link
    });
    this->link_map.insert(temp_link);
    return temp_link;
}

void Scene::addNode(const std::string &name, ImVec2 pos) {
    auto temp_id = std::to_string(this->context->id);
    auto node = std::make_shared<Node>(this->context, name, pos);
    node->init();
    this->nodes.insert({std::move(temp_id), std::move(node)});
}

void Scene::drawGrid() {
    // draw Grid
    auto draw_list = ImGui::GetWindowDrawList();
    
    ImVec2 win_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetWindowSize();
    for (float x = fmodf(this->context->viewport_translate.x, scene_grid_sz); x < canvas_sz.x; x += scene_grid_sz)
        draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, scene_grid_color);
    for (float y = fmodf(this->context->viewport_translate.y, scene_grid_sz); y < canvas_sz.y; y += scene_grid_sz)
        draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, scene_grid_color);
}

void Scene::drawNodes() {
    auto draw_list = ImGui::GetWindowDrawList();
    ImGuiIO& io = ImGui::GetIO();

    for(auto &[node_id, node] : nodes) {
        ImGui::PushID(node->id);
        // draw nodes
        node->draw();
        this->drawNodeInputSockets(node);
        this->drawNodeOutputSockets(node);


        // 变化坐标系
        ImGui::SetCursorScreenPos(node->pos + this->context->viewport_translate);
        // 建立一个虚拟按钮充当节点的Item
        ImGui::InvisibleButton("node", node->size);
        // 点击事件
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
        {
            fmt::print("{}\n", node->id);
            this->context->last_click_node = node->id;
        }
        // 拖拽事件
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            node->pos = node->pos + io.MouseDelta;
        }

        ImGui::PopID();
    }
}

//@todo better methods
// auto Scene::checkNodeLink(std::shared_ptr<Node> node, std::unique_ptr<NodeSocket> socket) 
//     -> std::unique_ptr<NodeSocket>
void Scene::checkNodeLink(std::shared_ptr<Node> node, NodeSocket* socket) 
{
    // this->context->last_click_node = node->id;
    if(socket->socket_type == NodeSocketType::Input) {
        this->context->input_node_id = node->id;
        this->context->input_socket_id = socket->id;

        if(this->context->output_socket_id != -1) {
            this->addNodeLink();
        }
    }
    if(socket->socket_type == NodeSocketType::Output) {
        this->context->output_node_id = node->id;
        this->context->output_socket_id = socket->id;
    }
}

void Scene::drawNodeInputSockets(std::shared_ptr<Node> node){
    auto draw_list = ImGui::GetWindowDrawList();
    for(const auto &[socket_id, socket] : node->input_sockets) {
        ImGui::PushID(socket->id);
        socket->draw();

        // socket visible button 
        auto socket_width = ImVec2(socket->socket_radius, socket->socket_radius);
        ImGui::SetCursorScreenPos(socket->pos - socket_width);
        ImGui::InvisibleButton("socket", socket_width*2);

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            fmt::print("{}\n", socket->id);

            this->checkNodeLink(node,socket.get());      
        }
        // for test
        if (ImGui::IsItemHovered())
        {
            draw_list->AddRectFilled(socket->pos - ImVec2(5.0f,5.0f), socket->pos + ImVec2(10.0f,10.0f), IM_COL32(0,255,0,255));
        }
        
        ImGui::PopID();
    }
}

void Scene::drawNodeOutputSockets(std::shared_ptr<Node> node){
    auto draw_list = ImGui::GetWindowDrawList();
    for(const auto &[socket_id, socket] : node->output_sockets) {
        ImGui::PushID(socket->id);
        socket->draw();

        // socket visible button 
        auto socket_width = ImVec2(socket->socket_radius, socket->socket_radius);
        ImGui::SetCursorScreenPos(socket->pos - socket_width);
        ImGui::InvisibleButton("socket", socket_width*2);

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            fmt::print("{}\n", socket->id);

            this->checkNodeLink(node,socket.get());            
        }
        // for test
        if (ImGui::IsItemHovered())
        {
            draw_list->AddRectFilled(socket->pos - ImVec2(5.0f,5.0f), socket->pos + ImVec2(10.0f,10.0f), IM_COL32(0,255,0,255));
        }
        
        ImGui::PopID();
    }
}

void Scene::drawNodeLinks() {
    auto draw_list = ImGui::GetWindowDrawList();
    for(auto &[node_link_id,node_link]:node_links){
        ImGui::PushID(node_link->id);

        auto last_pos = 
            this->nodes[std::to_string(node_link->input_node_id)]
                ->input_sockets[std::to_string(node_link->input_socket_id)]->pos;
        auto current_pos = 
            this->nodes[std::to_string(node_link->output_node_id)]
                ->output_sockets[std::to_string(node_link->output_socket_id)]->pos;

        draw_list->AddBezierCurve(
            last_pos, 
            last_pos + ImVec2(-50, 0), 
            current_pos + ImVec2(+50, 0), 
            current_pos, 
            scene_link_color, 
            3.0f);
        ImGui::PopID();
    }
}

void Scene::executeEvent() {
    ImGuiIO& io = ImGui::GetIO();
    // Scene Events
    if (ImGui::IsMouseClicked(1)){
        this->addNode("hello", ImGui::GetMousePos());
        this->open_menu = !this->open_menu;
    }
    if(ImGui::IsKeyPressed(ImGuiKey_Q)) {
        fmt::print("test\n");
        this->test();
    }
    if(ImGui::IsKeyPressed(ImGuiKey_E)) {
        fmt::print("test\n");
        this->link_map.print();
    }
    if(ImGui::IsKeyPressed(ImGuiKey_R)) {
        fmt::print("test\n");
        this->link_map.search(0,4);
    }
    if(ImGui::IsKeyPressed(ImGuiKey_W)) {
        fmt::print("clear\n");
        this->context->clear();
    }

    if(ImGui::IsKeyPressed(ImGuiKey_D)) {
        fmt::print("delete node: {}\n", this->context->last_click_node);
        this->nodes.erase(std::to_string(this->context->last_click_node));
        this->context->last_click_node = -1;

        //@todo 删除相关的线
    }
    if(this->open_menu){
        ImGui::OpenPopup("context_menu");
    }
    // 拖曳视图
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f)){
        this->context->viewport_translate = this->context->viewport_translate + io.MouseDelta;
    }
}

void Scene::test() {
    fmt::print("SCENE DATA STATUS:\n");
    fmt::print("link number:{}\n",this->node_links.size());
    for(auto const &[a,node] : this->nodes) {
        fmt::print("context_id:{} node_id:{} node_use:{}\n",a,node->id,node.use_count());
        for(auto const &[b,socket] : node->input_sockets) {
            fmt::print("context_id:{} in_socket_id:{}\n",b,socket->id);
        }
        for(auto const &[b,socket] : node->output_sockets) {
            fmt::print("context_id:{} ou_socket_id:{}\n",b,socket->id);
        }
    }
    for(auto const &[b,link] : this->node_links) {
        fmt::print("context_id:{} link_id:{} link_from:{}/{} link_to:{}/{}\n",
            b,link->id,
            link->input_node_id,link->input_socket_id,
            link->output_node_id,link->output_socket_id);
    }
}

void Scene::Show() {
    ImGui::Begin("hello");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    this->drawGrid();
    this->drawNodes();
    this->drawNodeLinks();
    this->executeEvent();

    ImGui::End();
};