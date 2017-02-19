# At the time being, use the VS project file on Windows

COMPILER    := clang++
# was g++
ARCH        := x86_64
ARCHES      := i386 x86_64
BUILD_TYPE  := debug
BUILD_TYPES := debug release
OS_TYPES    := win32 win64 linux osx
OS_TYPE     := linux

SOURCE_SUFFIXES := '(' -name '*.cpp' ')'

SRCFILES := $(shell find 'src' $(SOURCE_SUFFIXES))
COMMONSRCFILES := $(shell find 'src/common' $(SOURCE_SUFFIXES))
CLIENTSRCFILES := $(shell find 'src/client' $(SOURCE_SUFFIXES))
SERVERSRCFILES := $(shell find 'src/server' $(SOURCE_SUFFIXES))

OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))
COMMONOBJFILES := $(patsubst %.cpp,%.o,$(COMMONSRCFILES))
CLIENTOBJFILES := $(patsubst %.cpp,%.o,$(CLIENTSRCFILES))
SERVEROBJFILES := $(patsubst %.cpp,%.o,$(SERVERSRCFILES))

CFLAGS := -I include -g -O2 -fno-rtti

all: client server

client: $(COMMONOBJFILES) $(CLIENTOBJFILES)
	$(COMPILER) $+ -o client -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system -lGLU -lGL

server: $(COMMONOBJFILES) $(SERVEROBJFILES)
	$(COMPILER) $+ -o server -lsfml-network -lsfml-system -lGLU -lGL

%.o: %.cpp
	$(COMPILER) $(CFLAGS) -c $< -o $@

clean:
	rm -rf client server $(OBJFILES)

.PHONY: all clean
