CC = g++
INCLUDEDIRS = grok3d/Grok3d_Windows/inc grok3d/glm grok3d/notstd
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

all: $(TARGET)

$(TARGET): dependencies $(OBJECTS) | $(BINDIR)
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

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
	cd grok3d/Grok3d_Windows && make clean

print-%: 
	@echo $($*)
