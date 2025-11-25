CC = g++
CFLAGS = -std=c++11 -Wall
LIBS = -lGL -lGLEW -lglfw -lGLU -lglut
SRC = src/
INC = includes/
SRC_UTIL = utils/
OUT = app
OBJS = main.o camera.o bird.o boid.o primitives.o

all: $(OUT)

main.o: $(SRC)main.cpp $(INC)colors.hpp $(INC)primitives.hpp $(INC)camera.hpp $(INC)object.hpp $(INC)vector.hpp $(INC)constants.hpp $(INC)bird.hpp $(INC)boid.hpp $(SRC_UTIL)calculation-functions.cpp
	$(CC) $(CFLAGS) -I$(INC) -c $(SRC)main.cpp -o $@

camera.o: $(SRC)camera.cpp $(INC)camera.hpp $(INC)vector.hpp
	$(CC) $(CFLAGS) -I$(INC) -c $(SRC)camera.cpp -o $@

bird.o: $(SRC)bird.cpp $(INC)bird.hpp $(INC)vector.hpp  $(INC)primitives.hpp $(SRC_UTIL)calculation-functions.cpp
	$(CC) $(CFLAGS) -I$(INC) -c $(SRC)bird.cpp -o $@

primitives.o: $(SRC)primitives.cpp $(INC)primitives.hpp $(INC)vector.hpp  $(INC)colors.hpp
	$(CC) $(CFLAGS) -I$(INC) -c $(SRC)primitives.cpp -o $@

boid.o: $(SRC)boid.cpp $(INC)boid.hpp $(INC)bird.hpp $(INC)object.hpp $(INC)constants.hpp $(INC)bird.hpp  $(INC)primitives.hpp $(INC)colors.hpp $(SRC_UTIL)calculation-functions.cpp
	$(CC) $(CFLAGS) -I$(INC) -c $(SRC)boid.cpp -o $@

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT) $(LIBS)

run: all
	./$(OUT)

clean:
	rm -f $(OUT) $(OBJS)

#include "bird.hpp"
#include "constants.hpp"
#include "object.hpp"
#include "primitives.hpp"