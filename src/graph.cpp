#ifndef VAR_GRAPH_CC
#define VAR_GRAPH_CC

#include <iostream>
#include <algorithm>

#include "graph.hpp"

using namespace std;


Graph::Graph(){
    node_count = 0;
    edge_count = 0;
    group_count = 0;
    time = 0;
}


Graph::~Graph(){
}


void Graph::print_edges(){
    for (int i = 0; i < (int)adj.size(); i++){
        cout << i << ": ";
        for (int j = 0; j < (int)adj[i].size(); j++){
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
}


void Graph::print_groups(){
    cout << "There are " << group_list.size() << " groups." << endl;
    for (int i = 0; i < (int)group_list.size(); i++){
        cout << group_list[i].id << ": ";
        for (int j = 0; j < (int)group_list[i].nodes.size(); j++){
            cout << group_list[i].nodes[j] << " ";
        }
        cout << endl;
    }
}


bool Graph::node_exists(string node_name, int& node_id){
    unordered_map<string, int>::const_iterator index;
    index = name_id_mapping.find(node_name);
    if (index == name_id_mapping.end())
        return false;
    else {
        node_id = index->second;
        return true;
    }
}


void Graph::create_node(string node_name, int& node_id){
    name_id_mapping[node_name] = node_count;
    node_id = node_count;
    id_name_mapping.push_back(node_name);
    node_count++;
    if ((int)id_name_mapping.size() != node_count)
        cout << "id-name mapping misaligned" << endl;
}


//returns true if edge successfully added otherwise false
bool Graph::add_edge(int from, int to){
    while (from >= (int)adj.size()){
        adj.push_back(vector<int>());
    }
    if (find(adj[from].begin(), adj[from].end(), to) == adj[from].end()){
        adj[from].push_back(to);
        edge_count++;
        return true;
    }
    return false;
}


float Graph::edge_identification(Group s1, Group s2){
    int connections = 0;
    for (int i = 0; i < (int)s1.nodes.size(); i++){
        for (int j = 0; j < (int)s2.nodes.size(); i++){
            if (node_group_mapping[s2.nodes[j]] != -1
            && find(adj[s1.nodes[i]].begin(), adj[s1.nodes[i]].end(), s2.nodes[j]) != adj[s1.nodes[i]].end()){
                connections++;
            }
        }
    }
    return connections / (s1.nodes.size() * s2.nodes.size());
}


//check all connected groups for a certain group
//check if they all satisfy the privacy requirement
bool Graph::group_density(Group s1){
    vector<int> connected_groups;
    int group_nr;
    for (int i = 0; i < (int)s1.nodes.size(); i++){
        for (int j = 0; j < (int)adj[s1.nodes[i]].size(); j++){
            group_nr = adj[s1.nodes[i]][j];
            if (group_nr != -1
            && find(connected_groups.begin(), connected_groups.end(), group_nr) != connected_groups.end()){
                    connected_groups.push_back(group_nr);
            }
        }
    }
    for (int i = 0; i < (int)connected_groups.size(); i++){
        if (edge_identification(s1, group_list[connected_groups[i]]) < MIN_PRIVACY)
            return false;
    }
    return true;
}


void Graph::assign_groups(){
    vector<Group> new_groups;
    vector<Group> full_groups;
    Group g;
    int cur;
    while (ungrouped.size() > 0){
        bool found = false;
        cur = ungrouped.back();
        ungrouped.pop_back();
        if ((int)new_groups.size() == 0){
            g.id = group_count;
            g.time = time;
            g.nodes.clear();
            g.nodes.push_back(cur);
            new_groups.push_back(g);
            if ((int)node_group_mapping.size() < cur)
                node_group_mapping.resize(cur);
            node_group_mapping[cur] = g.id;
            group_count++;
        } else { 
            for (int i = 0; i < (int)new_groups.size() && !found; i++){
                new_groups[i].nodes.push_back(cur);
                node_group_mapping.push_back(new_groups[i].id);
                if (true/*group_density(new_groups[i])*/){
                    if (new_groups[i].nodes.size() >= GROUP_SIZE){
                        g = new_groups[i];
                        full_groups.push_back(g);
                        new_groups.erase(new_groups.begin() + i);
                    }
                    found = true;
                } else {
                    new_groups[i].nodes.pop_back();
                }
            } if (!found) {
                g.id = group_count;
                g.time = time;
                g.nodes.clear();
                g.nodes.push_back(cur);
                new_groups.push_back(g);
                node_group_mapping.push_back(g.id);
                group_count++;
            }
        }
    }
    //TODO: divide nodes of new_groups over full_groups


    for (int i = 0; i < (int)new_groups.size(); i++){
        g = new_groups[i];
        group_list.push_back(g);
    }

    for (int i = 0; i < (int)full_groups.size(); i++){
        g = full_groups[i];
        group_list.push_back(g);
    }
}


bool Graph::add_edges_from_file(const char* input_name){
    ifstream fin;
    string from_node, to_node;
    int from_id, to_id;

    fin.open(input_name, ios::in);

    if (fin.is_open()){
        while (!fin.eof()){
            fin >> from_node >> to_node;

            if (!node_exists(from_node, from_id)){
                create_node(from_node, from_id);
                ungrouped.push_back(from_id);
            }
            if (!node_exists(to_node, to_id)){
                create_node(to_node, to_id);
                ungrouped.push_back(to_id);
            }

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
