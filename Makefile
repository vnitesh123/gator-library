
# Compiler version
CXX := g++
CXXFLAGS := -std=c++14

# Source files
SRCS := $(wildcard *.cpp)

# Object files
OBJS := $(SRCS:.cpp=.o)

INC_DIRS := -I.

# executable
gatorLibrary: $(OBJS)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

depend: .depend

.depend: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $^ > $@

clean:
	rm -f gatorLibrary $(OBJS) .depend

.PHONY: all depend clean

all: gatorLibrary

-include .depend
