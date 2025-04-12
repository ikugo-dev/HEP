# WARNING: if you are on windows, you need to have g++ installed with MinGW

CXX = gcc
# CXXFLAGS += -I./include
# CXXFLAGS += -v
LDFLAGS += -L./lib -nostdlib

SRC_FILES := $(wildcard src/*.c)
OUT_FILES := $(patsubst src/%.c, bin/%, $(SRC_FILES))

EXT =
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    LIBS = -L./lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S), Darwin)
    LIBS = -L./lib -lraylib -framework OpenGL
endif
ifeq ($(OS), Windows_NT)
    LIBS = -L./lib -lraylib -lopengl32 -lgdi32
    EXT = .exe
endif

bin/%: src/%.c
	@mkdir -p bin
	$(CXX) $< -o $@$(EXT) $(CXXFLAGS) $(LIBS)

all: $(OUT_FILES)

clean:
	rm -rf bin/
