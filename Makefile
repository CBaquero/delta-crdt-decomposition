CC = g++
DEBUG = -g -v
FLAGS = -std=c++17

all: gset

gset: test_gset.cpp gset.cpp gset.h
	$(CC) $(FLAGS) test_gset.cpp -o test_gset

clean:
	rm test_gset
