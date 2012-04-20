CC=g++
CFLAGS=-O3 -funroll-loops -c -Wall -std=c++0x
LDFLAGS=-O2 -lm -std=c++0x

SOURCES=$(wildcard *.cpp)
OBJECTS=$(addprefix bin/, $(notdir $(SOURCES:.cpp=.o)))

EXECUTABLE=bin/MyBot

#Comment/Uncomment the following to enable debugging
ifdef LOGFILENAME
CFLAGS+=-g -DDEBUG -DLOGFILENAME=\"$(LOGFILENAME)\"
else
CFLAGS+=-g -DDEBUG
endif

all: clean bin $(OBJECTS) $(MAINOBJECTS) $(EXECUTABLE) $(ALLEXECUTABLES)

bin:
	[ -d bin ] || mkdir bin

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

bin/%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	-rm -f bin/*.o $(EXECUTABLE) *.d debug.txt MylaEtapa1.zip && rm -rf bin/

pack:
	zip MylaEtapa1.zip *.h *.cpp README Makefile

.PHONY: all clean pack

