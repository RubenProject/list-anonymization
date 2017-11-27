#ifndef VAR_GRAPH_H
#define VAR_GRAPH_H

#define GROUP_SIZE 10
#define MIN_PRIVACY 1.0 / (GROUP_SIZE * GROUP_SIZE)

#include <unordered_map>
#include <vector>
#include <deque>
#include <fstream>
#include <string>

using namespace std;


struct Group {
    int id;
    int time;
    vector<int> nodes;
};


struct FeatureSet {
    bool foaf;
    int cn;
    float aa;
    float pa;
};


class Graph {
    public:
        Graph();
        ~Graph();

        bool add_edges_from_file(const char* input_name);
        void assign_groups();
        void print_edges();
        void print_groups();
        float edge_identification(Group s1, Group s2);
        float node_group_density();
        void extract_features();
    private:
        void create_node(string node_name, int& node_id);
        bool node_exists(string node_name, int& node_id);
        bool add_edge(int from, int to);
        bool group_density(Group s1);
        void pred_fcap(FeatureSet& f, int n0, int n0);

        vector<vector<int>> adj;
        vector<vector<FeatureSet> fm;
        vector<vector<int>> pred;
        vector<int> ungrouped;
        unordered_map<string, int> name_id_mapping;
        vector<string> id_name_mapping;
        vector<int> node_group_mapping;
        vector<Group> group_list;
        
        int node_count;
        int edge_count;
        int group_count;
        int time;
};

#endif
