strassen-winograd
=================

Strassen and Winograd algorithms for efficient matrix multiplication

Original code by https://github.com/pmkravets/strassen-winograd, adapted to RISCV32 and ORCA by Alexandre Amory.

Compile to Host
====


```
$ ./comp.sh
$ ./winograd
starting to alloc 64x64 matrix ...
allocated!
naive done!
winograd done!
strassen done!
{"N" : 64, "naive" : 0.001337, "winograd" : 0.001231, "strassen" : 0.001030}

starting to alloc 128x128 matrix ...
allocated!
naive done!
winograd done!
strassen done!
{"N" : 128, "naive" : 0.008649, "winograd" : 0.009671, "strassen" : 0.007249}

starting to alloc 256x256 matrix ...
allocated!
naive done!
winograd done!
strassen done!
{"N" : 256, "naive" : 0.085046, "winograd" : 0.097739, "strassen" : 0.046168}

starting to alloc 512x512 matrix ...
allocated!
naive done!
winograd done!
strassen done!
{"N" : 512, "naive" : 0.639586, "winograd" : 0.751948, "strassen" : 0.323980}
```


Compiling to ORCA
----

Edit Makefile-Orca to set the variables ORCA_SIM_DIR and ORCA_SW_DIR to point to 
[orca-sim](https://github.com/andersondomingues/orca-sim) and [orca-software-tools](https://github.com/andersondomingues/orca-software-tools). Make sure that orca-sim is compiled to single-core platform.


```
make -f Makefile-Orca all
mkdir logs
```

it will generate image.bin. Then run 

```
$ORCA_SIM_DIR/bin/single-core.exe image.bin
```

The define --defsym=HEAP_SIZE=0x100000 (1MBytes) in Makefile-Orca-App was increased to allow more allocated memory.


After a while (about 1 minute), check the results

```
cat logs/core\!.cpu_debug.log
starting to alloc 16x16 matrix ...
allocated!
naive done!
winograd done!
strassen done!
{"N" : 16, "naive" : 1369234, "winograd" : 1316880, "strassen" : 2614733}

starting to alloc 32x32 matrix ...
allocated!
naive done!
winograd done!
strassen done!
{"N" : 32, "naive" : 10947128, "winograd" : 10039553, "strassen" : 10947188}

starting to alloc 64x64 matrix ...
allocated!
naive done!
winograd done!
strassen done!
{"N" : 64, "naive" : 87517262, "winograd" : 78222163, "strassen" : 80246836}
```
