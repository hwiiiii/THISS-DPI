SHELL:=/bin/bash -O extglob -c

#TARGET	= commandline
#TARGET	= My_JPEG
#TARGET	= test
TARGET	= arith

TARGET_LIST = commandline My_JPEG test hello

####################################################################

ISA = 32
COMPILE_OPTION = -march=rv$(ISA)im -mabi=ilp$(ISA)

COMPILER = riscv64-unknown-elf


LINKSCRIPT = link
STARTCODE = start

WORK_DIR = .
SRC_DIR = $(WORK_DIR)/src
TST_DIR = $(WORK_DIR)/testsrc
OBJ_DIR = $(WORK_DIR)/obj
BIN_DIR = $(WORK_DIR)/bin
APP_DIR = $(WORK_DIR)/app
FILE =	$(SRC_DIR)/$(TARGET).c \
		$(SRC_DIR)/retarget.c

SPIKE_SRCS =	$(TST_DIR)/$(TARGET).c 
SPIKE_OBJS =	$(OBJ_DIR)/SPIKE_$(TARGET).o 

SRCS =			$(TST_DIR)/$(TARGET).c \
				$(SRC_DIR)/retarget.c \
				$(SRC_DIR)/common.c \
				$(SRC_DIR)/$(STARTCODE).S

OBJS =			$(OBJ_DIR)/$(TARGET).o \
				$(OBJ_DIR)/retarget.o \
				$(OBJ_DIR)/common.o \
				$(SRC_DIR)/$(STARTCODE).S

all : SPIKE SYISS

SPIKE: $(SPIKE_SRCS)
	$(COMPILER)-gcc $(COMPILE_OPTION) -c $(TST_DIR)/$(TARGET).c -o $(OBJ_DIR)/SPIKE_$(TARGET).o 
	$(COMPILER)-gcc $(COMPILE_OPTION) $(SPIKE_OBJS) -o $(BIN_DIR)/SPIKE_$(TARGET) -Wl,"-Map=$(OBJ_DIR)/SPIKE_$(TARGET).map" -save-temps=obj
	$(COMPILER)-objdump -h $(BIN_DIR)/SPIKE_$(TARGET) > $(OBJ_DIR)/SPIKE_$(TARGET).section
	$(COMPILER)-objdump -t $(BIN_DIR)/SPIKE_$(TARGET) > $(OBJ_DIR)/SPIKE_$(TARGET).symbol
	$(COMPILER)-objdump -D $(BIN_DIR)/SPIKE_$(TARGET) > $(OBJ_DIR)/SPIKE_$(TARGET).lst
	$(COMPILER)-objcopy -S $(BIN_DIR)/SPIKE_$(TARGET) -O verilog $(OBJ_DIR)/SPIKE_$(TARGET).hex
	cp $(BIN_DIR)/SPIKE_$(TARGET) $(APP_DIR)/SPIKE_program.bin
	cp $(OBJ_DIR)/SPIKE_$(TARGET).section $(APP_DIR)/SPIKE_program.section
	cp $(OBJ_DIR)/SPIKE_$(TARGET).symbol $(APP_DIR)/SPIKE_program.symbol
	cp $(OBJ_DIR)/SPIKE_$(TARGET).lst $(APP_DIR)/SPIKE_program.lst
	
SYISS: $(SRCS)
	$(COMPILER)-gcc $(COMPILE_OPTION) -c $(SRC_DIR)/retarget.c -o $(OBJ_DIR)/retarget.o -DSYISS
	$(COMPILER)-gcc $(COMPILE_OPTION) -c $(SRC_DIR)/common.c -o $(OBJ_DIR)/common.o -DSYISS
	$(COMPILER)-gcc $(COMPILE_OPTION) -c $(TST_DIR)/$(TARGET).c -o $(OBJ_DIR)/$(TARGET).o -DSYISS 
	$(COMPILER)-gcc $(COMPILE_OPTION) $(OBJS) -o $(BIN_DIR)/$(TARGET) -DSYISS -Wl,"-Map=$(OBJ_DIR)/$(TARGET).map" -save-temps=obj \
		-nostartfiles -T $(SRC_DIR)/$(LINKSCRIPT).ld
	$(COMPILER)-objdump -h $(BIN_DIR)/$(TARGET) > $(OBJ_DIR)/$(TARGET).section
	$(COMPILER)-objdump -t $(BIN_DIR)/$(TARGET) > $(OBJ_DIR)/$(TARGET).symbol
	$(COMPILER)-objdump -D $(BIN_DIR)/$(TARGET) > $(OBJ_DIR)/$(TARGET).lst
	$(COMPILER)-objcopy -S $(BIN_DIR)/$(TARGET) -O verilog $(OBJ_DIR)/$(TARGET).hex
	cp $(BIN_DIR)/$(TARGET) $(APP_DIR)/program.bin
	cp $(OBJ_DIR)/$(TARGET).section $(APP_DIR)/program.section
	cp $(OBJ_DIR)/$(TARGET).symbol $(APP_DIR)/program.symbol
	cp $(OBJ_DIR)/$(TARGET).lst $(APP_DIR)/program.lst

clean:
	rm -vf $(OBJ_DIR)/!(*.md)
	rm -vf $(BIN_DIR)/!(*.md)
