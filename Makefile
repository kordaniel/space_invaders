OSTYPE   :=
ifneq ($(OS), Windows_NT)
	OSTYPE := $(shell uname -s)
endif

APPNAME     := space_inv
APPNAMEDBG  := space_inv-dbg

SRCDIR      := src
BUILDDIR    := build/rel
BUILDDIRDBG := build/dbg
TARGETDIR   := bin
SRCEXT      := cpp
HDREXT      := hpp

TARGET      := $(TARGETDIR)/$(APPNAME)
TARGETDBG   := $(TARGETDIR)/$(APPNAMEDBG)


CXX         := g++
CXXFLAGS    := --std=c++17 -Wall -Wextra -Wshadow -pedantic#-Wsign-conversion

# Check if the compiler is clang (macos symlinks gcc to clang) and set clang-only flags.
ifneq (, $(findstring clang, $(shell $(CXX) --version)))
	CXXFLAGS += -Wshadow-field-in-constructor -Wsign-compare
endif

CXXFLAGS  += $(shell pkg-config --cflags glew glfw3)
LDFLAGS   := $(shell pkg-config --libs glew glfw3)

ifeq ($(OSTYPE),Darwin)
	LDFLAGS += -framework OpenGL
else
	CXXFLAGS += $(shell pkg-config --cflags opengl)
	LDFLAGS += $(shell pkg-config --libs opengl)
endif

# Release flags
RELCXXFLAGS := -Werror -O3 -flto -DNDEBUG

# Debug flags
DBGCXXFLAGS := -fsanitize=undefined -g -O0
DBGLDFLAGS  := -rdynamic

# Show commands to run and use verbose output
#CXXFLAGS    += -v
#LDFLAGS     += -v

RM          := rm -f
MKDIR       := mkdir -p

SRCFILES    := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCFILES:.$(SRCEXT)=.o))
OBJECTSDBG  := $(patsubst $(SRCDIR)/%,$(BUILDDIRDBG)/%,$(SRCFILES:.$(SRCEXT)=.o))

.PHONY: all debug release clean cleandebug cleanrelease

all: debug

debug: $(TARGETDIR) $(BUILDDIRDBG) $(TARGETDBG)

$(TARGETDBG): $(OBJECTSDBG)
	@echo " Linking binary: $@..."
	$(CXX) $(CXXFLAGS) $(DBGCXXFLAGS) $(LDFLAGS) $(DBGLDFLAGS) $^ -o $(TARGETDIR)/$(APPNAMEDBG)

$(BUILDDIRDBG)/%.o: $(SRCDIR)/%.$(SRCEXT) $(SRCDIR)/Global.hpp $(SRCDIR)/%.$(HDREXT)
	@echo " Compiling object: $@.."
	$(CXX) $(CXXFLAGS) $(DBGCXXFLAGS) -c -o $@ $<

release: $(TARGETDIR) $(BUILDDIR) $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " Linking binary: $@..."
	$(CXX) $(CXXFLAGS) $(RELCXXFLAGS) $(LDFLAGS) $^ -o $(TARGETDIR)/$(APPNAME)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(SRCDIR)/Global.hpp $(SRCDIR)/%.$(HDREXT)
	@echo " Compiling object: $@.."
	$(CXX) $(CXXFLAGS) $(RELCXXFLAGS) -c -o $@ $<

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

$(BUILDDIRDBG):
	$(MKDIR) $(BUILDDIRDBG)

$(TARGETDIR):
	$(MKDIR) $(TARGETDIR)

clean:
	$(RM) $(BUILDDIRDBG)/* $(BUILDDIR)/* $(TARGETDIR)/$(APPNAME) $(TARGETDIR)/$(APPNAMEDBG)

cleandebug:
	$(RM) $(BUILDDIRDBG)/* $(TARGETDIR)/$(APPNAMEDBG)

cleanrelease:
	$(RM) $(BUILDDIR)/* $(TARGETDIR)/$(APPNAME)
