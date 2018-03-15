all:
	gcc -g  -Wall -Wstrict-prototypes -c graph.c
	gcc -g  -Wall -Wstrict-prototypes -c make-graph.c
	gcc -g  -Wall -Wstrict-prototypes -c print-graph.c
	gcc -lm  -o make-graph make-graph.o graph.o
	gcc -lm -o print-graph print-graph.o graph.o
	gcc -g -w -Wstrict-prototypes -c BackSub.c
	gcc -w -o BackSub BackSub.o graph.o -lrt
	gcc -g -w -Wstrict-prototypes -c MatVec.c
	gcc -w -o MatVec MatVec.o graph.o -lrt
	gcc -g -w -Wstrict-prototypes -c VecComp.c
	gcc -w -o VecComp VecComp.o graph.o -lrt -lm
	mpicc -g -w -Wall -Wstrict-prototypes -c MyMPI.c 
	mpicc -g -w -Wall -Wstrict-prototypes -c Gauss-Parallel.c
	mpicc -w -o Gauss-Parallel  Gauss-Parallel.o graph.o MyMPI.o -lrt

clean:
	$(RM) graph.o make-graph.o make-graph print-graph.o print-graph Gauss-Parallel.o Gauss-Parallel MyMPI.o MyMPI MatVec MatVec.o BackSub BackSub.o VecComp VecComp.o  gauss-parallel.o*
