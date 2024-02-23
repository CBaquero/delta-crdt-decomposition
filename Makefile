CC = g++
DEBUG = -g -v
FLAGS = -std=c++17 -Wall -Wextra -Wconversion -Wunused -Wshadow -Werror -pedantic

all: gset 2pset

gset: test_gset.cpp gset.cpp gset.h
	$(CC) $(FLAGS) test_gset.cpp -o test_gset

2pset: test_2pset.cpp 2pset.h
	$(CC) $(FLAGS) test_2pset.cpp -o test_2pset

awset: test_awset.cpp awset.h
	$(CC) $(FLAGS) test_awset.cpp -o test_awset
	
clean:
	rm test_gset test_2pset test_awset
