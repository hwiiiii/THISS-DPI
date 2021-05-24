SHELL:=/bin/bash -O extglob -c


CC = gcc -g -Wall

WORK_DIR = ..
SRC_DIR = $(WORK_DIR)/src
OBJ_DIR = $(WORK_DIR)/obj
BIN_DIR = $(WORK_DIR)/bin

#OBJS =	$(OBJ_DIR)/retarget.o \
#		$(OBJ_DIR)/handler.o \
#		$(OBJ_DIR)/instruction.o \
#		$(OBJ_DIR)/proc.o \
#		$(OBJ_DIR)/syiss.o \
#		/usr/lib/x86_64-linux-gnu/libelf.so.1

OBJS =	$(SRC_DIR)/thissmain.c \
		$(SRC_DIR)/handler.c \
		$(SRC_DIR)/instruction.c \
		$(SRC_DIR)/proc.c

$(BIN_DIR)/thiss: $(OBJS)
	#$(CC) -o $(BIN_DIR)/syiss $(OBJS)
		
	$(CC) -Wl,"-Map=$(OBJ_DIR)/syiss.map" $(OBJS) -fpic -shared -o librun.so -lelf
	mv -f librun.so ./xcelium.d/run.lnx8664.19.03.d/librun.so
	
#objdump -h $(BIN_DIR)/syiss > $(BASE_DIR)$(TARGET).section
	#objdump -t $(BIN_DIR)/syiss > $(BASE_DIR)$(TARGET).symbol
	#objdump -S $(BIN_DIR)/syiss > $(BASE_DIR)$(TARGET).lst
		

#$(OBJ_DIR)/proc.o: $(SRC_DIR)/proc.c
#	#$(CC) -L/usr/lib/x86_64-linux-gnu/libelf.a -lelf -o $@ -c $< -lz
#	$(CC) -o $@ -c $<
#
#$(OBJ_DIR)/syiss.o: $(SRC_DIR)/syiss.c
#	$(CC) -o $@ -c $<
#
#$(OBJ_DIR)/handler.o: $(SRC_DIR)/handler.c
#	$(CC) -o $@ -c $<
#
#$(OBJ_DIR)/instruction.o: $(SRC_DIR)/instruction.c
#	$(CC) -o $@ -c $<
#
#$(OBJ_DIR)/retarget.o: $(SRC_DIR)/retarget.c
#	$(CC) -o $@ -c $<


/usr/lib/x86_64-linux-gnu/libelf.so.1 :

clean:
	rm -vf $(OBJ_DIR)/!(*.md)
	rm -vf $(BIN_DIR)/!(*.md)
