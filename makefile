CC = g++
CFLAGS = -std=c++11 -Wall
LIBS = -lGL -lGLEW -lglfw -lGLU -lglut
SRC = src/
INC = includes/
OUT = app
OBJS = main.o camera.o

all: $(OUT)

main.o: $(SRC)main.cpp $(INC)colors.hpp $(INC)primitives.hpp $(INC)camera.hpp $(INC)bird.hpp $(INC)object.hpp $(INC)vector.hpp $(INC)constants.hpp
	$(CC) $(CFLAGS) -I$(INC) -c $(SRC)main.cpp -o $@

camera.o: $(SRC)camera.cpp $(INC)camera.hpp $(INC)vector.hpp
	$(CC) $(CFLAGS) -I$(INC) -c $(SRC)camera.cpp -o $@

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT) $(LIBS)

run: all
	./$(OUT)

clean:
	rm -f $(OUT) $(OBJS)