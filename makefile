CC = g++
CFLAGS = -Wall -c

test : models.o comm.o test.o
	$(CC) -Wall models.o test.o -o test

test.o:
	$(CC) $(CFLAGS) test.cpp

models.o :
	$(CC) $(CFLAGS) models.cpp

comm.o :
	$(CC) $(CFLAGS) comm.cpp
