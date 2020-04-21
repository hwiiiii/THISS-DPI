# syiss
RISC-V Instruction Simulator 
(march = rv32im / mabi = ilp32)


## Requirement

### Ubuntu

1. Install libelf library


```
sudo apt-get install libelf-dev
```


## Install

	make

## Tutorial

### 1. go to testcode directory

 	cd ./testcode/testsrc

### 2. create your testcode

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


### 3. include "common.h"

 this header file is for :
  1. Passing commandline argument to program
  2. Marking end of program
 
 	 
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
>	 		
>	 	#ifdef SYISS
>	 	end_main()
>	 	#endif
>
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
