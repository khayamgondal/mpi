all:
	gcc -g  -Wall -Wstrict-prototypes -c graph.c
	gcc -g  -Wall -Wstrict-prototypes -c make-graph.c
	gcc -g  -Wall -Wstrict-prototypes -c print-graph.c
	gcc -g -w -Wall -Wstrict-prototypes -c floyd-serial.c
	gcc  -o make-graph make-graph.o graph.o
	gcc -lm -o print-graph print-graph.o graph.o
	gcc -w -o floyd-serial floyd-serial.o graph.o -lrt
	mpicc -g -w -Wall -Wstrict-prototypes -c MyMPI.c 
	mpicc -g -w -Wall -Wstrict-prototypes -c floyd-parallel.c
	mpicc -w -o floyd-parallel floyd-parallel.o graph.o MyMPI.o -lrt

clean:
	$(RM) graph.o make-graph.o make-graph print-graph.o print-graph floyd-serial.o floyd-serial floyd-parallel.o floyd-parallel MyMPI.o MyMPI floyd-serial.o* floyd-parallel.o*
