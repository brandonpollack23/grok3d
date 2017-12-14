CC = g++
INCLUDEDIRS = grok3d/grok3d/inc grok3d/glm grok3d/notstd grok3d/include
CFLAGS = -std=c++17 -g -O2 -Wall -fno-exceptions $(GLFAGS) $(addprefix -I, $(INCLUDEDIRS))

LD = g++
GL_LD_FLAGS = -lglfw -lGL -ldl
LDFLAGS = $(GL_LD_FLAGS)

TARGET = grok3dtests

SOURCES = $(shell find ./src -name "*.cpp" -o -name "*.c")
OBJECTS = $(patsubst ./src/%, ./$(OBJDIR)/%, $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES))))

VPATHCPP = $(shell find ./src -name "*.cpp" | sed 's/\(.*\)\/.*\..*$$/\1/g' | sort -u)
VPATHOBJ = $(patsubst ./src%, ./$(OBJDIR)%, $(VPATHCPP))
VPATH = $(VPATHCPP) $(VPATHOBJ)

BINDIR = bin
OBJDIR = o

STATICLIBS = grok3d/grok3d/o/libgrok3d.a

all: dependencies $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(LD) -o $(BINDIR)/$@ $^ $(STATICLIBS) $(LDFLAGS) 
$(OBJDIR)/%.o: %.cpp | $(VPATHOBJ)
	$(CC) -c $(CFLAGS) $< -o $@

dependencies:
	cd grok3d/grok3d && make

$(BINDIR):
	mkdir -p $@

$(VPATHOBJ):
	mkdir -p $@

clean:
	rm -rf o bin
	cd grok3d/grok3d && make clean

print-%: 
	@echo $($*)
