#include <iostream>

#include "graph.hpp"

using namespace std;

Graph* g;

int main (){

    g = new Graph();
    g->add_edges_from_file("../data/forestfire1.txt");
    g->print_edges();
    g->assign_groups();
    g->print_groups();
    delete g;

    return 0;
}

