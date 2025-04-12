CXX = g++
CXXFLAGS += -I./lib
# CXXFLAGS += -v
LDFLAGS += -L./lib
NO_WARN_FLAGS = -w

SRC = $(wildcard src/*.cpp)
OBJ_DIR = obj
OBJ = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
OUT = output
EXT =

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S), Darwin)
    LIBS = -lraylib -framework OpenGL
endif
# Windows platform; assumes MinGW
ifeq ($(OS), Windows_NT)
    LIBS = -lraylib -lopengl32 -lgdi32
    EXT = .exe
endif


# Build target
$(OUT)$(EXT): $(OBJ)
	$(CXX) $(OBJ) -o $(OUT)$(EXT) $(CXXFLAGS) $(LIBS)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Ignoring buggy dependancy
$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	if grep -q "raygui.h" $<; then \
		$(CXX) -c $< -o $@ $(CXXFLAGS) $(NO_WARN_FLAGS); \
	else \
		$(CXX) -c $< -o $@ $(CXXFLAGS); \
	fi

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(OUT)$(EXT)

