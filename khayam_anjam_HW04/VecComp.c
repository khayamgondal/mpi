/* THIS program will compare two vectors
    AUTHOR KHAYAM ANJAM
    Version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "graph.h"
#include <math.h>

double RMS_V_cmp(int n, double *x, double *y)
{
   double sum = 0.0;
   int i;
   for (i = 0; i < n; i++)   {
     double d;
     d = y[i] - x[i];
     sum += d * d;
    }
    return abs(sqrt(sum));
}

void main(int argc, char *argv[]) {
  if (argc != 3) { 
    printf ("usage : %s <input vector 1> <input vector 2> \n", argv[0]);
    exit(EXIT_FAILURE);
    }
  char *vec1_file = argv[1];
  char *vec2_file = argv[2]; 
  
  double **V1;
  double **V2;
  int n,m, vn, vm =0;
  int i,j,k;
  read_graph(vec1_file, &n, &m, &V1);
  read_graph(vec2_file, &vn, &vm, &V2);
  double result = RMS_V_cmp(n, V1[0], V2[0]);
  printf ("result from Compare %f \n", result/n);
  }