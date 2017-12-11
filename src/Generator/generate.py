from networkit import *

FF_gen = generators.DynamicForestFireGenerator(0.25, 0, 0.3).generate(2000)
g = dynamic.graphFromStream(FF_gen, 0, 0)

j = 0
for i in range(0, 11):
    node_count = 0
    while node_count < 1000 + 100 * i:
        g = dynamic.graphFromStream(FF_gen[:j], 0, 0)
        node_count = g.numberOfNodes()
        j += 1
    print(node_count)
    graphio.EdgeListWriter(" ", 0, 0).write(g, "./data/FF_1k-2k_t" + str(i) + ".in")
