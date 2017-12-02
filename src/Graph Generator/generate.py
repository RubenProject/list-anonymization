from networkit import *

FF_gen = generators.DynamicForestFireGenerator(0.25, 0, 0.3).generate(20000)
g = dynamic.graphFromStream(FF_gen, 0, 0)

j = 0
for i in range(0, 11):
    node_count = 0
    while node_count < 10000 + 1000 * i:
        g = dynamic.graphFromStream(FF_gen[:j], 0, 0)
        node_count = g.numberOfNodes()
        j += 10
    print(node_count)
    graphio.EdgeListWriter(" ", 0, 0).write(g, "../../data/training/FF_10k-20k_t" + str(i) + ".in")
