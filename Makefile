# At the time being, use the VS project file on Windows

COMPILER    := g++
APPLICATION := minecube
ARCH        := x86_64
ARCHES      := i386 x86_64
BUILD_TYPE  := debug
BUILD_TYPES := debug release
OS_TYPES    := win32 win64 linux osx
OS_TYPE     := linux


SOURCE_SUFFIXES := '(' -name '*.cpp' ')'
SRCFILES := $(shell find 'src' ${SOURCE_SUFFIXES})
OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))

CFLAGS := -I include -g

all: $(APPLICATION)

$(APPLICATION): $(OBJFILES)
	$(COMPILER) $+ -o $(APPLICATION) -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU

%.o: %.cpp
	${COMPILER} ${CFLAGS} -MMD -MP -MT "$*.d $*.o"  -c $< -o $@ -I/usr/include/SDL

clean:
	@rm -rf $(APPLICATION)
	@find ./src -name '*.o' -delete
	@find ./src -name '*.d' -delete
	@find ./src -name '*.lib' -delete
	@find ./src -name '*.dll' -delete
	@find ./src -name '*.exe' -delete

%::
	@echo "NOHIT" ${ARCH} '$$@' $@ '$$%' $% '$$<' $< '$$?' $? '$$^' $^ '$$+' $+ '$$|' $| '$$*' $*

.PHONY: all clean
