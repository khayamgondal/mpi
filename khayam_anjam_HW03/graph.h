#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <stdio.h>
int read_graph(char *file_name, int *n, int ***A);
int write_graph (char *file_name, int n, int **A);
//int write_graph_line (char *file_name, FILE *fileHandler,int n, int *A);
int print_graph(int n, int **A);
#endif
