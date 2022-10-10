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
        this->id = _context->id++;
    }
    void draw(){}
};

struct LinkMap
{
    // using PairId = std::pair<int,int>;
    using NodeLinkTree = std::map<int, std::shared_ptr<NodeLink>>;
    //@check
    std::unordered_map<std::string, std::shared_ptr<NodeLinkTree>> node_links;

    LinkMap(){}

    void insert(std::shared_ptr<NodeLink> node_link){
        std::string link_id = std::to_string(node_link->output_node_id) 
                                + '/' 
                                + std::to_string(node_link->output_socket_id);
        fmt::print("{}\n",link_id);
        if(this->node_links.contains(link_id)){
            auto node_link_tree = this->node_links[link_id];
            node_link_tree->insert({node_link->id, node_link});
        }else{
            auto node_link_tree = std::make_shared<NodeLinkTree>();
            node_link_tree->insert({node_link->id, node_link});
            this->node_links.insert(
                {
                    link_id,
                    node_link_tree
                });
        }
    }

    void search(int output_node_id, int output_socket_id){
        std::string link_id = std::to_string(output_node_id) 
                                + '/' 
                                + std::to_string(output_socket_id);
        if(this->node_links.contains(link_id)){
            fmt::print("search\n");
            for(auto i=this->node_links[link_id]->begin();i!=node_links[link_id]->end();i++){
                fmt::print("{},{},{},{}\n",i->second->input_node_id,
                        i->second->input_socket_id,
                        i->second->output_node_id,
                        i->second->output_socket_id
                    );

            }
            // for(auto& [id, link]: this->node_links[link_id]->begin){
            // }
        }
    }

    void print(){
        // fmt::print("{}\n",node_links.size());
        for (const auto& [link_id,link_tree] : this->node_links){
            fmt::print("debug{}\n",link_tree->size());
        }
    }
};
