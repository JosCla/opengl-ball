CC = g++
CFLAGS = -g -Wall
INCLUDE = -Iinclude
LIBS = -lGL -lglfw -lassimp -ldl

CPPFILES = main.cpp src/utils.cpp src/collision.cpp src/shapes.cpp src/mesh.cpp src/model.cpp src/shader.cpp src/camera.cpp src/light.cpp src/text.cpp src/thing.cpp include/glad/glad.cpp
OBJFILES = $(CPPFILES:.cpp=.o)
TARGET = main
SOURCE = main.cpp

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $^ -o $@

clean:
	$(RM) $(OBJFILES)
	$(RM) $(TARGET)
