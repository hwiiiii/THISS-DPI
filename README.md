# THISS - SYISS Ver.2 for RV64IM
RISC-V Instruction Set Simulator 
(march = rv32im / mabi = ilp32)

***
***

## Requirement

### Ubuntu

1. Install libelf library


```
sudo apt-get install libelf-dev
```

*** 
*** 


## Install

### 1. go to your syiss repository

```
cd ./syiss
```

### 2. install
```
make
```
###### Ignore warings

***
***

## Tutorial

### 1. go to your syiss repository

```
cd ./syiss
```

### 2. go to testcode directory

 	cd ./testcode/testsrc

### 3. create your testcode

 	vi hello.c
 
 ***
> hello.c
> 
>	 	#include <stdio.h>
>	 
>	 	int main()
>	 
>	 	{
>	 		printf("Hello Sung Yeon!\n");
>	 
>	 		return 0;
>	 	}
 
 ***


### 3. include "common.h" in your testcode

 this header file is for :
  1. Passing commandline argument to program
 	 
 ***
> hello.c
> 
>	 	#include <stdio.h>
>	 
>	 	#ifdef SYISS
>	 	#include "../src/common.h"
>	 	#endif
>	 
>	 	int main()
>	 
>	 	{
>	 		printf("Hello Sung Yeon!\n");
>	 		return 0;
>	 	}
 
 
 ***

### 4. Makefile

  1. Open Makefile

 	cd ..
 
 	vi Makefile
 
  2. change TARGET to your testcode's name

***
> Makefile
> 
>	 	TARGET = hello

***

### 5. compile

```
make
```

#### _Thanks to SiFive, prebuilt RISC-V Toolchain is used for this compile process._
##### _Download available at : [SiFive](https://www.sifive.com/boards "semifive link")_

### 6. run simulator

 1. Go back to syiss directory


```
cd ..
```


 2. run simulator


```
./bin/syiss ./testcode/app/program.bin
```


or

```
./run
```

 You may reference any 'run' file or simply run simulator without arguments for help

***
***


