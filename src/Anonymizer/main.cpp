#include <iostream>

#include "graph.hpp"

using namespace std;

Graph* g;

int main (int argc, char* argv[]){
    if (argc > 1){
        cout << "Rebuilding features..." << endl;
        cout << argv[1] << endl;
    }
    g = new Graph();
    g->train_rfc();

    while(g->update()){
        //g->print_groups();
        cout << g->node_group_density() << " ";
        cout << g->edge_identification() << endl;
    //    break;
    }
    delete g;

    return 0;
}

