OSTYPE   :=
ifneq ($(OS), Windows_NT)
	OSTYPE := $(shell uname -s)
endif

APPNAME   := space_inv

SRCDIR    := src
BUILDDIR  := build
TARGETDIR := bin
SRCEXT    := cpp
HDREXT    := h

TARGET    := $(TARGETDIR)/$(APPNAME)

CXX       := g++
CC        := gcc
CXXFLAGS  := --std=c++11 -Wall -Wextra -Wshadow -fsanitize=undefined
CXXFLAGS  += -Werror -pedantic
#CXXFLAGS  += -O3 -flto

# Check if the compiler is clang (macos symlinks gcc to clang) and set clang-only flags.
# gcc default behaviour checks for constructor shadowing of member variables..
ifneq (, $(findstring clang, $(shell $(CXX) --version)))
	CXXFLAGS += -Wshadow-field-in-constructor
endif

CXXFLAGS  += $(shell pkg-config --cflags glew glfw3)
LDFLAGS   := $(shell pkg-config --libs glew glfw3)

ifeq ($(OSTYPE),Darwin)
	LDFLAGS += -framework OpenGL
else
	CXXFLAGS += $(shell pkg-config --cflags opengl)
	LDFLAGS += $(shell pkg-config --libs opengl)
endif

# Debug flags
CXXFLAGS  += -g
LDFLAGS   += -rdynamic

RM        := rm -f
MKDIR     := mkdir -p

SRCFILES  := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCFILES:.$(SRCEXT)=.o))

.PHONY: directories

all: directories $(APPNAME)

$(APPNAME): $(OBJECTS)
	@echo " Linking binary: $@..."
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $(TARGETDIR)/$(APPNAME) -v

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

