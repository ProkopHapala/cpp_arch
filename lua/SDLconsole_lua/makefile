
#FLAGS = -std=c++11 -w
FLAGS = -std=c++11 -Og -g -Wall
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops -ffast-math
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops --param max-unroll-times=4 -ffast-math -march=native -mtune=native -mssse3
LFLAGS = -I/usr/include/lua5.2 -L/usr/include/lua5.2 -llua5.2 -ldl  -lm -lSDL2_ttf -lSDL2_image -lSDL2


all: clean program

program: 
	g++ $(FLAGS) -o program main.cpp $(LFLAGS) 

clean:
	rm -rf *.o program  

