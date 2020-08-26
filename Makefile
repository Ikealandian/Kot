# Building

# Build Linux
#	Source
#	X11
#	Linux

# Build Mach
#	Source
#	X11
#	Mach (Not Developed)

# Build Windows
#	Source
#	Windows

LIB := out/libkot.a

OUT := out/obj/
SOURCE := src/
PLATFORM := platform/

PLATFORM_X11 := $(PLATFORM)X11/
PLATFORM_LINUX := $(PLATFORM)Linux/

CXX := g++
CXXFLAGS := -Wall -Wextra -I $(SOURCE) -I $(PLATFORM)
CXXLIBS := -L/usr/X11R6/lib -L/usr/vulkan/lib -lX11 -lvulkan

# src/
KOTSRC := $(wildcard $(SOURCE)*.cpp)
KOTOBJS := $(KOTSRC:$(SOURCE)%.cpp=$(OUT)%.o)

# platform/x11/
X11SRC := $(wildcard $(PLATFORM_X11)*.cpp)
X11OBJS := $(X11SRC:$(PLATFORM_X11)%.cpp=$(OUT)%.o)

# platform/linux/
LNXSRC := $(wildcard $(PLATFORM_LINUX)*.cpp)
LNXOBJS := $(LNXSRC:$(PLATFORM_LINUX)%.cpp=$(OUT)%.o)

# Setup, Build and Link
all: setup_kot $(KOTOBJS) $(X11OBJS) $(LNXOBJS) link_kot

setup_kot:
	mkdir -p out/obj/

# Build src/
$(KOTOBJS): $(OUT)%.o: $(SOURCE)%.cpp
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -c $< -o $@

# Build platform/x11/
$(X11OBJS): $(OUT)%.o: $(PLATFORM_X11)%.cpp
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -c $< -o $@

# Build platform/linux/
$(LNXOBJS): $(OUT)%.o: $(PLATFORM_LINUX)%.cpp
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -c $< -o $@

link_kot: 
	ar rs $(LIB) $(KOTOBJS) $(X11OBJS) $(LNXOBJS)

clean:
	$(RM) $(LIB) $(KOTOBJS) $(X11OBJS) $(LNXOBJS) *.o