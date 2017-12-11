from networkit import *

FF_gen = generators.DynamicForestFireGenerator(0.25, 0, 0.3)
#g = dynamic.graphFromStream(FF_gen, 0, 0)

j = 0
for i in range(0, 11):
    node_count = 0
    g = dynamic.graphFromStream(FF_gen.generate(100), 0, 0)
    node_count = g.numberOfNodes()
    print(node_count)
    graphio.EdgeListWriter(" ", 0, 0).write(g, "./data/FF_1k-2k_new_t" + str(i) + ".in")
