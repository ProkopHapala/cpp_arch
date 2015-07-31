
#FLAGS = -std=c++11 -w
FLAGS = -std=c++11 -Og -g -Wall -Wno-unused-result
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops -ffast-math
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops --param max-unroll-times=4 -ffast-math -march=native -mtune=native -mssse3
IFLAGS = -I/usr/include/SDL2 -I/usr/include/GL
LFLAGS = -lSDL2 -lSDL2_image -lGL -lGLU -lGLEW

all: clean program

program: 
	g++ $(FLAGS) $(IFLAGS) -o program main.cpp  $(LFLAGS)

clean:
	rm -rf *.o program  

