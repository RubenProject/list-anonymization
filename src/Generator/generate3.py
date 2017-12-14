from networkit import *

FF_gen = generators.DynamicForestFireGenerator(0.25, 0, 0.3).generate(1500);
g = dynamic.graphFromStream(FF_gen, 0, 0)

for i in range(5, -1, -1):
    print("----------------------------------------------------")
    overview(g)
    graphio.EdgeListWriter(" ", 0, 0).write(g, "./data/MY_DATA_t" + str(i) + ".in")
#delete 120 random edges per step
    for j in range(0, 120):
        u = g.randomNode()
        if g.degree(u) > 0:
            v = g.randomNeighbor(u)
            g.removeEdge(u, v)
            if g.degree(v) == 0:
                g.removeNode(v)
        else :
            g.removeNode(u)
#delete 60 random nodes, with their connected edges
    for j in range(0, 60):
        g.removeNode(g.randomNode())
