
#********************************************************************
# standard identifiers
CC = g++
STD = -std=c++11
#********************************************************************

#********************************************************************
# build main 
all: main.cpp team1lib.so
	$(CC) $(STD) main.cpp -L. team1lib.so -o main.out
# run executable
run:
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. && ./main.out
#********************************************************************
