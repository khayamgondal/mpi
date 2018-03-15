/*
This file will calulate shortest paths in parallel.
AUTHOR KHAYAM ANJAM
Version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include "graph.h"
#include "MyMPI.h"


int main(int argc, char *argv[]) {
  char *in,*out;
 	int opt;
  int rank,size;
  FILE *fileHandler;
  while ((opt = getopt(argc, argv, "i:o:")) != -1) {
		switch(opt) {
			case 'i':
				in = optarg;
				break;
			case 'o':
				out = optarg;
				break;
			default:
	  		printf("Usage: %s [-i input file] [-o output file]\n", argv[0]);
 		    exit(EXIT_FAILURE);	
        break;	
		}	
	}
 
 	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
	MPI_Comm_size (MPI_COMM_WORLD, &size);  
  
 	MPI_Barrier(MPI_COMM_WORLD);
	double entire_start=MPI_Wtime(); //start entire timer
 
  int dims[2] = {0,0};
  int periods[2] = {0,0}; //we have square
  
  MPI_Comm comm;
 	MPI_Comm row_comm;
	MPI_Comm col_comm;
 
  MPI_Dims_create(size,2,dims); // Get dimensions for topology
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm);
 
  void **A;
  int m,n;
  void *storage;  
	int row_dims[2] = {1,0};
  int col_dims[2] = {0,1};
  
  MPI_Cart_sub(comm, row_dims, &row_comm); // get the row comunicator
	MPI_Cart_sub(comm, col_dims, &col_comm); // get the col communicator
  
  read_checkerboard_matrix (
		in,		/* IN - File name */
		&A,         	/* OUT - 2D array */
		&storage,       /* OUT - Array elements */
		MPI_INT,
    &m,   	/* out array rows */
		&n,	        /* OUT - Array cols */
		comm);	
 
 	MPI_Barrier(MPI_COMM_WORLD);
	double comp_start=MPI_Wtime(); // computational time  
 
 	int coord[2];
	MPI_Cart_coords(comm, rank, 2, coord);
  
 	int rstart=BLOCK_LOW(coord[0],dims[0],n); // from where row starts for this process
	int cstart=BLOCK_LOW(coord[1],dims[1],n);  	
  int local_rows=BLOCK_SIZE(coord[0],dims[0],n);
	int local_cols=BLOCK_SIZE(coord[1],dims[1],n);
 
  int *rbuf=(int*)malloc(sizeof(int)*local_cols); //buffer to hold temp K row(row size is equal to local cols). it will be broadcasted acrros the K column
 	int *cbuf=(int*)malloc(sizeof(int)*local_rows);
  
  int root_row;
	int root_col;
	int i,j,k;
 
	for(k=0;k<n;k++){
		root_row=BLOCK_OWNER(k,dims[0],n); // which processor have the K row
		root_col=BLOCK_OWNER(k,dims[1],n); //which processor have the K col
   
    // If I owe the K, write that row in my own buffer and broadcast it in column
    // k-rstart will give the local row number inside that local block, 
		if(root_row==coord[0]){
			memcpy(rbuf, ((int**)A)[k-rstart], local_cols*sizeof(int)); // copy into rbuf
		}
		if(root_col==coord[1]){ // need to write the col ............
			int col_counter;
			for(col_counter=0;col_counter<local_rows;col_counter++){ // local_rows give us the lenght of local col. iterating over 0 to total rows will give us the whole column.
				cbuf[col_counter]=((int**)A)[col_counter][k-cstart]; //write it into local buffer,
			}
		}
     
		MPI_Bcast(rbuf,local_cols,MPI_INT,root_row,row_comm);
		MPI_Bcast(cbuf,local_rows,MPI_INT,root_col,col_comm);
  
		for(i=0;i<local_rows;i++){
			for(j=0;j<local_cols;j++){
				if(cbuf[i]==-1||rbuf[j]==-1) continue; // no route through K
				if(((int**)A)[i][j]>cbuf[i]+rbuf[j]||((int**)A)[i][j]==-1){
					((int**)A)[i][j]=cbuf[i]+rbuf[j];
				}
			}
		}
	}
 
	MPI_Barrier(MPI_COMM_WORLD);
	double comp_stop=MPI_Wtime();
  
  if (rank == 0) {
  fileHandler = fopen(out, "wb");
  if (!fileHandler) return -1;
  fwrite(&n,sizeof(int),1,fileHandler); // write rows first
  fwrite(&n,sizeof(int),1,fileHandler); // write cols
  printf("Writing to %s \n", out);
  }
  // I made changes into print function. Now it will save output into file
 	print_checkerboard_matrix (
		A, 
    fileHandler,          
		MPI_INT,        
		n,     
    n,     
	  comm   	
     );	
   

  
	MPI_Barrier(MPI_COMM_WORLD);
	double entire_stop=MPI_Wtime();
 
  if(rank==0){
		printf("floyd-parallel execution time:\n");
		printf("\tn = %d nodes\n\tp = %d cpus\n",n,size);
		printf("\tptime = %f (sec)\n\tftime = %f (sec)\n",
			entire_stop-entire_start,
			comp_stop-comp_start);
		fflush(stdout);
	}
  
  free(A);
  free(storage);
  
  MPI_Finalize(); 
  return 0;
}
