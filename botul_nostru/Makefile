CC=g++
CFLAGS=-O3 -funroll-loops -c -Wall -std=c++0x
LDFLAGS=-lm -std=c++0x

SOURCES=$(wildcard *.cpp)
OBJECTS=$(addprefix bin/, $(notdir $(SOURCES:.cpp=.o)))

EXECUTABLE=bin/MyBot

#Comment/Uncomment the following to enable debugging
ifdef LOGFILENAME
CFLAGS+=-g -DDEBUG -DLOGFILENAME=\"$(LOGFILENAME)\"
else
CFLAGS+=-g -DDEBUG
endif

all: clean $(OBJECTS) $(MAINOBJECTS) $(EXECUTABLE) $(ALLEXECUTABLES)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -g -o $@

bin/%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c -g -o $@ $^

clean: 
	-rm -f bin/*.o $(EXECUTABLE) *.d debug.txt

.PHONY: all clean

