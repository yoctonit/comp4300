# to compile and run in one command type:
# make run

# define which compiler to use
CXX    := g++
OUTPUT := sfmlgame
OS     := $(shell uname)

# linux compiler / linker flags
ifeq ($(OS), Linux)
    CXX_FLAGS := -O3 -std=c++20 -Wno-unused-result -Wno-deprecated-declarations
    INCLUDES  := -I./src -I ./src/imgui
    LDFLAGS   := -03 -sfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL
endif

# mac osx compiler / linker flags
ifeq ($(OS), Darwin)
    SFML_DIR  := /opt/homebrew/Cellar/sfml/2.6.1
    CXX_FLAGS := -O3 -std=c++20 -Wno-unused-result -Wno-deprecated-declarations
    INCLUDES  := -I./src -I ./src/imgui -I$(SFML_DIR)/include
    LDFLAGS   := -03 -sfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L$(SFML_DIR)/lib -framework OpenGL
endif

# the source files for the ecs game engine
SRC_FILES := $(wildcard src/*.cpp src/imgui/*cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

# include dependency files
DEP_FILES := $(SRC_FILES:.cpp=.d)
-include $(DEP_FILES)

# all of these targets will be made if you just type make
all: $(OUTPUT)

# define the main executable requirements / command
$(OUTPUT): $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

# specifies how the object files are compiled from cpp files
.cpp.o:
	$(CXX) -MMD -MP -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

# typing 'make clean' will remove all intermediate build files
clean:
	rm -f $(OBJ_FILES) $(DEP_FILES) ./bin/$(OUTPUT)

# typing 'make run' will compile and run the program
run: $(OUTPUT)
	cd bin && ./$(OUTPUT) && cd ..
