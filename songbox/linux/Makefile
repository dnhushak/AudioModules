
# x86-Linux-specific makefile
CC = g++
LDFLAGS = -lportmidi -Wall

SRC = ../src/
EXE = Songbox
HEADERS = $(SRC)Songbox.hpp $(SRC)MIDIParser.hpp $(SRC)MidiFileNameReader.hpp
SRCS = $(SRC)Songbox.cpp $(SRC)MIDIParser.cpp $(SRC)MidiFileNameReader.cpp

OBJECTS = Songbox.o MIDIParser.o MidiFileNameReader.o

Songbox: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE) $(LDFLAGS)

Songbox.o: $(SRC)Songbox.cpp $(HEADERS)
	$(CC) -c $(SRC)Songbox.cpp $(LDFLAGS)

MIDIParser.o: $(SRC)MIDIParser.cpp $(HEADERS)
	$(CC) -c $(SRC)MIDIParser.cpp $(LDFLAGS)

MidiFileNameReader.o: $(SRC)MidiFileNameReader.cpp $(HEADERS)
	$(CC) -c $(SRC)MidiFileNameReader.cpp $(LDFLAGS)

clean:
	rm -rf $(EXE) *.o
	
depend:
	gccmakedep -- $(LDFLAGS) -- $(SRCS)

