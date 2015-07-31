
#FLAGS = -std=c++11 -w
#FLAGS = -std=c++11 -Og -g -Wall -ggdb
#FLAGS = -O0 -Wall -ggdb
FLAGS = -std=c++11 -Og -Wall -ggdb
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops -ffast-math
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops --param max-unroll-times=4 -ffast-math -march=native -mtune=native -mssse3

all: clean program

program: 
	g++ $(FLAGS) -o program SDLapp.cpp  -lm -lSDL2 -lGL -lGLU

clean:
	rm -rf *.o program  

