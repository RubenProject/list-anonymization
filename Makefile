CC = g++

CompileParms = -c -Wall

OBJS = graph.o main.o

anon: $(OBJS)
	$(CC) $(OBJS) -o anon

graph.o: graph.cpp graph.hpp
	$(CC) $(CompileParms) graph.cpp

main.o: main.cpp graph.hpp
	$(CC) $(CompileParms) main.cpp
