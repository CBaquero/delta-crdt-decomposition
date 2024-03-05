CC = g++
DEBUG = -g -v
FLAGS = -std=c++20 -Wall -Wextra -Wconversion -Wunused -Wshadow -Werror -pedantic

all: gset 2pset lwwset

gset: test_gset.cpp
	$(CC) $(FLAGS) test_gset.cpp -o test_gset

2pset: test_2pset.cpp
	$(CC) $(FLAGS) test_2pset.cpp -o test_2pset

lwwset: test_lwwset.cpp
	$(CC) $(FLAGS) test_lwwset.cpp -o test_lwwset
	
clean:
	rm test_gset test_2pset test_lwwset
