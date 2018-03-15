/* THIS program will do a multiply 
    on a upper reduced matrix 
    AUTHOR KHAYAM ANJAM
    Version 1.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "graph.h"

void main(int argc, char *argv[]) {
  if (argc != 4) { 
    printf ("usage : %s <input mat> <input vec> <output vec> \n", argv[0]);
    exit(EXIT_FAILURE);
    }
  char *mat_file = argv[1];
  char *vec_file = argv[2]; 
  char *out_vec_file = argv[3];
  
  double **A;
  double **V;
  int n,m, vn, vm =0;
  int i,j,k;
  int errorHandler = read_graph(mat_file, &n, &m, &A);
  if (errorHandler == 0) printf("reading matrix from file %s \n", mat_file);
  else { printf("ERROR reading matrix from file %s \n", mat_file); exit(EXIT_FAILURE); }
  
  errorHandler = read_graph(vec_file, &vn, &vm, &V);
  if (errorHandler == 0) printf("reading vector from file %s \n", vec_file);
  else { printf("ERROR reading vector from file %s \n", vec_file); exit(EXIT_FAILURE);}
  if (n != m ) {
    printf("rows and cols of matrix not equal... exiting"); 
    exit(EXIT_FAILURE);
  }
  if (vm != 1) {
    printf("vector have more than 1 col... exiting"); 
    exit(EXIT_FAILURE);
  }
  if (n != vn ) {
    printf("vector & matrix don't have equal rows... exiting"); 
    exit(EXIT_FAILURE);
  }
      // Multiplying matrices a and b and
    // storing result in result matrix
  double result[n];
  for(i=0; i<n; i++)
    for(j=0; j<vm; j++)
      for(k=0; k<m; k++) {
        result[i]+=A[i][k]*V[k][j];
        }
   write_vector (out_vec_file,  n, 1, &result);
   printf("Writing vector back to file ... done\n");
}