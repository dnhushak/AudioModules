# x86-Linux-specific makefile
CC = g++
LDFLAGS = -lportmidi -Wall

SRC = ../src/
EXE = Songbox
HEADERS = $(SRC)Songbox.hpp $(SRC)SongboxMidiParser.hpp $(SRC)MidiFileNameReader.hpp

OBJECTS = Songbox.o SongboxMidiParser.o MidiFileNameReader.o

Songbox: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE) $(LDFLAGS)

MidiFileNameReader.o: $(SRC)MidiFileNameReader.cpp $(HEADERS)
	$(CC) -c $(SRC)MidiFileNameReader.cpp $(LDFLAGS)

SongboxMidiParser.o: $(SRC)SongboxMidiParser.cpp $(HEADERS)
	$(CC) -c $(SRC)SongboxMidiParser.cpp $(LDFLAGS)

Songbox.o: $(SRC)Songbox.cpp $(HEADERS)
	$(CC) -c $(SRC)Songbox.cpp $(LDFLAGS)

clean:
	rm -rf $(EXE) *.o
