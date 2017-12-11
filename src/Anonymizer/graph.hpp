#ifndef VAR_GRAPH_H
#define VAR_GRAPH_H

#define GROUP_SIZE 10
#define MIN_PRIVACY 3.0 / (GROUP_SIZE * GROUP_SIZE)
#define TRAINING_DIR "../../data/training/"
#define TEST_DIR "../../data/test/"
//#define DATA_SET "FF_10k-20k_t"
#define DATA_SET "FF_1k-2k_new_t"
//#define DATA_SET "FF_1k-2k_t"
//#define DATA_SET "COSTUM_"


#include "dataanalysis.h"


#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>


using namespace alglib;
using namespace std;


 /* 
  * 
  * TODO: data set does not create old-old edges...
  *
  * TODO: run experiments
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
        void print_edges();
        void print_pred_edges();
        void print_groups();
        void train_rfc();
        float edge_identification();
        float node_group_density();
    private:
        float edge_identification(Group s1, Group s2, bool pr);
        float node_group_density(int n0);
        float node_group_density(int n0, Group S);

        void assign_groups();
        void extract_features(vector<FeatureSet>& xy);
        bool add_edges_from_file(const char* input_name);
        void create_node(string node_name, int& node_id);
        bool node_exists(string node_name, int& node_id);
        bool add_edge(int from, int to);
        bool edge_exists(int n0, int n1);
        void add_labels(vector<FeatureSet>& f_list);
        bool group_density(Group s1);
        void pred_fcap(FeatureSet& f, int n0, int n1);
        int bfs(int s, int g);
        void add_pred_edge(int from, int to);
        void create_rfc(vector<FeatureSet>& f_list);
        void test_features(vector<FeatureSet> f_list);

        bool search(vector<int> x, int y);

        decisionforest df;


        vector<vector<int>> adj;
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
