# ASCII Collatz Plotter

This program is designed to print a simple ASCII x-y plot for viewing integers and their corresponding Collatz sequence length.

#### Prerequisites

This program uses ```sys/ioctl.h``` to detect terminal window size.

#### Usage

Compile using ```gcc``` with the ```-o``` argument.
```
gcc collatz.c -o Collatz
```

Run on command line with arguments specified as follows:
```
./Collatz <maxSeedVal>
```
where ```Collatz``` will graph the Collatz sequence length of integers in the range ```[1, maxSeedVal]```.