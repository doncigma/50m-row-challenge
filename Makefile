# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall

# Targets
all: main

# Default target (builds without extra flags)
main: main.c
	$(CC) $(CFLAGS) -o main main.c

# Compile with optimization level O2
o2: main.c
	$(CC) $(CFLAGS) -O2 -o main main.c

o3: main.c
	$(CC) $(CFLAGS) -O3 -o main main.c

time:
	time ./main

mem:
	valgrind --leak-check=full ./main

# Clean up generated files
clean:
	rm -f main
