OBJS     = main.cpp
OBJ_NAME =  program.x
LFLAGS   = -I/usr/local/include/SDL2
#FLAGS = -std=c++11 -Og -g -w
#FLAGS = -std=c++11 -Og -g -Wall
FLAGS = -std=c++11 -O3 -ffast-math
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops -ffast-math
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops  -ffast-math -march=native -mtune=native -mssse3
#FLAGS = -std=c++11 -O3 -ftree-vectorize -unroll-loops --param max-unroll-times=4 -ffast-math -march=native -mtune=native -mssse3

all : $(OBJS)
	g++ $(OBJS) $(LFLAGS) $(FLAGS) -lSDL2 -o $(OBJ_NAME)
