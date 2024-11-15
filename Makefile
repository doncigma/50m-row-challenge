all: o2

main: main.c
	gcc -Wall -o process_measurements main.c

o2: main.c
	gcc -Wall -O2 -o process_measurements main.c

o3: main.c
	gcc -Wall -O3 -o process_measurements main.c

time:
	time ./process_measurements

mem:
	valgrind --leak-check=full ./process_measurements

clean:
	rm -f process_measurements
