/*
This file will solve LES in parallel using gauss 
AUTHOR KHAYAM ANJAM
Version 1.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include "graph.h"

int val;

write_rows(char * file_name, int n, int m, int rank, int size, double **a, int *to_read) {
 if (n % size > rank)
   to_read = n/size +1 ;
 else
   to_read = n/size;
 
  val = to_read; // I was not able to pass to_read directly to malloc. No idea
  to_read = val;
  
  //printf("\n in fun %d  %d \n", to_read, n);
  int i;
  FILE *file = fopen(file_name, "r+");

  for (i=0; i < val ; i++) {
      int global_row_number = rank + size * i;
      fseek(file, ((global_row_number * m) + 1 ) * sizeof(double), SEEK_SET);
      //(*a)[i]=strg+i*n;
      fwrite((a)[i],sizeof(double),m,file);
  }
  fclose(file);
}

read_rows(char * file_name, int n, int m, int rank, int size, double ***a, int *to_read) {
 if (n % size > rank)
   to_read = n/size +1 ;
 else
   to_read = n/size;
 
  val = to_read; // I was not able to pass to_read directly to malloc. No idea
  to_read = val;
  
  //printf("\n in fun %d  %d \n", to_read, n);
  int i;
  FILE *file = fopen(file_name, "rb");

  double *strg=(double*) malloc( val * (m) * (sizeof(double)) );
  *a=(double**) malloc((val) * sizeof(double*));
  for (i=0; i < val ; i++) {
      int global_row_number = rank + size * i;
      fseek(file, ((global_row_number * m) + 1 ) * sizeof(double), SEEK_SET);
      (*a)[i]=strg+i*m;
      fread((*a)[i],sizeof(double),m,file);
  }
  fclose(file);
}

void main(int argc, char *argv[]) {
  if (argc != 5) { 
    printf ("usage : %s <input mat> <input vec> <output mat> <output vec>\n", argv[0]);
    exit(EXIT_FAILURE);
    }
  char *mat_file = argv[1];
  char *vec_file = argv[2]; 
  char *mat_out_file = argv[3];
  char *vec_out_file = argv[4];
  FILE *fileHandler;
  
  double **A;
  double **V;
    
  int n,m, vn, vm =0;
  int rank, size;
  int i,j,k;
  
 	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
	MPI_Comm_size (MPI_COMM_WORLD, &size);  
  
 	MPI_Barrier(MPI_COMM_WORLD);
	double total_start=MPI_Wtime(); 
  
  if (rank == 0) { //readout the matrix dimenions.
    fileHandler = fopen(mat_file, "rb");
    if (!fileHandler) {
      printf("Error reading mat");
      exit(EXIT_FAILURE); 
      }
    fread(&n, sizeof(int), 1, fileHandler); // read rows
    fread(&m, sizeof(int), 1, fileHandler); // read cols
    close(fileHandler);
    
    fileHandler = fopen(vec_file, "rb");
    if (!fileHandler) {
      printf("Error reading vector");
      exit(EXIT_FAILURE); 
      }
    fread(&vn, sizeof(int), 1, fileHandler); // read rows
    fread(&vm, sizeof(int), 1, fileHandler); // read cols
    close(fileHandler);
    
    if (n != m ) {
      printf("\nRows and cols of matrix not equal... \n\n"); 
      //MPI_Finalize(); 
      //exit(EXIT_FAILURE);
    }
    if (vm != 1) {
      printf("vector have more than 1 col... exiting"); 
      exit(EXIT_FAILURE);
    }
    if (n != vn ) {
      printf("vector & matrix don't have equal rows... exiting"); 
      exit(EXIT_FAILURE);
    }
  }
 	MPI_Barrier(MPI_COMM_WORLD);
	double elem_start=MPI_Wtime(); 
 
  // Now broadcast N to other procs so they can read their matrix
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  double **a;  
  double **v;
  int to_read;

  read_rows(mat_file ,n, m, rank, size, &a, &to_read); 
  read_rows(vec_file ,n, 1, rank, size, &v, &to_read); 
  
  if (rank ==0) printf("Reading matrix and vector ... done \n");
  
  MPI_Request index_request, row_request, vec_request;
  int size_loop;
  int local_row_no = 0; // next time I have to send next local row
  for (size_loop=0; size_loop<n; size_loop++) {
    int main_sender = size_loop % size; // Ring of processors. 
    int last_receiver = (size + main_sender -1)%size;

    int pivot_index;
    double *rbuf=(double*)malloc(sizeof(double)*n); 
    double vec;
    int loc[n];
    for (i=0;i<n;i++) loc[i]=i;
    ///////////////
    if (rank == main_sender) { //just send
      for (j=0; j < n; j++)   rbuf[j] = a[local_row_no][j]; //HAVE TO SEND entire ROW to next proc. 
      pivot_index = -1;
      /*double max = -1000;
      for (j=size_loop; j < n; j++) { // column pivoting
          if (a[local_row_no][j] > max) { 
            max = a[local_row_no][j];  
            pivot_index = j; 
            }
        }
      rbuf[pivot_index] = rbuf[size_loop];
      rbuf[size_loop] = max; */
      
      pivot_index = size_loop; 
      vec = v[local_row_no][0];
      
      MPI_Isend (&pivot_index, 1, MPI_INT, (rank+size+1)%size, 1, MPI_COMM_WORLD, &index_request);
      MPI_Isend (rbuf, n, MPI_DOUBLE, (rank+size+1)%size, 2, MPI_COMM_WORLD, &row_request);
      MPI_Isend (&vec, 1, MPI_DOUBLE, (rank+size+1)%size, 3, MPI_COMM_WORLD, &vec_request);
  
      for (k=local_row_no+1; k < val ; k++) {
        double t=a[k][pivot_index]/rbuf[pivot_index];
        for (j=0; j<n; j++) {
          a[k][j]=a[k][j]-t*rbuf[j]; 
        }
        v[k][0] = v[k][0]-t*vec; 
      }
      local_row_no++;
    }
    else if (rank == last_receiver) { // just receive
      MPI_Recv (&pivot_index, 1, MPI_INT, (rank+size-1)%size, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv (rbuf, n, MPI_DOUBLE, (rank+size-1)%size, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv (&vec, 1, MPI_DOUBLE, (rank+size-1)%size, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
      
      for (k=local_row_no; k < val ; k++) {
        double t=a[k][pivot_index]/rbuf[pivot_index];
        for (j=0; j<n; j++) {
          a[k][j]=a[k][j]-t*rbuf[j]; 
        }
        v[k][0] = v[k][0]-t*vec;
      }
    }
    
    else { //send to next receive from previous
      MPI_Recv (&pivot_index, 1, MPI_INT, (rank+size-1)%size, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv (rbuf, n , MPI_DOUBLE, (rank+size-1)%size, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv (&vec, 1, MPI_DOUBLE, (rank+size-1)%size, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      
      MPI_Isend (&pivot_index, 1, MPI_INT, (rank+size+1)%size, 1, MPI_COMM_WORLD, &index_request);
      MPI_Isend (rbuf, n, MPI_DOUBLE, (rank+size+1)%size, 2, MPI_COMM_WORLD, &row_request);
      MPI_Isend (&vec, 1, MPI_DOUBLE, (rank+size+1)%size, 3, MPI_COMM_WORLD, &vec_request);
      
      // Now reduce my set
      for (k=local_row_no; k < val ; k++) {
        double t=a[k][pivot_index]/rbuf[pivot_index];
        for (j=0; j<n; j++) {
          a[k][j]=a[k][j]-t*rbuf[j]; 
        }
        v[k][0] = v[k][0]-t*vec;
      }
    }
  }
  if (rank ==0) printf("Gauss Elmination ... done \n");
 	MPI_Barrier(MPI_COMM_WORLD);
	double elem_stop=MPI_Wtime(); 
  
  if  (rank ==0) { // write n and m into file
      FILE *fileHandler = fopen(mat_out_file, "wb");
      if (!fileHandler) return -1;
      fwrite(&n,sizeof(int),1,fileHandler); // write rows first
      fwrite(&m,sizeof(int),1,fileHandler); // write cols
      
      fileHandler = fopen(vec_out_file, "wb");
      if (!fileHandler) return -1;
      fwrite(&n,sizeof(int),1,fileHandler); // write rows first
      fwrite(&vm,sizeof(int),1,fileHandler); // write cols
      
      fclose(fileHandler);
  }

  write_rows(mat_out_file ,n, m, rank, size, a, &to_read);
  write_rows(vec_out_file ,n, 1, rank, size, v, &to_read);
  
  if (rank ==0) printf("Writing matrix and vector back to file ... done \n");
 	
  MPI_Barrier(MPI_COMM_WORLD);
	double total_stop=MPI_Wtime(); 
 
  if(rank==0){
		printf("gauss-parallel execution time:\n");
		printf("\tn = %d nodes\n\tp = %d cpus\n",n,size);
		printf("\tptime = %f (sec)\n\tftime = %f (sec)\n",
			total_stop-total_start,
			elem_stop-elem_start);
		fflush(stdout);
	}
 
  MPI_Finalize(); 
  return 0;
  }
