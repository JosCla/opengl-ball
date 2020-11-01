CXX = g++
CFLAGS = -g -Wall

INCLUDE = -Iinclude
LIBS = -lGL -lglfw -lassimp -ldl -lstb

CPPFILES = main.cpp src/utils.cpp src/collision.cpp src/shapes.cpp src/mesh.cpp src/model.cpp src/shader.cpp src/camera.cpp src/light.cpp src/text.cpp src/thing.cpp src/octree.cpp include/glad/glad.cpp
OBJFILES = $(CPPFILES:.cpp=.o)

TARGET = main
SOURCE = main.cpp

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(CFLAGS) $(LIBS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) -c $^ -o $@

clean:
	$(RM) $(OBJFILES)
	$(RM) $(TARGET)
