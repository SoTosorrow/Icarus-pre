#include"node.hpp"
#include"node_socket.hpp"
#include "scene.hpp"

Node::Node(
    std::weak_ptr<Scene> _scene,
    std::shared_ptr<Context> _context,
    const std::string_view &_name,
    ImVec2 _pos)
    :scene(std::move(_scene)),context(_context),name(_name),pos(_pos)
{
    this->id = _context->genId();
    fmt::print("node get{}\n",this->id);
}

void Node::setSocketsNum(int _input_sockets_num, int _output_sockets_num){
    this->input_sockets_num = _input_sockets_num;
    this->ouput_sockets_num = _output_sockets_num;
}
void Node::setNodeSize(int _size_x, int _size_y){
    this->size = ImVec2(_size_x, _size_y);
}

void Node::init() {
    for(auto i=0;i<this->input_sockets_num;i++){
        auto socket = std::make_shared<NodeSocket>(this->scene, this->context, this->id, NodeSocketType::Input, i);
        this->scene.lock()->map_sockets.insert({socket->id, socket});
        this->input_socket_ids.insert({
            i,
            socket->id
        });
    }
    for(auto i=0;i<this->ouput_sockets_num;i++){
        auto socket = std::make_shared<NodeSocket>(this->scene, this->context, this->id, NodeSocketType::Ouput, i);
        this->scene.lock()->map_sockets.insert({socket->id, socket});
        this->ouput_socket_ids.insert({
            i,
            socket->id
        });
    }
}

void Node::draw() {
    
    auto draw_list = ImGui::GetWindowDrawList();
    auto im_id = ImGui::GetID(this->id.c_str());
    ImGuiIO& io = ImGui::GetIO();

    auto node_start_pos = this->pos + this->context->vp_trans;
    
    // @debug draw
    if(is_debug_mode){
        this->drawDebug(node_start_pos);
    }

    /*
        virtual add content
        custom view
        @todo
    */
    draw_list->ChannelsSetCurrent(3);
    ImGui::SetCursorScreenPos(node_start_pos + ImVec2(5,5));
    ImGui::BeginGroup();
    this->fillContent();
    ImGui::EndGroup();
    if (this->resize)
        this->size = ImGui::GetItemRectSize() + ImVec2(10,10);


    // draw Node body
    draw_list->ChannelsSetCurrent(1);
    draw_list->AddRectFilled(
        node_start_pos,
        node_start_pos + this->size,
        node_body_color
    );


    // add Node body Item
    ImVec2 size = ImGui::CalcItemSize(this->size, 0.0f, 0.0f);
    ImRect bb(node_start_pos, node_start_pos + size);
    ImGui::ItemSize(size);
    ImGui::ItemAdd(bb ,im_id);

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, im_id, &hovered, &held);
    
    // response Node basic event
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
        fmt::print("DEBUG: click Node:{}\n", this->id);
        this->context->last_selected_node_id = this->id;
    }
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        this->pos = this->pos + io.MouseDelta;
    }

}

void Node::drawDebug(ImVec2 node_start_pos){
    // auto draw_list = ImGui::GetWindowDrawList();

    // ImGui::SetCursorScreenPos(node_start_pos);
    // ImGui::BeginGroup();
    // ImGui::PushItemWidth(120);

    // ImGui::Text("%s",this->name.c_str());
    
    // ImGui::PopItemWidth();
    // ImGui::EndGroup();

}
