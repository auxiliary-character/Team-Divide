EXE := team-divide
SRC  := $(shell find src/ -type f -name '*.cpp')
OBJ  := $(SRC:.cpp=.o)
DEP  := $(OBJ:.o=.d)
CXX  := g++
INST := install
BIN  := /usr/bin/

CPPFLAGS  := -MMD -MP
CXXFLAGS  := -std=c++11 -Wall -W -pedantic -O3 -pthread -g
LDLIBS    := -lz -ljsoncpp -lcurl -O3 -pthread -g
INSTFLAGS := -m 0755

.PHONY: all clean

.PHONY: all install

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $^ $(LDLIBS) -o $(EXE)

clean:
	$(RM) $(EXE) $(OBJ) $(DEP)

install:
	$(INST) $(INSTFLAGS) $(EXE) $(BIN)

ifeq "$(MAKECMDGOALS)" ""
-include $(DEP)
endif
