#ifndef VAR_GRAPH_CC
#define VAR_GRAPH_CC

#include "graph.hpp"

using namespace std;


Graph::Graph(){
    node_count = 0;
    edge_count = 0;
}


Graph::~Graph(){
}


bool node_exists(string node_name, int& node_id){
    for (int i = 0; i < node_info_list.size(); i++){
        if (node_info_list[i].node_name == node_name){
            node_id = i;
            return true;
        }
    }
    return false;
}


void create_node(string node_name, int& node_id){
    Node_info info(node_count, node_name);
    node_info_list.push_back(info);
    node_count++;
}


//TODO: check for duplicate edges
void add_edge(int from, int to){
    while (from > adj.size())
        adj.push_back(vector<int>);
    adj[from].push_back(to);
    edge_count++;
}


bool Graph::add_edges_from_file(const char* input_name){
    ifstream fin;
    string from_node, to_node;
    int from_id, to_id;

    fin.open(input_name, ios:in);

    if (fin.is_open()){
        while (!exit){
            fin >> from_node >> to_node;
            if (!node_exists(from_node, from_id))
                create_node(from_node, from_id);
            if (!node_exists(to_node, to_id))
                create_node(to_node, to_id);

            add_edge(from_id, to_id);
        }
        fin.close();
        return true;
    } else {
        cout << "File does not exist!" << endl;
        return false;
    }
}

#endif
