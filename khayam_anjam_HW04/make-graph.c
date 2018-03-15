/*
This file will generate a graph of n*n size
AUTHOR KHAYAM ANJAM
Version 1.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "graph.h"
#include <math.h>

int **makeGraph(int, int, double, double);
double randGen(double, double);

int main(int argc, char *argv[]) {
	int opt;
	int n,m;
  double r, p;
  char *o;
	srand((unsigned)time(NULL)); // seed random number generator 
	
  while ((opt = getopt(argc, argv, "n:m:r:p:o:")) != -1) {
		switch(opt) {
			case 'n':
				n = atoi(optarg);
				break;
      case 'm':
        m = atoi(optarg);
        break;
			case 'r':
				r = atof(optarg);
				break;
			case 'p':
				p = atof(optarg);
				break;
 			case 'o':
				o = optarg;
				break;
			default: /* '?' */
			printf("Usage: %s [-n rows] [-m cols] [-r r] [-p p] [-o output]\n", argv[0]);
            		exit(EXIT_FAILURE);		
		}	
	}

    double **A = makeGraph(n, m, r, p);
		int errorHandler = write_graph(o, n, m , A);
    if (errorHandler == 0 ) printf("Writting matix to file %s \n", o);
  return 0;
}

int **makeGraph(int n, int m, double r, double p) {

  int rix;
  double *storage, **array;
	storage = (double *) malloc (n * m * sizeof(double));
	array = (double **) malloc (n * sizeof(double *));
	for (rix = 0; rix < n; rix++) {
	array[rix] = &storage[rix * m];
	}
  int i, j;
	for (i=0 ; i <n; i++) {
		for (j=0; j < m; j++) {
      //if (i == j) array[i][j] = 0;
      //else
      	array[i][j] = randGen(p,r);
		}
	}
	return array;
}

double randGen(double p, double r) {

  //double u = fmod((double) rand(), p);
  //if (u <= r) 
  double u=((double)rand()/(double)p);

  return u;
	//else return -1;
}


