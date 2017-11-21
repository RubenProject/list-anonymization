#ifndef VAR_GRAPH_H
#define VAR_GRAPH_H

#define GROUP_SIZE 10

#include <vector>
#include <fstream>

using namespace std;


struct Group {
    int time;
    int size;
    vector<int> nodes;
};


struct Node_info {
    int id;
    string node_name;
};


class Graph {
    public:
        Graph();
        ~Graph();
        bool add_edges_from_file(const char* input_name);
        float edge_identification();
        float node_group_density();
    private:
        void create_node(string node_name, int& node_id);
        bool node_exists(string node_name, int& node_id);
        void add_edge(int from, int to);

        vector<vector<int>> adj;
        vector<vector<int>> pred;
        vector<Node_info> node_info_list;
        vector<Group> group_list;
        int node_count;
        int edge_count;
};

#endif
