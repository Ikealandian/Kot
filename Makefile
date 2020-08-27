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
INC := -L out/ -lkot

OUT := out/
OBJ := out/obj/

SOURCE := src/
PLATFORM := platform/

PLATFORM_X11 := $(PLATFORM)X11/
PLATFORM_LINUX := $(PLATFORM)Linux/
EXAMPLES = examples/

CXX := g++
CXXFLAGS := -Wall -Wextra -I $(SOURCE) -I $(PLATFORM)
CXXLIBS := -L/usr/X11R6/lib -L/usr/vulkan/lib -lX11 -lvulkan

# src/
KOTSRC := $(wildcard $(SOURCE)*.cpp)
KOTOBJS := $(KOTSRC:$(SOURCE)%.cpp=$(OBJ)%.o)

# platform/x11/
X11SRC := $(wildcard $(PLATFORM_X11)*.cpp)
X11OBJS := $(X11SRC:$(PLATFORM_X11)%.cpp=$(OBJ)%.o)

# platform/linux/
LNXSRC := $(wildcard $(PLATFORM_LINUX)*.cpp)
LNXOBJS := $(LNXSRC:$(PLATFORM_LINUX)%.cpp=$(OBJ)%.o)

# examples/
EXMSRC := $(wildcard $(EXAMPLES)*.cpp)
EXMOBJS := $(EXMSRC:$(EXAMPLES)%.cpp=$(OUT)%.o)

# Setup, Build and Link
all: $(OBJ) $(KOTOBJS) $(X11OBJS) $(LNXOBJS) $(LIB) $(EXMOBJS)

# Make out/obj/ folders
$(OBJ):
	mkdir -p $(OBJ)

# Build src/
$(KOTOBJS): $(OBJ)%.o: $(SOURCE)%.cpp
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -c $< -o $@

# Build platform/x11/
$(X11OBJS): $(OBJ)%.o: $(PLATFORM_X11)%.cpp
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -c $< -o $@

# Build platform/linux/
$(LNXOBJS): $(OBJ)%.o: $(PLATFORM_LINUX)%.cpp
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -c $< -o $@

# Link kotlib.a
$(LIB):
	ar rs $(LIB) $(KOTOBJS) $(X11OBJS) $(LNXOBJS)

# Build examples/
$(EXMOBJS): $(OUT)%.o: $(EXAMPLES)%.cpp
	$(CXX) $(CXXFLAGS) $(CXXLIBS) $< $(INC) -o $@
 
clean:
	$(RM) $(LIB) $(KOTOBJS) $(X11OBJS) $(LNXOBJS) $(EXMOBJS) *.o