#include "scene.hpp"
#include "utils.hpp"
#include "../inherit/node.hpp"

Scene::Scene() {
    this->context = std::make_shared<Context>();
}

void Scene::init(){
    auto lambda = [](std::shared_ptr<Node> node){
        node->setSocketsNum(2,5);
        node->setNodeSize(400, 200);
        node->node_body_color = IM_COL32(200, 200, 150, 200);
    };
    auto node = this->addNode(lambda);
    // lambda();

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
    auto input_socket = map_sockets[this->context->input_socket_id];
    auto ouput_socket = map_sockets[this->context->ouput_socket_id];
    if(input_socket->node_id == ouput_socket->node_id){
        fmt::print("DEBUG: can not link node self\n");
        return;
    }

    if(this->map_sockets[this->context->input_socket_id]->node_link_ids.size() >0) {
        fmt::print("DEBUG: can not link input more than 1\n");
        return;
    }

    auto temp_link = std::make_shared<NodeLink>(
            this->context,
            this->context->input_socket_id,
            this->context->ouput_socket_id
        );

    //@todo NodeLink add node_id
    this->map_sockets[this->context->input_socket_id]->addNodeLink(temp_link->id);
    this->map_sockets[this->context->ouput_socket_id]->addNodeLink(temp_link->id);
    
    this->map_nodelinks.insert({
        temp_link->id,
        std::move(temp_link)
    });

    
}

void Scene::drawNodes() {
    auto draw_list = ImGui::GetWindowDrawList();
    ImGuiIO& io = ImGui::GetIO();

    for(auto &[node_id, node] : map_nodes) {

        if(node->enable == false)
            continue;

        ImGui::PushID(ImGui::GetID(node_id.c_str()));

        // draw socket first to response the event
        draw_list->ChannelsSetCurrent(3);
        this->drawNodeInputSockets(node);
        this->drawNodeOuputSockets(node);
        
        // draw nodes
        draw_list->ChannelsSetCurrent(1);
        node->draw();

        ImGui::PopID();
    }
}

auto Scene::addNode(
    const std::function<void(std::shared_ptr<Node>)>& init_func, 
    const std::string& name, 
    ImVec2 pos) 
    -> std::shared_ptr<Node>{

    auto node = std::make_shared<Node>(this->weak_from_this(),this->context, name, pos);
    auto temp_id = node->id; 
    // lambda to define the node properties
    init_func(node);
    node->init();
    
    this->map_nodes.insert({std::move(temp_id), node});
    return std::move(node);
}

