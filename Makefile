APPNAME   := space_inv

SRCDIR    := src
BUILDDIR  := build
TARGETDIR := bin
SRCEXT    := cpp
HDREXT    := h

TARGET    := $(TARGETDIR)/$(APPNAME)

CXX       := g++
CC        := gcc
CXXFLAGS  := --std=c++11 -Wall -Wextra -Wshadow -fsanitize=undefined -Werror# -pedantic -O3 -flto
LDFLAGS   := -framework OpenGL
LDLIBS    := -lglfw -lglew

RM        := rm -f
MKDIR     := mkdir -p

SRCFILES  := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCFILES:.$(SRCEXT)=.o))

.PHONY: directories

all: directories $(APPNAME)

$(APPNAME): $(OBJECTS)
	@echo " Linking binary: $@..."
	$(CXX) $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) $^ -o $(TARGETDIR)/$(APPNAME)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(SRCDIR)/global.h $(SRCDIR)/%.$(HDREXT)
	@echo " Compiling object: $@.."
	$(CXX) $(CXXFLAGS) -c -o $@ $<

directories: $(BUILDDIR) $(TARGETDIR)

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

$(TARGETDIR):
	$(MKDIR) $(TARGETDIR)

clean:
	$(RM) $(BUILDDIR)/* $(TARGETDIR)/$(APPNAME)

