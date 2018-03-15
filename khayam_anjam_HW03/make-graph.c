/*
This file will generate a graph of n*n size
AUTHOR KHAYAM ANJAM
Version 1.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "graph.h"

int **makeGraph(int, int, int);
int randGen(int, int);

int main(int argc, char *argv[]) {
	int opt;
	int n,r,p;
  char *o;
	srand ( time(NULL) ); // seed random number generator 
	while ((opt = getopt(argc, argv, "n:r:p:o:")) != -1) {
		switch(opt) {
			case 'n':
				n = atoi(optarg);
				break;
			case 'r':
				r = atoi(optarg);
				break;
			case 'p':
				p = atoi(optarg);
				break;
 			case 'o':
				o = optarg;
				break;
			default: /* '?' */
			printf("Usage: %s [-n size] [-r r] [-p p] [-o output]\n", argv[0]);
            		exit(EXIT_FAILURE);		
		}	
	}
    int **A = makeGraph(n, r, p);
		int errorHandler = write_graph(o, n , A);
    if (errorHandler == 0 ) printf("Writting graph to file %s \n", o);
    
  return 0;
}

int **makeGraph(int n, int r, int p) {

  int rix;
  int *storage, **array;
	storage = (int *) malloc (n * n * sizeof(int));
	array = (int **) malloc (n * sizeof(int *));
	for (rix = 0; rix < n; rix++) {
	array[rix] = &storage[rix * n];
	}
  int i, j;
	for (i=0 ; i <n; i++) {
		for (j=0; j < n; j++) {
      if (i == j) array[i][j] = 0;
      else	array[i][j] = randGen(p,r);
		}
	}
	return array;
}

int randGen(int p, int r) {
  int u = rand()%(p+1);
  if (u <= r) return u;
	else return -1;
}



