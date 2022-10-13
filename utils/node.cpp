#include"node.hpp"

Node::Node(
    std::shared_ptr<Context> _context,
    const std::string &_name,
    ImVec2 _pos)
    :context(_context),name(_name),pos(_pos)
{
    this->id = _context->id++;
}


void Node::init() {
    for(auto i=0;i<this->input_sockets_num;i++){
        this->input_sockets.insert({
            std::to_string(this->context->id),
            std::make_unique<NodeSocket>(this->context, this->weak_from_this(), NodeSocketType::Input, i)
        });
    }
    for(auto i=0;i<this->output_sockets_num;i++){
        this->output_sockets.insert({
            std::to_string(this->context->id),
            std::make_unique<NodeSocket>(this->context, this->weak_from_this(), NodeSocketType::Output, i)
        });
    }
}

void Node:: draw() {
    auto draw_list = ImGui::GetWindowDrawList();

    auto node_start_pos = this->pos + this->context->viewport_translate;
    auto node_end_pos = this->pos + this->size + this->context->viewport_translate;
    
    // draw Node body
    draw_list->AddRectFilled(
        node_start_pos,
        node_end_pos,
        node_body_color
    );

    // @debug draw
    if(is_debug_mode){
        this->drawDebug(draw_list, node_start_pos);
    }

}

void Node::drawDebug(ImDrawList* draw_list, ImVec2 node_start_pos){
    // @debug info
    auto pos = std::to_string(this->pos.x)+":"+std::to_string(this->pos.y);
    ImGui::SetCursorScreenPos(node_start_pos + ImVec2(0,-25));
    ImGui::Text("%s",this->name.c_str());

    ImGui::SetCursorScreenPos(node_start_pos + ImVec2(0,-15));
    ImGui::Text("%s",pos.c_str());
}
