# At the time being, use the VS project file on Windows

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

CCFLAGS := -I include

all: $(APPLICATION)

$(APPLICATION):
	g++ $(CCFLAGS) $(SRCFILES) -o $(APPLICATION)

clean:
	@rm -rf $(APPLICATION)
	@find ./src -name '*.o' -delete
	@find ./src -name '*.lib' -delete
	@find ./src -name '*.dll' -delete
	@find ./src -name '*.exe' -delete

%::
	@echo "NOHIT" ${ARCH} '$$@' $@ '$$%' $% '$$<' $< '$$?' $? '$$^' $^ '$$+' $+ '$$|' $| '$$*' $*

.PHONY: all clean
