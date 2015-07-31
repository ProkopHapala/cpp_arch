
PROG = CHARGEview

CC = g++
CFLAGS  = -w -O3
#CFLAGS = -Wall

#LIBS   = -lGL -lGLU -lglut
LIBS    = -L/usr/lib/nvidia-331 -lGL -lGLU -lglut

SRCS = chargeview.cpp trackball.c

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
