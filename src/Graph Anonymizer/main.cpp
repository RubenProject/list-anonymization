#include <iostream>

#include "graph.hpp"

using namespace std;

Graph* g;

int main (int argc, char* argv[]){
    if (argc > 1){
        cout << argv[1] << endl;
    }
    g = new Graph();
    g->generate_trainingdata();
    //while(!g->update()){
     //   break;
        //do some statistic analysis
    //}
    delete g;

    return 0;
}

