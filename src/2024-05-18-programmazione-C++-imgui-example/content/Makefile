CXX    := g++
OUTPUT := sfmlgame

CXX_FLAGS := -std=c++20 -Wno-unsued-result
INCLUDES  := -I./src -I./src/imgui
LDFLAGS   := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL

SRC_FILES := $(wildcard src/*.cpp src/imgui/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

all: $(OUTPUT)

# main executable requirements / command
$(OUTPUT): $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

# how the object files are compiled from cpp files
.cpp.o:
	$(CXX) -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(OBJ_FILES) ./bin/$(OUTPUT)

run: $(OUTPUT)
	./bin/$(OUTPUT)
