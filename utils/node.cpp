#include"node.hpp"
#include "fmt/core.h"
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
    
    // @debug draw
    draw_list->ChannelsSplit(2);
    if(is_debug_mode){
        this->drawDebug(draw_list, node_start_pos);
    }

    // draw Node body
    draw_list->ChannelsSetCurrent(0);//Background
    draw_list->AddRectFilled(
        node_start_pos,
        node_end_pos,
        node_body_color
    );
    draw_list->ChannelsMerge();


}

void Node::drawDebug(ImDrawList* draw_list, ImVec2 node_start_pos){
    // @debug info
    auto pos = std::to_string(this->pos.x)+":"+std::to_string(this->pos.y);

    draw_list->ChannelsSetCurrent(1);//Foreground
    ImGui::SetCursorScreenPos(node_start_pos);
    ImGui::BeginGroup();
    ImGui::PushItemWidth(this->size.x);
    
    ImGui::Text("%s",this->name.c_str());
    static float a;
    static ImVec4 c;
    // float c;
    auto size_str =std::to_string(size.x) +":"+std::to_string(size.y);
    ImGui::Text("%s",size_str.c_str());
    static bool s;
    ImGui::Checkbox("111",&s);
    if(ImGui::IsItemClicked()){
        a = 1.0f;
    }

    // ImGui::GetCursorPos();= pos + window.scroll
    auto recover = ImGui::GetCursorScreenPos();
    // ImGui::InvisibleButton("testa",ImVec2(50,50));
    ImGui::ItemSize(recover,0);
    ImRect bb(recover,recover+ImVec2(5,15));
    ImGui::ItemAdd(bb,0);
    if(ImGui::IsItemClicked()){
        a = 0.5f;
    }
    auto size2 = ImGui::GetItemRectSize();
    // fmt::print("{}\n",size.x);
    draw_list->AddRectFilled(recover, recover +size2,IM_COL32_WHITE);
    ImGui::SetCursorScreenPos(recover+ImVec2(0,15));

    // ImGui::SetCursorPos(recover);
    
    ImGui::SliderFloat("##value", &a, 0.0f, 1.0f, "Alpha %.2f");
    // draw_list->AddCircleFilled(node_start_pos + ImVec2(20,20),5.0f,IM_COL32_WHITE);
    ImGui::ColorEdit3("##color", &c.x );


    ImGui::PopItemWidth();
    ImGui::EndGroup();
    auto size = ImGui::GetItemRectSize();
    // draw_list->AddRectFilled(node_start_pos, node_start_pos +size,IM_COL32_WHITE);

    // ImGui::SetCursorScreenPos(node_start_pos + ImVec2(0,-25));

    // ImGui::SetCursorScreenPos(node_start_pos + ImVec2(0,-15));
}
