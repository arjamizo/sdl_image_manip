CFLAGS = -Wall -O -g
LIBS = -I./SDL -L./lSDL -lSDL -lSDL_image
EXE = main

all: $(EXE)

main: main.cpp
	gcc -o $@ $@.cpp $(CFLAGS)

clean:
	rm -f *.o $(EXE)
