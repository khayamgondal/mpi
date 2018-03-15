#include "graph.h"
#include<stdio.h>

int write_graph (char *file_name, int n, int **A) {

  FILE *fileHandler = fopen(file_name, "wb");
  if (!fileHandler) return -1;
  fwrite(&n,sizeof(int),1,fileHandler); // write rows first
  fwrite(&n,sizeof(int),1,fileHandler); // write cols
  int i;
  for(i=0;i<n;i++){
		fwrite(A[i],sizeof(int),n,fileHandler);
	}
  fclose(fileHandler);
  return 0;
}

int read_graph(char *file_name, int *n, int ***A) { 
  int m;
  FILE *fileHandler = fopen(file_name, "rb");
  if (!fileHandler) return -1;
  fread(n, sizeof(int), 1, fileHandler); // read dims
  fread(n, sizeof(int), 1, fileHandler); // read dims
  
	int *strg=(int*)malloc((*n)*(*n)*sizeof(int));
	*A=(int**)malloc(*n*sizeof(int*));
 	int i;
	for(i=0;i<*n;i++){
		(*A)[i]=strg+i*(*n);
		fread((*A)[i],sizeof(int),*n,fileHandler);
     //printf(" %d \n", *A[0][i]);
	}
 fclose(fileHandler);
 return 0;
}  

int print_graph(int n, int **A) {
  int i,j;
	printf("Array is a %d x %d matrix\n\n",n,n);
  for (i = 0; i < n; i++) {
    for (j = 0; j < n ; j++)
         printf ("%4d ", A[i][j]);
    printf("\n");
   }
 return 0;
}


