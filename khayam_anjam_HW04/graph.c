#include "graph.h"
#include<stdio.h>

int write_vector (char *file_name, int n, int m, double *A) {

  FILE *fileHandler = fopen(file_name, "wb");
  if (!fileHandler) return -1;
  fwrite(&n,sizeof(int),1,fileHandler); // write rows first
  fwrite(&m,sizeof(int),1,fileHandler); // write cols
  int i;
  for(i=0;i<n;i++){    
		fwrite(&A[i],sizeof(double),m,fileHandler);
    }
  fclose(fileHandler);
  return 0;
}

int write_graph (char *file_name, int n, int m, double **A) {

  FILE *fileHandler = fopen(file_name, "wb");
  if (!fileHandler) return -1;
  fwrite(&n,sizeof(int),1,fileHandler); // write rows first
  fwrite(&m,sizeof(int),1,fileHandler); // write cols
  int i;
  printf(" %d %d %f \n", n,m, A[0][0]);
  for(i=0;i<n;i++){
		fwrite(A[i],sizeof(double),m,fileHandler);
	}
  fclose(fileHandler);
  return 0;
}

int read_graph(char *file_name, int *n, int *m, double ***A) { 

  FILE *fileHandler = fopen(file_name, "rb");
  if (!fileHandler) return -1;
  fread(n, sizeof(int), 1, fileHandler); // read rows
  fread(m, sizeof(int), 1, fileHandler); // read cols
  
	double *strg=(double*)malloc((*n)*(*m)*sizeof(double));
	*A=(double**)malloc(*n*sizeof(double*));
 	int i;
	for(i=0;i<*n;i++){
		(*A)[i]=strg+i*(*m);
		fread((*A)[i],sizeof(double),*m,fileHandler);
	}
 fclose(fileHandler);
 return 0;
}  

int print_graph(int n, int m, double **A) {
  int i,j;
	printf("Array is a %d x %d matrix\n\n",n,m);
  for (i = 0; i < n; i++) {
    for (j = 0; j < m ; j++)
         printf ("%f  ", A[i][j]);
    printf("\n");
   }
 return 0;
}


