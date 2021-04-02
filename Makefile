APPNAME  := space_inv
SRCDIR   := src
BUILDDIR := build

CC       := gcc
CXX      := g++
CXXFLAGS := --std=c++11 -Wall -Werror -Wshadow -fsanitize=undefined
LDFLAGS  := -framework OpenGL
LDLIBS   := -lglfw -lglew

RM    := rm -f
MKDIR := mkdir -p

srcfiles := main.cpp
objects  :=

.PHONY: directories

all: directories $(APPNAME)

$(APPNAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) -o $(BUILDDIR)/$(APPNAME) $(SRCDIR)/main.cpp

directories: $(BUILDDIR)

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

clean:
	$(RM) $(BUILDDIR)/$(APPNAME)
