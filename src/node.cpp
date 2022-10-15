#include"node.hpp"
#include"node_socket.hpp"
#include "scene.hpp"
#include<iostream>

Node::Node(
    std::weak_ptr<Scene> _scene,
    std::shared_ptr<Context> _context,
    const std::string &_name,
    ImVec2 _pos)
    :scene(std::move(_scene)),context(_context),name(_name),pos(_pos)
{
    this->id = _context->genId();
    std::cout<<"?"<<_scene.lock()->scene_grid_sz<<std::endl;
}

void Node::setSocketsNum(int _input_sockets_num, int _output_sockets_num){
    this->input_sockets_num = _input_sockets_num;
    this->ouput_sockets_num = _output_sockets_num;
}
void Node::setNodeSize(ImVec2 _size){
    this->size = _size;
}

void Node::init() {
    for(auto i=0;i<this->input_sockets_num;i++){
        std::cout<<&this->scene<<std::endl;
        // fmt::print("{}\n",this->scene.lock()->scene_grid_sz);
        auto socket = std::make_shared<NodeSocket>(this->scene, this->context, this->id, NodeSocketType::Input, i);
        // fmt::print("{}\n",this->scene);
        auto p =this->scene.lock();
        p->map_sockets.insert({socket->id, socket});
    fmt::print("2\n");
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

void Node:: draw() {
    auto draw_list = ImGui::GetWindowDrawList();

    auto node_start_pos = this->pos +              this->context->vp_trans;
    auto node_end_pos   = this->pos + this->size + this->context->vp_trans;
    
    // @debug draw
    if(is_debug_mode){
        this->drawDebug(node_start_pos);
    }

    // draw Node body
    draw_list->AddRectFilled(
        node_start_pos,
        node_end_pos,
        node_body_color
    );
    draw_list->ChannelsMerge();
}

void Node::drawDebug(ImVec2 node_start_pos){
    auto draw_list = ImGui::GetWindowDrawList();

}
