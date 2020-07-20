#### CUDA Friendly Numbers Algorithm

To run this algorithm on linux: 

Use this command to compile it:

    nvcc friendlynumbers.cu -o exe_name

To run it, you'll need to pass the start and end number.

Example:

    ./exe_name 1 100000

The example above will run from number 1 to 100000.

If you want to see how long did it take to run, use command "time" in front of it.

Example:

    time ./exe_name 1 100000
