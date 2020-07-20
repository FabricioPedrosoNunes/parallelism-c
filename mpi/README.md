#### MPI Friendly Numbers Algorithm

To run this algorithm on linux:

Use this command to compile it:
`mpicc friendlynumbers.c -o exe_name`

To run it, you'll need to use the command mpirun and pass the start and end numbers.
The number of threads should be informed by the parameter -np in mpirun when you execute.

Example:
`mpirun -np 8 exe_name 1 100000`
The example above will run from number 1 to 100000 using 8 threads.

If you want to see how long did it take to run, use command "time" in front of it
Example:
`time mpirun -np 8 exe_name 1 100000`

Obs.: you can use this algorithm in more than one computer, if you want to do it, you can use the parameter -hostfiles. I strongly recommend the MPI documentation to further details.