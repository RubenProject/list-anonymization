#ifndef VAR_GRAPH_CC
#define VAR_GRAPH_CC

#include <iostream>
#include <algorithm>
#include <cmath>
#include <climits>
#include <list>

#include "dataanalysis.h"
#include "graph.hpp"


using namespace alglib;
using namespace std;


Graph::Graph(){
    node_count = 0;
    edge_count = 0;
    group_count = 0;
    time = 0;
}



void Graph::train_rfc(){
    string file_name_0 = string(TRAINING_DIR) 
        + string(DATA_SET) 
        + to_string(0) 
        + ".in";
    string file_name_1 = string(TRAINING_DIR) 
        + string(DATA_SET) 
        + to_string(1) 
        + ".in";
    string file_name_2 = string(TRAINING_DIR) 
        + string(DATA_SET) 
        + to_string(2) 
        + ".in";
    string file_name_3 = string(TRAINING_DIR) 
        + string(DATA_SET) 
        + to_string(3) 
        + ".in";
    string file_name_4 = string(TRAINING_DIR) 
        + string(DATA_SET) 
        + to_string(4) 
        + ".in";
    string file_name_5 = string(TRAINING_DIR) 
        + string(DATA_SET) 
        + to_string(5) 
        + ".in";
    add_edges_from_file(file_name_0.c_str());
    cout << "t0: " << node_count << endl;
    cout << "t0: " << edge_count << endl;
    vector<FeatureSet> f_list;
    extract_features(f_list);
    add_edges_from_file(file_name_1.c_str());
    add_edges_from_file(file_name_2.c_str());
    add_edges_from_file(file_name_3.c_str());
    add_edges_from_file(file_name_4.c_str());
    add_edges_from_file(file_name_5.c_str());
    cout << "t5: " << node_count << endl;
    cout << "t5: " << edge_count << endl;
    add_labels(f_list);
    create_rfc(f_list);

    //clear all datastructures
    adj.clear();
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


void Graph::create_rfc(vector<FeatureSet>& f_list){
    cout << "training classifier" << endl;
    string content;
    content = "[";
    for (int i = 0; i < (int)f_list.size(); i++){
        content += "[";
        content += to_string(f_list[i].foaf);
        content += ",";
        content += to_string(f_list[i].cn);
        content += ",";
        content += to_string(f_list[i].aa);
        content += ",";
        content += to_string(f_list[i].pa);
        content += ",";
        content += to_string(f_list[i].jc);
        content += ",";
        content += to_string(f_list[i].d);
        content += ",";
        content += to_string(f_list[i].label);
        content += "]";
        content += ",";
    }
    content.pop_back();
    content += "]";
    real_2d_array xy;
    xy = content.c_str();
    dfreport rep;
    ae_int_t info;
    const ae_int_t npoints = f_list.size();
    const ae_int_t nvars = 6;
    f_list.clear();
    dfbuildrandomdecisionforest(xy, npoints, nvars, 2, 10, 0.3, info, df, rep);
    cout << "done!" << info << endl;
    string df_s;
    ofstream f_out;
    f_out.open("../../data/rfc/tree.in", ios::out);
    dfserialize(df, f_out);
    f_out.close();
}


void Graph::add_labels(vector<FeatureSet>& f_list){
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
    vector<FeatureSet> f_list; 
    extract_features(f_list);
    test_features(f_list);
    f_list.clear();
    //print_pred_edges();
    assign_groups();
    cout << time << " ";
    cout << node_count << " ";
    cout << edge_count << " ";
    cout << group_count << " ";
    pred.clear();
    time++;
    return true;
}


void Graph::test_features(vector<FeatureSet> f_list){
    pred.resize(adj.size());
    ifstream f_in;
    f_in.open("../../data/rfc/tree.in", ios::in);
    dfunserialize(f_in, df);
    f_in.close();
    string line;
    real_1d_array x;
    real_1d_array y;
    for (int i = 0; i < (int)f_list.size(); i++){
        line = "[";
        line += to_string(f_list[i].foaf);
        line += ",";
        line += to_string(f_list[i].cn);
        line += ",";
        line += to_string(f_list[i].aa);
        line += ",";
        line += to_string(f_list[i].pa);
        line += ",";
        line += to_string(f_list[i].jc);
        line += ",";
        line += to_string(f_list[i].d);
        line += "]";
        x = line.c_str();
        dfprocess(df, x, y);
        if (y[1] >= P_CHANCE){
            //cout << "pred: " << f_list[i].n0 << ", " << f_list[i].n1 << endl;
            add_pred_edge(f_list[i].n0, f_list[i].n1);
            add_pred_edge(f_list[i].n1, f_list[i].n0);
        }
    }

}


void Graph::add_pred_edge(int from, int to){
    while (from >= (int)pred.size()){
        pred.push_back(vector<int>());
    }
    if (find(pred[from].begin(), pred[from].end(), to) == pred[from].end()){
        pred[from].push_back(to);
    }
}


float Graph::edge_identification(){
    float ei = 0;
    int comb = 0;
    for (int i = 0; i < (int)group_list.size(); i++){
        for (int j = i + 1; j < (int)group_list.size(); j++){
            ei += edge_identification(group_list[i], group_list[j], false);
            comb++;
        }
    }
    return ei/comb;
}


float Graph::node_group_density(){
    float ng = 0.0;
    for (int i = 0; i < (int)adj.size(); i++){
        ng += node_group_density(i);
    }
    return ng / (int)adj.size();
}


float Graph::node_group_density(int n0){
    float m = 0.0;
    int gid;
    Group S;
    for (int i = 0; i < (int)adj[n0].size(); i++){
        gid = node_group_mapping[adj[n0][i]];
        S = group_list[gid];
        m = max(m, node_group_density(n0, S));
    }
    return m;
}


float Graph::node_group_density(int n0, Group S){
    int count = 0;
    for (int i = 0; i < (int)adj[n0].size(); i++){
        if (find(S.nodes.begin(), S.nodes.end(), adj[n0][i]) != S.nodes.end()){
            count++;
        }
    }
    return (float)count / S.nodes.size();
}

void Graph::extract_features(vector<FeatureSet>& f_list){
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
                    f.d = bfs(i, j);
                    //f.d = -1;
                }
                f.jc = pred_jaccard(i, j);
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


float Graph::pred_jaccard(int n0, int n1){
    int int_size = 0;
    int un_size = (int)(adj[n0].size() + adj[n1].size());
    for ( int i = 0; i < (int)adj[n0].size(); i++){
        for ( int j = 0; j < (int)adj[n1].size(); j++){
            if (adj[n0][i] == adj[n1][j]){
                int_size++;
                un_size--;
            }
        }
    }
    return (float)int_size / un_size;
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


void Graph::print_pred_edges(){
    for (int i = 0; i < (int)pred.size(); i++){
        cout << i << ": ";
        for (int j = 0; j < (int)pred[i].size(); j++){
            cout << pred[i][j] << " ";
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


bool Graph::search(vector<int> x, int y){
    for (int i = 0; i < (int)x.size(); i++){
        if (x[i] == y){
            return true;
        }
    }
    return false;
}


float Graph::edge_identification(Group s1, Group s2, bool pr){
    int connections = 0;
    for (int i = 0; i < (int)s1.nodes.size(); i++){
        for (int j = 0; j < (int)s2.nodes.size(); j++){
            if (node_group_mapping[s2.nodes[j]] != -1){
                if(search(adj[s1.nodes[i]], s2.nodes[j])){
                    connections++;
                } else if (pr && search(pred[s1.nodes[i]], s2.nodes[j])){
                    connections++;
                }
            }
        }
    }
    float ret = ((float)connections / 2) / (s1.nodes.size() * s2.nodes.size());
    return ret;
}


//check all connected groups for a certain group
//check if they all satisfy the privacy requirement
bool Graph::group_density(Group s1){
    vector<int> connected_groups;
    int group_nr;
    for (int i = 0; i < (int)s1.nodes.size(); i++){
        for (int j = 0; j < (int)adj[s1.nodes[i]].size(); j++){
            group_nr = node_group_mapping[adj[s1.nodes[i]][j]];
            if (group_nr != -1 && !search(connected_groups, group_nr)){
                    connected_groups.push_back(group_nr);
            }
        }
        for (int j = 0; j < (int)pred[s1.nodes[i]].size(); j++){
            group_nr = node_group_mapping[pred[s1.nodes[i]][j]];
            if (group_nr != -1 && !search(connected_groups, group_nr)){
                    connected_groups.push_back(group_nr);
            }
        }
    }
    for (int i = 0; i < (int)connected_groups.size(); i++){
        if (edge_identification(s1, group_list[connected_groups[i]], true) > MIN_PRIVACY)
            return false;
    }
    return true;
}


void Graph::assign_groups(){
    group_list.resize(node_count);
    node_group_mapping.resize(node_count, -1);
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
            node_group_mapping[cur] = g.id;
            group_list[g.id] = g;
            group_count++;
        } else { 
            for (int i = 0; i < (int)new_groups.size() && !found; i++){
                new_groups[i].nodes.push_back(cur);
                node_group_mapping[cur] = new_groups[i].id;
                if (group_density(new_groups[i])){
                    group_list[new_groups[i].id] = new_groups[i];
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
                node_group_mapping[cur] = g.id;
                group_list[g.id] = g;
                group_count++;
            }
        }
    }
    int c = 0;
    int node;
    for (int i = 0; i < (int)new_groups.size(); i++){
        for (int j = 0; j < (int)new_groups[i].nodes.size(); j++){
            node = new_groups[i].nodes[j];
            full_groups[c % (int)full_groups.size()].nodes.push_back(node);
            node_group_mapping[node] = full_groups[c % (int)full_groups.size()].id;
            c++;
        }
        group_list[new_groups[i].id].nodes.clear();
    }

    for (int i = 0; i < (int)full_groups.size(); i++){
        g = full_groups[i];
        group_list[g.id] = g;
    }
    group_list.resize(group_count);
    new_groups.clear();
    full_groups.clear();

    for (int i = 0; i < (int)group_list.size(); i++){
        if (group_list[i].nodes.empty()){
            for (int j = i + 1; j < (int)group_list.size(); j++){
                if (!group_list[j].nodes.empty()){
                    group_list[i] = group_list[j];
                    group_list[j].nodes.clear();
                    break;
                }
            }
        }
    }
    c = 0;
    for (int i = 0; i < (int)group_list.size(); i++){
        group_list[i].id = i;
        for (int j = 0; j < (int)group_list[i].nodes.size(); j++){
            node_group_mapping[group_list[i].nodes[j]] = group_list[i].id;
        }
        if(group_list[i].nodes.empty())
            c++;
    }
    group_list.erase(group_list.end() - c, group_list.end());
    group_count = (int)group_list.size();
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
