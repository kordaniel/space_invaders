APPNAME   := space_inv

SRCDIR    := src
BUILDDIR  := build
TARGETDIR := bin
SRCEXT    := cpp

TARGET    := $(TARGETDIR)/$(APPNAME)

CXX       := g++
CC        := gcc
CXXFLAGS  := --std=c++11 -Wall -Wshadow -fsanitize=undefined
LDFLAGS   := -framework OpenGL
LDLIBS    := -lglfw -lglew

RM        := rm -f
MKDIR     := mkdir -p

SRCFILES  := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCFILES:.$(SRCEXT)=.o))

.PHONY: directories

all: directories $(OBJECTS) $(APPNAME)

$(APPNAME): $(OBJECTS)
	@echo " Linking binary: $@..."
	$(CXX) $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) $^ -o $(TARGETDIR)/$(APPNAME)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Compiling object: $@.."
	$(CXX) $(CXXFLAGS) -c -o $@ $<

directories: $(BUILDDIR) $(TARGETDIR)

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

$(TARGETDIR):
	$(MKDIR) $(TARGETDIR)

clean:
	$(RM) $(BUILDDIR)/* $(TARGETDIR)/$(APPNAME)

