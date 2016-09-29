CC = gcc
CFLAGS = -O0 -std=gnu99 -Wall -fopenmp -mavx
EXECUTABLE = \
	benchmark_clock_gettime

default: contral.o
	$(CC) $(CFLAGS) contral.o time_test.c -o benchmark_clock_gettime

.PHONY: clean default

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 


gencsv: default
	for i in `seq 100 5000 25000`; do \
		printf "%d," $$i;\
		./benchmark_clock_gettime $$i; \
	done > result_clock_gettime.csv	

plot: result_clock_gettime.csv
	gnuplot clocktime.gp


clean:
	rm -f $(EXECUTABLE) *.o *.s result_clock_gettime.csv
