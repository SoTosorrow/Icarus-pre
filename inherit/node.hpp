#include"../src/node.hpp"

struct NodeB :public Node{
    float a= 1;
    ImVec4 c;

    NodeB(std::weak_ptr<Scene> _scene, 
        std::shared_ptr<Context> _context, 
        const std::string &_name, 
        ImVec2 _pos)
    :Node(_scene, _context, _name, _pos){
        fmt::print("TEST node create\n");
        this->node_body_color = IM_COL32(50, 200, 200, 200);
    }
    void draw(){
        Node::draw();

        auto draw_list = ImGui::GetWindowDrawList();
        auto node_start_pos = this->pos + this->context->vp_trans;
        ImGui::SetCursorScreenPos(node_start_pos);
        ImGui::BeginGroup();
        ImGui::PushItemWidth(120);

        ImGui::SliderFloat("##value", &a, 0.0f, 1.0f, "Alpha %.2f");
        ImGui::ColorEdit3("##color", &c.x );

        ImGui::PopItemWidth();
        ImGui::EndGroup();
        // auto size = ImGui::GetItemRectSize();
        // draw_list->AddRectFilled(node_start_pos, node_start_pos +size,IM_COL32(255,255,255,200));

    }

};