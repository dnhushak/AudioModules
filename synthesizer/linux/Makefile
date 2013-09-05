
# x86-Linux-specific makefile
CC = g++
LDFLAGS = -lportaudio -Wall

SRC = ../src/
EXE = Synthesizer
HEADERS = $(SRC)AudioProcessor.hpp $(SRC)Mixer.hpp $(SRC)Module.hpp $(SRC)IAudio.hpp $(SRC)PolyVoice.hpp
SRCS = $(SRC)Synthesizer.cpp $(SRC)AudioProcessor.cpp $(SRC)Mixer.cpp $(SRC)Module.cpp $(SRC)IAudio.cpp $(SRC)PolyVoice.cpp

OBJECTS = Synthesizer.o AudioProcessor.o Mixer.o Module.o PolyVoice.o IAudio.o

Synthesizer: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE) $(LDFLAGS)

Synthesizer.o: $(SRC)Synthesizer.cpp $(HEADERS)
	$(CC) -c $(SRC)Synthesizer.cpp $(LDFLAGS)

AudioProcessor.o: $(SRC)AudioProcessor.cpp $(HEADERS)
	$(CC) -c $(SRC)AudioProcessor.cpp $(LDFLAGS)

Mixer.o: $(SRC)Mixer.cpp $(HEADERS)
	$(CC) -c $(SRC)Mixer.cpp $(LDFLAGS)

Module.o: $(SRC)Module.cpp $(HEADERS)
	$(CC) -c $(SRC)Module.cpp $(LDFLAGS)

PolyVoice.o: $(SRC)PolyVoice.cpp $(HEADERS)
	$(CC) -c $(SRC)PolyVoice.cpp $(LDFLAGS)

IAudio.o: $(SRC)IAudio.cpp $(HEADERS)
	$(CC) -c $(SRC)IAudio.cpp $(LDFLAGS)

clean:
	rm *.o
	
depend:
	gccmakedep -- $(LDFLAGS) -- $(SRCS)

