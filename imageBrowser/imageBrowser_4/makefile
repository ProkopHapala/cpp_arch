
#FLAGS = -std=c++11 -w
FLAGS = -std=c++11 -Og -g -Wall
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops -ffast-math
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops --param max-unroll-times=4 -ffast-math -march=native -mtune=native -mssse3
IFLAGS = -I/usr/include/SDL2 -I/usr/local/include/SDL2 
LFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf

all: clean program

program: 
	g++ $(FLAGS) $(IFLAGS) -o program main.cpp  $(LFLAGS)

clean:
	rm -rf *.o program  

