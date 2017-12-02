#ifndef VAR_GRAPH_H
#define VAR_GRAPH_H

#define GROUP_SIZE 10
#define MIN_PRIVACY 1.0 / (GROUP_SIZE * GROUP_SIZE)
#define TRAINING_DIR "../../data/training/"
#define TEST_DIR "../../data/test/"
#define DATA_SET "FF_10k-20k_t"
//#define DATA_SET "COSTUM_"

#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>

using namespace std;


/*  TODO: final stage of grouping
 *  TODO: TEST grouping condition
 *  TODO: implement RFC
 *  TODO: add prediction to grouping condition
 *  TODO: run experiments
 *  
 *
 *  TODO: 0 edges are predicted correctly? How is this even possible..
 *  TODO: generate smaller test set...
 *
 *
 *
 *
 */



struct Group {
    int id;
    int time;
    vector<int> nodes;
};


struct FeatureSet {
    int n0;
    int n1;
    bool foaf;
    int cn;
    float aa;
    int pa;
    int d;
    bool label;
};


class Graph {
    public:
        Graph();

        bool update();
        bool add_edges_from_file(const char* input_name);
        void assign_groups();
        void print_edges();
        void print_groups();
        void generate_trainingdata();
        float edge_identification(Group s1, Group s2);
        float node_group_density();
        void extract_features();
    private:
        void create_node(string node_name, int& node_id);
        bool node_exists(string node_name, int& node_id);
        bool add_edge(int from, int to);
        bool edge_exists(int n0, int n1);
        void add_labels();
        bool group_density(Group s1);
        void pred_fcap(FeatureSet& f, int n0, int n1);
        int bfs(int s, int g);
        void write_features(bool training);
        bool read_predictions();
        void add_pred_edge(int from, int to);

        vector<vector<int>> adj;
        vector<FeatureSet> f_list;
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
