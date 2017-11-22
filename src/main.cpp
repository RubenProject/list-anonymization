#include <iostream>

#include "graph.hpp"

using namespace std;

Graph* g;

int main (){

    g = new Graph();
    g->add_edges_from_file("../data/forestfire3.txt");
    g->print_edges();
    delete g;

    return 0;
}

