# Many Objects 

Optimized simulation of many spheres on CPU.

## Build

Use `make` with the name of the program you wish to create.
If no name is provided will create `bin/csv`.

## `bin/csv`

`bin/csv` outputs a `csv` of 20 seconds of simulation.
If a filename is not provided `bin/csv` outputs to standard output. 
If you use `-e` flag output only first and last frame of simulation;
it will also print the time the simulation to standard error. 

## `bin/vid`

`bin/vid` turns a `csv` from `bin/csv` into `mp4`. 
First argument is the source `csv`.
If this argument is not provided default to standard input.
Second argument is the output `mp4`.
If this argument is not provided default to `vid.mp4`.

## `bin/wnd`
`bin/wnd` opens window with a continuous simulation.
