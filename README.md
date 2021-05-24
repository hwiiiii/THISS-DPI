# THISS for DPI 
Comparing Verilog RTL Calculation Output with THISS RISC-V Instruction Set Simulator 
(RV64IM)

***
***

## Requirement

## svdpi.h

download header file svdpi.h
include this header to c code

```
#include <your path/svdpi.h>

```

### Ubuntu

1. Install libelf library


```
sudo apt-get install libelf-dev
```

*** 
*** 


## example code
```
 #include <stdio.h>
 #include <stdlib.h>
 #include </home1/wjkim/dsal/CPU_CORE_DES/alu_final/sim/svdpi.h>

 int hello() {
     int a=1;
     int b=3;
     int c=a+b;
     printf("Hello world!");
     return c;
 }
```
### run simulator

 1. check alu_rtl.f

```
vi alu_rtl.f
```
 2. change name of c file 

 2. add your c code in .f file


 3. compile with xrun

```
xrun -f alu_rtl.f

```

When you got library error, use Makefile
 
```
make
```
and rerun xrun

***
***


