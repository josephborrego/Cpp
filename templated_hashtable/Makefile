# Assign compiler:
CC=g++

# Compiler options:
CFLAGS=-g -Wall -Werror -std=c++0x -c

# Loader Flags:
LDFLAGS=-lssl -lcrypto

# Source files (.c or .cpp)
SRCS=Hash.cpp useHash.cpp

# Object files (Derived from SRCS)
OBJS=$(SRCS:.c=.o)

# Header files (.h)
HEADERS=Hash.h Node.h

# Executable
EXE=runprog

# Tar Archive
TAR=struct.tgz

#all: struct
all: $(EXE)

#struct: Item.o struct.o
$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -std=c++0x -o $@


%.o: %.c Item.h
	$(CC) $(CFLAGS) $< -o $@
#	gcc -g.......-c %.c -o %.o

#struct.o: struct.c Item.h
#	$(CC) $(CFLAGS) struct.c -o struct.o
#
#Item.o: Item.c Item.h
#	$(CC) $(CFLAGS) Item.c -o Item.o
#Item.o: Item.c Item.h
#	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean tar
clean:
	rm -f $(OBJS) $(EXE) $(TAR)

tar:
	tar -czvf $(TAR) $(SRCS) $(HEADERS)

#depend: .depend
#.depend: $(SRCS)
#	rm -f ./.depend
#	$(CC) $(CFLAGS) -MM $^ -MF ./.depend;
#
#include .depend

