#pragma once

#include<memory>
#include<map>
#include "utils.hpp"

struct NodeLink
{
    int id;
    int input_node_id;
    int output_node_id;
    int input_socket_id;
    int output_socket_id;
    //@todo bool enable = true;
    //@todo bool is_check = true; make link color diff

    // NodeLink(const Context &_context)
    NodeLink(std::shared_ptr<Context> _context, 
        int _input_node_id, 
        int _input_socket_id, 
        int _output_node_id, 
        int _output_socket_id)
        :input_node_id(_input_node_id),
        output_node_id(_output_node_id),
        input_socket_id(_input_socket_id),
        output_socket_id(_output_socket_id)
    {
        this->id = ++_context->id;
    }
    void draw(){}
};

struct LinkMap
{
    using tId = std::string;
    // input socket id -> NodeLink
    using NodeLinkMap = std::unordered_map<tId, std::shared_ptr<NodeLink>>;
    // output socket id -> NodeLinks
    std::unordered_map<tId, std::shared_ptr<NodeLinkMap>> node_links;

    LinkMap(){}

    std::string makeId(int id1,int id2){
        return std::to_string(id1) + '/' + std::to_string(id2);
    }


    void insert(std::shared_ptr<NodeLink> node_link){
        std::string output_id = makeId(node_link->output_node_id, node_link->output_socket_id);
        std::string input_id = makeId(node_link->input_node_id, node_link->input_socket_id);

        fmt::print("{}\n",output_id);
        if(this->node_links.contains(output_id)){
            auto node_link_tree = this->node_links[output_id];
            node_link_tree->insert({input_id, node_link});
        }else{
            auto node_link_tree = std::make_shared<NodeLinkMap>();
            node_link_tree->insert({input_id, node_link});
            this->node_links.insert(
                {
                    output_id,
                    node_link_tree
                });
        }
    }

    void search(int output_node_id, int output_socket_id){
        std::string link_id = makeId(output_node_id, output_socket_id);

        if(this->node_links.contains(link_id)){
            fmt::print("search\n");
            for(auto i=this->node_links[link_id]->begin();i!=node_links[link_id]->end();i++){
                fmt::print("{}:{}->{}:{}\n",
                    i->second->output_node_id,
                    i->second->output_socket_id,
                    i->second->input_node_id,
                    i->second->input_socket_id
                );
            }
            // for(auto& [id, link]: this->node_links[link_id]->begin){
            // }
        }
    }

    void print(){
        fmt::print("{}\n",node_links.size());
        for (const auto& [link_id,link_tree] : this->node_links){
            fmt::print("debug{}\n",link_tree->size());
            for(auto i=this->node_links[link_id]->begin();i!=node_links[link_id]->end();i++){
                // fmt::print("    {}:{}->{}:{}\n",
                //     i->second->output_node_id,
                //     i->second->output_socket_id,
                //     i->second->input_node_id,
                //     i->second->input_socket_id
                // );
                fmt::print("    {}:{}", link_id, i->first);
            }
        }
    }
};
