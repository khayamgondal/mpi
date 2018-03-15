/*
This file will read a graph of n*n size
AUTHOR KHAYAM ANJAM
Version 1.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "graph.h"

int main(int argc, char *argv[]) {
  char *filename;
  double **A;
  int n=0;
  int m=0;
  int opt;
	while((opt=getopt(argc, argv, "i:"))!=-1){
		switch(opt){
			case 'i':
				filename=optarg;
				break;
			default :
			  printf("Usage: %s [--i file to read]\n", argv[0]);
 		    exit(EXIT_FAILURE);	
				break;
		}
   }
   int errorHandler = read_graph(filename, &n, &m, &A);
   if (errorHandler == 0) printf("reading graph from file %s \n", filename);
   else exit(EXIT_FAILURE);	
   print_graph(n,m, A);
   
   free(A);
   return 0;
	}