void Scene::drawNodeInputSockets(std::shared_ptr<Node> node){
    auto draw_list = ImGui::GetWindowDrawList();
    for(const auto& [socket_index, socket_id] : node->input_socket_ids) {

        ImGui::PushID(ImGui::GetID(socket_id.c_str()));
        auto socket = this->map_sockets[socket_id];

        socket->draw();

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            this->checkNodeLink(socket);     
            fmt::print("DEBUG: click Socket:{}\n", socket->id); 
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

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            this->checkNodeLink(socket);   
            fmt::print("DEBUG: click Socket:{}\n", socket->id);  
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
    for(auto &[node_link_id,node_link]:map_nodelinks){

        if(node_link->enable == false)
            continue;

        ImGui::PushID(ImGui::GetID(node_link_id.c_str()));

        auto last_pos = 
            this->map_sockets[node_link->input_socket_id]->pos;
        auto current_pos = 
            this->map_sockets[node_link->ouput_socket_id]->pos;

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

void Scene::delNode(){
    if(context->selectIsEmpty()){
        return;
    }
    fmt::print("DEBUG: delete node: {}\n", this->context->last_selected_node_id);
    
    //@todo make node disable instead of erase
    Idtype delete_id = this->context->last_selected_node_id;
    this->context->clearSelected();
    auto delete_node = map_nodes[delete_id];

    /*
        delete from scene-map
        delete from socket-map
        delete from connect-socket
            auto conn_socket_input_id = this->map_nodelinks[*i]->input_socket_id;
            auto conn_socket_ouput_id = this->map_nodelinks[*i]->ouput_socket_id;
            fmt::print("{},{}\n", conn_socket_input_id,conn_socket_ouput_id);
    */
    for(const auto& [index, socket_id]: delete_node->ouput_socket_ids){
        for(auto i=this->map_sockets[socket_id]->node_link_ids.begin();
                i!=this->map_sockets[socket_id]->node_link_ids.end();
                i)
        {
            this->map_nodelinks[*i]->enable = false;

            //@todo for more-in input_socket this should be same delete method like ouput_socket_id
            auto conn_socket_input_id = this->map_nodelinks[*i]->input_socket_id;
            this->map_sockets[conn_socket_input_id]->node_link_ids = {};
            // for(auto j=this->map_sockets[conn_socket_input_id]->node_link_ids.begin();
            //     j!=this->map_sockets[conn_socket_input_id]->node_link_ids.end();
            //     j++)
            // {
            // if(*j == *i){
            //     std::list<Idtype>::iterator delete_j = j;
            //     j++;
            //     this->map_sockets[conn_socket_input_id]->node_link_ids.erase(delete_j);
            //     break;
            // }
            // }

            std::list<Idtype>::iterator delete_i = i;
            i++;
            this->map_sockets[socket_id]->node_link_ids.erase(delete_i);

        }

    }
    for(const auto& [index, socket_id]: delete_node->input_socket_ids){

        for(auto i=this->map_sockets[socket_id]->node_link_ids.begin();
                i!=this->map_sockets[socket_id]->node_link_ids.end();
                i)
        {
            this->map_nodelinks[*i]->enable = false;

            auto conn_socket_ouput_id = this->map_nodelinks[*i]->ouput_socket_id;
            for(auto j=this->map_sockets[conn_socket_ouput_id]->node_link_ids.begin();
                j!=this->map_sockets[conn_socket_ouput_id]->node_link_ids.end();
                j++)
            {
            if(*j == *i){
                std::list<Idtype>::iterator delete_j = j;
                j++;
                this->map_sockets[conn_socket_ouput_id]->node_link_ids.erase(delete_j);
                break;
            }
            }

            std::list<Idtype>::iterator delete_i = i;
            i++;
            this->map_sockets[socket_id]->node_link_ids.erase(delete_i);
        }
    }

    delete_node->enable = false;
    // this->nodes.erase(std::to_string(delete_id));
}

void Scene::addNodeTest() {
    auto nodeb = std::make_shared<NodeB>(this->weak_from_this(),this->context, "test", ImGui::GetMousePos());
    auto temp_id = nodeb->id; 
    // lambda to define the node properties
    nodeb->init();
    
    this->map_nodes.insert({std::move(temp_id), nodeb});
}


void Scene::executeEvent() {
    ImGuiIO& io = ImGui::GetIO();
    // Scene Events

    // right click to add Node
    if (ImGui::IsMouseClicked(1)){
        auto lambda = [](std::shared_ptr<Node> node){};
        this->addNode(std::bind_front(lambda), "hello", ImGui::GetMousePos());
        // this->open_menu = !this->open_menu;
    }
    if(ImGui::IsKeyPressed(ImGuiKey_Q)) {
        fmt::print("test\n");
        this->debugInfo();
    }
    if(ImGui::IsKeyPressed(ImGuiKey_W)) {
        fmt::print("DEBUG: SortNodes ing...\n");
        this->sortNodes();
    }
    if(ImGui::IsKeyPressed(ImGuiKey_E)){
        this->addNodeTest();
    }

    if(ImGui::IsKeyPressed(ImGuiKey_D)) {
        this->delNode();
    }
    if(this->open_menu){
        ImGui::OpenPopup("context_menu");
    }
    // ????????????
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f)){
        this->context->vp_trans = this->context->vp_trans + io.MouseDelta;
    }
}

void Scene::debugInfo() {
    fmt::print("SCENE DATA STATUS:\n");
    fmt::print("    node number:{}\n",this->map_nodes.size());
    fmt::print("    sock number:{}\n",this->map_sockets.size());
    fmt::print("    link number:{}\n",this->map_nodelinks.size());
    for(auto const &[a,node] : this->map_nodes) {
        if(node->enable)
            fmt::print("node_id:{} node_use:{}\n",node->id,node.use_count());
    }
    for(auto const &[a,link] : this->map_nodelinks) {
        if(link->enable){
            fmt::print("link_id:{} link_use:{}\n",link->id,link.use_count());
        }
    }
    

}

void Scene::Show() {
    ImGui::Begin("hello");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    auto draw_list = ImGui::GetWindowDrawList();

    /*
        draw layer bigger>show-first
        link 0
        node 1
        cont 2
        sock 3
    */
    draw_list->ChannelsSplit(4);

    this->drawGrid();
    this->drawNodes();

    draw_list->ChannelsSetCurrent(0);
    this->drawNodeLinks();
    this->executeEvent();

    draw_list->ChannelsMerge();


    ImGui::End();
};

void Scene::sortNodes(){
    fmt::print("DEBUG: nodes size: {}\n", map_nodes.size());

    // init compute_dep
    fmt::print("EXEC: nodes init\n");
    for(auto& [id, node]:map_nodes){
        node->compute_input_dep = 0;
        for(const auto& [index,socket_id]:node->input_socket_ids){
            node->compute_input_dep += map_sockets[socket_id]->node_link_ids.size();
        }
        // fmt::print("STAT: node {}:{}\n",node->id,node->compute_input_dep);
    }
    fmt::print("EXEC: nodes init DONE\n");

    // init compute_list
    fmt::print("EXEC: nodes prepare\n");
    for(auto& [id, node]:map_nodes){
        if(node->enable && node->compute_input_dep==0)
            this->compute_queue.push(node);
    }
    fmt::print("EXEC: nodes prepare DONE\n");

    // execute node and search new node prepared
    fmt::print("EXEC: nodes execute\n");
    while(!this->compute_queue.empty()){
        auto execute_node = this->compute_queue.front();
        this->compute_queue.pop();

        execute_node->update();

        for(auto [socket_index, socket_id]: execute_node->ouput_socket_ids){
            for(auto i = this->map_sockets[socket_id]->node_link_ids.begin();
                     i!= this->map_sockets[socket_id]->node_link_ids.end();
                     i++
            ){
                auto input_id = this->map_nodelinks[*i]->input_socket_id;
                auto search_node_id = this->map_sockets[input_id]->node_id;
                auto search_node = this->map_nodes[search_node_id];
                search_node->compute_input_dep -= 1;
                if(search_node->compute_input_dep == 0)
                {
                    this->compute_queue.push(search_node);
                }
            }
        }
    }
    fmt::print("EXEC: nodes execute DONE\n");


}