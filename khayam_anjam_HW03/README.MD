Make sure you add openmpi/1.6.4 before doing make
Type 'make' to Make the project
./make-graph -n 180  -r 100 -p 150 -o in.dat // Creats a graph
./print-graph -i in.dat                      // Prints the graph
./floyd-serial -i in.dat -o out.seq          // Runs floyd in serial
mpiexec -np 24 floyd-parallel -i in.dat -o out.seq // Parallel


PBS for serial & parallel are also provided. PBS scripts will write in ouput/ folder.


