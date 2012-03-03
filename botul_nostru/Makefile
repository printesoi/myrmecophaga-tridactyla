CC=g++
CFLAGS=-O3 -funroll-loops -c -Wall -std=c++0x
LDFLAGS=-lm -std=c++0x

SOURCES=$(wildcard *.cpp)
OBJECTS=$(addprefix bin/, $(notdir $(SOURCES:.cpp=.o)))

EXECUTABLE=bin/Myla

#Comment/Uncomment the following to enable debugging
ifdef LOGFILENAME
CFLAGS+=-g -DDEBUG -DLOGFILENAME=\"$(LOGFILENAME)\"
else
CFLAGS+=-g -DDEBUG
endif

all: bin clean $(OBJECTS) $(MAINOBJECTS) $(EXECUTABLE) $(ALLEXECUTABLES)

bin:
	[ -d bin ] || mkdir bin

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

bin/%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	-rm -f bin/*.o $(EXECUTABLE) *.d debug.txt Myla.zip

pack:
	zip Myla.zip *.h *.cpp README Makefile

.PHONY: all clean pack

