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
    g->generate_trainingdata();
    delete g;

    g = new Graph();
    while(g->update()){
        g->print_groups();
        break;
        //do some statistic analysis
    }
    delete g;

    return 0;
}

