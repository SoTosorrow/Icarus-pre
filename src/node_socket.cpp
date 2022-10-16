#include"scene.hpp"
#include"node_socket.hpp"
#include"node_link.hpp"

//@todo socket index should be increment
//@todo default constructor
NodeSocket::NodeSocket(
    std::weak_ptr<Scene> _scene,
    std::shared_ptr<Context> _context,
    Idtype _node_id,
    NodeSocketType _socket_type,
    int _socket_index
):  scene(_scene),
    context(_context),
    node_id(_node_id),
    socket_type(_socket_type),
    socket_index(_socket_index)
{
    this->id = _context->genId();
    fmt::print("socket get{}\n",this->id);
}

void NodeSocket::draw() {
    auto im_id = ImGui::GetID(this->id.c_str());
    auto draw_list = ImGui::GetWindowDrawList();
    // 加上viewport的全局位置
    auto belong_node = this->scene.lock()->map_nodes[this->node_id];

    this->pos = 
        belong_node->pos 
        + ImVec2(
            (this->socket_type==NodeSocketType::Input)? 0.0f : belong_node->size.x, 
            socket_index * socket_margin + socket_padding) 
        + this->context->vp_trans;
    draw_list->AddCircleFilled(
        this->pos, 
        socket_radius, 
        socket_color
    );

    // Add Socket Item
    auto socket_width = ImVec2(this->socket_radius, this->socket_radius);
    auto socket_start_pos = this->pos - socket_width;
    ImVec2 size = ImGui::CalcItemSize( socket_width*2 , 0.0f, 0.0f);
    ImRect bb(socket_start_pos, socket_start_pos + size);
    ImGui::ItemSize(size);
    ImGui::ItemAdd(bb ,im_id);

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, im_id, &hovered, &held);
}

void NodeSocket::addNodeLink(Idtype node_link_id) {

    this->node_link_ids.push_back(node_link_id);
}

void NodeSocket::delNodeLink(Idtype node_link_id) {
   
    for(auto i=this->node_link_ids.begin(); i!=this->node_link_ids.end(); i++){
        if(*i == node_link_id){
            this->node_link_ids.erase(i);
            this->scene.lock()->map_nodelinks[node_link_id]->enable =false;
        }   
    }
}