/*
This file will calulate shortest paths in serial fasion.
AUTHOR KHAYAM ANJAM
Version 1.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "graph.h"

void main(int argc, char *argv[]) {
  char *in,*out;
 	int opt;
  int **A;
  int n;
  int i,j,k;
  struct timeval totalTime, procTime, procTimeStop;
  
  	while ((opt = getopt(argc, argv, "i:o:")) != -1) {
		switch(opt) {
			case 'i':
				in = optarg;
				break;
			case 'o':
				out = optarg;
				break;
			default: /* '?' */
	  		printf("Usage: %s [-i input file] [-o output file]\n", argv[0]);
 		    exit(EXIT_FAILURE);	
        break;	
		}	
	}

 gettimeofday(&totalTime,NULL);
 int errorHandler = read_graph(in, &n, &A);
 if (errorHandler == 0) printf("reading graph from file %s \n", in);
 else exit(EXIT_FAILURE);	
 
 //clock_gettime(CLOCK_MONOTONIC,&procTime); // start the processing time clock
 gettimeofday(&procTime, NULL);
 //Run floyd .. I have taken this piece from lecture slides.
 for(k=0;k<n;k++){
    for(i=0;i<n;i++){
      for(j=0;j<n;j++){
        if(A[i][k]==-1||A[k][j]==-1)  continue; // no route through K
			  if(A[i][j]>A[i][k]+A[k][j] ||A[i][j]==-1)  A[i][j]=A[i][k]+A[k][j];				
			}
		}
	}
  
  gettimeofday(&procTimeStop,NULL);
  errorHandler = write_graph(out, n , A);
  if (errorHandler == 0 ) printf("Writting graph to file %s \n", out);
  
  struct timeval totalTimeStop;
  gettimeofday(&totalTimeStop,NULL);
  
  printf("floyd-serial execution time:\n");
  printf("  n = %d nodes\n", n);
  printf("  p = 1 cpus\n");
  
  printf("  ptime = %f (sec)\n",((totalTimeStop.tv_sec - totalTime.tv_sec) + (totalTimeStop.tv_usec - totalTime.tv_usec)*1e-9));  
  printf("  stime = %f (sec)\n", ((procTimeStop.tv_sec - procTime.tv_sec) + (procTimeStop.tv_usec - procTime.tv_usec)*1e-9));

  free (A);
 
}
