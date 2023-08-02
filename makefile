CC = g++
INCFLAGS = -I/opt/homebrew/Cellar/sdl2/2.28.1/include/ -I/opt/homebrew/Cellar/sdl2_ttf/2.20.2/include/
LIBFLAGS = -L/opt/homebrew/Cellar/sdl2/2.28.1/lib/ -L/opt/homebrew/Cellar/sdl2_ttf/2.20.2/lib -lSDL2_ttf `sdl2-config --cflags` `sdl2-config --libs`
CFLAGS = -std=c++11 -Wc++11-extensions
FILES = src/**.cpp
O_FILES = **.o
OUTPUT = -o bin/main.out

all:
	$(CC) src/main.cpp $(OUTPUT) $(LIBFLAGS) $(INCFLAGS) $(CFLAGS)

run:
	./bin/main.out