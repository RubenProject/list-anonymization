#ifndef VAR_GRAPH_CC
#define VAR_GRAPH_CC

#include <iostream>
#include <algorithm>

#include "graph.hpp"

using namespace std;


Graph::Graph(){
    node_count = 0;
    edge_count = 0;
}


Graph::~Graph(){
}


void Graph::print_edges(){
    for (int i = 0; i < (int)adj.size(); i++){
        cout << i << ": ";
        for (int j = 0; j < (int)adj[i].size(); j++){
            cout << adj[i][j] << "-";
        }
        cout << endl;
    }
}


void Graph::print_node_info(){
    for (int i = 0; i < (int)node_info_list.size(); i++){
        cout << node_info_list[i].node_name << " -> "
        <<  node_info_list[i].id << endl;
    }
}


void Graph::print_groups(){
    cout << "not implemented" << endl;
}


bool Graph::node_exists(string node_name, int& node_id){
    for (int i = 0; i < (int)node_info_list.size(); i++){
        if (node_info_list[i].node_name == node_name){
            node_id = i;
            return true;
        }
    }
    return false;
}


void Graph::create_node(string node_name, int& node_id){
    Node_info info{.id = node_count, .node_name = node_name};
    node_info_list.push_back(info);
    node_id = node_count;
    node_count++;
}


//returns true if edge successfully added otherwise false
bool Graph::add_edge(int from, int to){
    while (from >= (int)adj.size()){
        adj.push_back(vector<int>(0, 0));
    }
    if (find(adj[from].begin(), adj[from].end(), to) == adj[from].end()){
        adj[from].push_back(to);
        edge_count++;
        return true;
    }
    return false;
}


bool Graph::add_edges_from_file(const char* input_name){
    ifstream fin;
    string from_node, to_node;
    int from_id, to_id;

    fin.open(input_name, ios::in);

    if (fin.is_open()){
        while (!fin.eof()){
            fin >> from_node >> to_node;
            if (!node_exists(from_node, from_id))
                create_node(from_node, from_id);
            if (!node_exists(to_node, to_id))
                create_node(to_node, to_id);

            add_edge(from_id, to_id);
            add_edge(to_id, from_id);
        }
        fin.close();
        return true;
    } else {
        cout << "File does not exist!" << endl;
        return false;
    }
}

#endif
