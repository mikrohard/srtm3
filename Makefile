all: srtm3

srtm3.o: srtm3.c
	gcc -c srtm3.c

srtm3: srtm3.o
	gcc -o srtm3 srtm3.o -lm

clean:
	rm *.o
	rm srtm3
