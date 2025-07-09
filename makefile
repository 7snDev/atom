# For Makefile I Used ChatGPT :)

CXX        := g++
CXXFLAGS   := -std=c++20 -Wall -O3

PKG_CFLAGS := $(shell pkg-config --cflags gtk+-3.0 webkit2gtk-4.1)
PKG_LIBS   := $(shell pkg-config --libs   gtk+-3.0 webkit2gtk-4.1)

LDFLAGS    := -ldl -pthread $(PKG_LIBS)

TARGET     := atomic

SRCS       := $(wildcard *.cpp)
OBJS       := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

%.o: %.cpp atom/atom.hpp
	$(CXX) $(CXXFLAGS) $(PKG_CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean
