#pragma once
#include "utils.hpp"
#include <memory>
#include <unordered_map>

/*
@todo size to ImGui::GetItemSize
*/

struct Scene;
struct Node
{
    Idtype id;
    ImVec2 pos;
    ImVec2 size = ImVec2(150, 60);
    std::string name;
    std::shared_ptr<Context> context;
    ElementType element_type = ElementType::ElementNode;

    std::weak_ptr<Scene> scene;
    bool enable = true;
    int input_sockets_num = 3;
    int ouput_sockets_num = 3;
    std::unordered_map<int, Idtype> input_socket_ids;
    std::unordered_map<int, Idtype> ouput_socket_ids;

    ImU32 node_body_color = IM_COL32(150, 150, 150, 150);
    bool is_debug_mode = true;

    Node(std::weak_ptr<Scene> _scene, 
        std::shared_ptr<Context> _context, 
        const std::string &_name, 
        ImVec2 _pos);

    void setSocketsNum(int _input_sockets_num, int _output_sockets_num);
    void setNodeSize(ImVec2 _size);

    void init();
    void draw();
    void update(){}
    void drawDebug(ImVec2 node_start_pos);

};