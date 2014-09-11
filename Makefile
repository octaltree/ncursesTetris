CXX = g++
FLAGS = -lncurses -std=c++11
tetris: a.cpp frame.cpp frame.h mino.cpp mino.h
	@${CXX} ${FLAGS} a.cpp frame.cpp mino.cpp -o tetris
