#### OpenMP Friendly Numbers Algorithm

To run this algorithm on linux: 

Use this command to compile it:

    gcc -o exe_name -fopenmp friendlynumbers.c

To run it, you'll need to pass the start, end and number of threads you want to work with.

Example:

    ./exe_name 1 100000 4

The example above will run from number 1 to 100000 using 4 threads.

If you want to see how long did it take to run, use command "time" in front of it.

Example:

    time ./exe_name 1 100000 4
