CC = g++

CompileParms = -c -Wall -std=c++11

OBJS = graph.o alglibinternal.o alglibmisc.o ap.o dataanalysis.o diffequations.o fasttransforms.o integration.o interpolation.o linalg.o optimization.o solvers.o specialfunctions.o statistics.o main.o

anon: $(OBJS)
	$(CC) $(OBJS) -o anon

alglibinternal.o: alglibinternal.cpp alglibinternal.h
	$(CC) $(CompileParms) alglibinternal.cpp

alglibmisc.o: alglibmisc.cpp alglibmisc.h
	$(CC) $(CompileParms) alglibmisc.cpp

ap.o: ap.cpp ap.h
	$(CC) $(CompileParms) ap.cpp

dataanalysis.o: dataanalysis.cpp dataanalysis.h
	$(CC) $(CompileParms) dataanalysis.cpp

diffequations.o: diffequations.cpp diffequations.h
	$(CC) $(CompileParms) diffequations.cpp

fasttransforms.o: fasttransforms.cpp fasttransforms.h
	$(CC) $(CompileParms) fasttransforms.cpp

integration.o: integration.cpp integration.h
	$(CC) $(CompileParms) integration.cpp

interpolation.o: interpolation.cpp interpolation.h
	$(CC) $(CompileParms) interpolation.cpp

linalg.o: linalg.cpp linalg.h
	$(CC) $(CompileParms) linalg.cpp

optimization.o: optimization.cpp optimization.h
	$(CC) $(CompileParms) optimization.cpp

solvers.o: solvers.cpp solvers.h
	$(CC) $(CompileParms) solvers.cpp

specialfunctions.o: specialfunctions.cpp specialfunctions.h
	$(CC) $(CompileParms) specialfunctions.cpp

statistics.o: statistics.cpp statistics.h
	$(CC) $(CompileParms) statistics.cpp

graph.o: graph.cpp graph.hpp
	$(CC) $(CompileParms) graph.cpp

main.o: main.cpp graph.hpp
	$(CC) $(CompileParms) main.cpp
