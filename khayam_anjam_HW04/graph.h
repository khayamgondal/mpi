#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <stdio.h>
int read_graph(char *file_name, int *n, int *m, double ***A);
int write_graph (char *file_name, int n, int m,  double **A);
int print_graph(int n, int m, double **A);
#endif
