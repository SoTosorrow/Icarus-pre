#pragma once
#include "utils.hpp"
#include <memory>
#include <unordered_map>
#include <type_traits>
#include <string_view>
/*
@todo size to ImGui::GetItemSize
*/

struct Scene;
struct Node
{
    Idtype id;
    ImVec2 pos;
    ImVec2 size = ImVec2(150, 60);
    std::string_view name;
    std::shared_ptr<Context> context;
    ElementType element_type = ElementType::ElementNode;

    std::weak_ptr<Scene> scene;
    bool enable = true;
    bool resize = false;
    int input_sockets_num = 3;
    int ouput_sockets_num = 3;
    std::unordered_map<int, Idtype> input_socket_ids;
    std::unordered_map<int, Idtype> ouput_socket_ids;

    int compute_input_dep = 0;

    ImU32 node_body_color = IM_COL32(150, 150, 150, 150);
    bool is_debug_mode = true;

    Node(std::weak_ptr<Scene> _scene, 
        std::shared_ptr<Context> _context, 
        const std::string_view &_name, 
        ImVec2 _pos);

    void setSocketsNum(int _input_sockets_num, int _output_sockets_num);
    void setNodeSize(int _size_x, int _size_y);

    /*
        virtual func to dispatch derive node func
    */
    virtual void init();
    virtual void draw();
    virtual void fillContent(){}
    virtual void update(){fmt::print("EXEC: {}\n", this->id);}
    void drawDebug(ImVec2 node_start_pos);

};


template<class T>
concept NodeAble = std::is_base_of<Node, T>::value;

// template<NodeAble T>
// struct NodeA : public NodeAble{

// };
