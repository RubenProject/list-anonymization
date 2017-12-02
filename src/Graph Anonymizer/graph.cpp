#ifndef VAR_GRAPH_CC
#define VAR_GRAPH_CC

#include <iostream>
#include <algorithm>
#include <cmath>
#include <climits>
#include <list>

#include "graph.hpp"

using namespace std;


Graph::Graph(){
    node_count = 0;
    edge_count = 0;
    group_count = 0;
    time = 0;
}



void Graph::generate_trainingdata(){
    string file_name_0 = string(TRAINING_DIR) 
        + string(DATA_SET) 
        + to_string(0) 
        + ".in";
    string file_name_1 = string(TRAINING_DIR) 
        + string(DATA_SET) 
        + to_string(1) 
        + ".in";
    add_edges_from_file(file_name_0.c_str());
    extract_features();
    add_edges_from_file(file_name_1.c_str());
    add_labels();
    write_features(true);
    //clear all datastructures
    adj.clear();
    f_list.clear();
    pred.clear();
    ungrouped.clear();
    name_id_mapping.clear();
    id_name_mapping.clear();
    node_group_mapping.clear();
    group_list.clear();
    //reset counters
    node_count = 0;
    edge_count = 0;
    group_count = 0;
    time = 0;
}


void Graph::add_labels(){
    for (int i = 0; i < (int)f_list.size(); i++){
        f_list[i].label = edge_exists(f_list[i].n0, f_list[i].n1);
    }
}


bool Graph::update(){
    if (time == 0){
        node_count = 0;
        edge_count = 0;
        group_count = 0;
    }
    string cur_file_name = string(TEST_DIR) 
        + string(DATA_SET) 
        + to_string(time) 
        + ".in";
    if (!add_edges_from_file(cur_file_name.c_str()))
        return false;
    extract_features();
    write_features(false);
    //wait for user input when RFC finished
    int go;
    cout << "input number when RFC finished" << endl;
    cin >> go;
    read_predictions();
    assign_groups();
    pred.clear();
    time++;
    return true;
}


void Graph::add_pred_edge(int from, int to){
    while (from >= (int)pred.size()){
        pred.push_back(vector<int>());
    }
    if (find(pred[from].begin(), pred[from].end(), to) == pred[from].end()){
        pred[from].push_back(to);
    }
}


bool Graph::read_predictions(){
    ifstream fin;
    string file_name;
    string line;
    int from_id, to_id;
    file_name = string(TEST_DIR) + "predicted_edges.in";
    fin.open(file_name.c_str(), ios::in);
    if (fin.is_open()){
        cout << "reading from: " << file_name << endl;
        while (!fin.eof()){
            fin >> from_id >> to_id;

            add_pred_edge(from_id, to_id);
            add_pred_edge(to_id, from_id);
        }
        fin.close();
        return true;
    } else {
        cout << "File does not exist!" << endl;
        return false;
    }
}


void Graph::write_features(bool training){
    ofstream fout;
    string file_name;
    string line;
    if (training){
        file_name = string(TRAINING_DIR)
            + "labeled_features.in";
        fout.open(file_name.c_str(), ios::out);
    } else {
        file_name = string(TEST_DIR)
            + "unlabeled_features.in";
        fout.open(file_name.c_str(), ios::out);
    }
    if (fout.is_open()){
        cout << "Writing features to: " << file_name << endl;
        for (int i = 0; i < (int)f_list.size(); i++){
            line.clear();
            line += to_string(f_list[i].n0) ;
            line += ", " + to_string(f_list[i].n1); 
            line += ", " + to_string(f_list[i].foaf);
            line += ", " + to_string(f_list[i].cn);
            line += ", " + to_string(f_list[i].aa);
            line += ", " + to_string(f_list[i].pa);
            line += ", " + to_string(f_list[i].d);
            if (training){
                line += ", " + to_string(f_list[i].label);
            }
            line += "\n";
            fout << line;
        }
        fout.close();
    }
}


void Graph::extract_features(){
    cout << "Extracting features..." << endl;
    FeatureSet f;
    float progress = 0;
    float last_progress = 0;
    float step = 100.0 / node_count;
    for (int i = 0; i < node_count; i++){
        for (int j = i + 1; j < node_count; j++){
            if (!edge_exists(i , j)){
                f.n0 = i;
                f.n1 = j;
                pred_fcap(f, i, j);
                if (f.foaf) {
                    f.d = 2;
                }else{
                    //f.d = bfs(i, j);
                    f.d = -1;
                }
                f_list.push_back(f);
            }
        }
        //progress counter
        progress += step;
        if ((int)progress > (int)last_progress + 10){
            cout << (int)progress << "%" << endl;
            last_progress = progress;
        }
    }
}


int Graph::bfs(int s, int g){
    const int max_depth = 15;
    int depth = 0;
    bool* visited = new bool[node_count];
    for (int i = 0; i < node_count; i++)
        visited[i] = false;

    list<int> parent;
    list<int> child;

    visited[s] = true;
    parent.push_back(s);

    vector<int>::iterator i;

    while(!parent.empty() || !child.empty()){

        if (!parent.empty()){
            s = parent.front();
            parent.pop_front();
        } else {
            parent = child;
            child.clear();
            s = parent.front();
            parent.pop_front();
            depth++;
            if (depth > max_depth)
                return max_depth;
        }
        if (s == g)
            return depth;
        
        for (i = adj[s].begin(); i != adj[s].end(); ++i){
            if (!visited[*i]){
                visited[*i] = true;
                child.push_back(*i);
            }
        }
    }
    return max_depth;
}


bool Graph::edge_exists(int n0, int n1){
    if ((int)adj.size() < n0)
        return false;
    for (int i = 0; i < (int)adj[n0].size(); i++){
        if (adj[n0][i] == n1)
            return true;
    }
    return false;
}


void Graph::pred_fcap(FeatureSet& f, int n0, int n1){
    int cn = 0;
    float aa = 0;
    for (int i = 0; i < (int)adj[n0].size(); i++){
        for (int j = 0; j < (int)adj[n1].size(); j++){
            if (adj[n0][i] == adj[n1][j]){//common neighbour found
                cn++;
                aa += (float)(1 / log10((double)adj[adj[n0][i]].size()));
            }
        }
    }
    f.foaf = (bool)cn;
    f.cn = cn;
    f.aa = aa;
    f.pa = adj[n0].size() * adj[n1].size();
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
        cout << "reading from: " << input_name << endl;
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
