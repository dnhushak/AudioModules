# Makefile for tests x86-Linux-specific makefile
CC = g++
LDFLAGS = -lportaudio -Wall

SRC = ../src/
EXE = Tests
HEADERS = $(SRC)AudioProcessor.hpp $(SRC)Mixer.hpp $(SRC)Module.hpp $(SRC)IAudio.hpp $(SRC)PolyVoice.hpp $(SRC)Wavetables.hpp
SRCS = $(SRC)Synthesizer.cpp $(SRC)AudioProcessor.cpp $(SRC)Mixer.cpp $(SRC)Module.cpp $(SRC)IAudio.cpp $(SRC)PolyVoice.cpp $(SRC)Wavetables.cpp

OBJECTS = Wavetables.o WavetablesTest.o 

Tests: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE) $(LDFLAGS)
	
WavetablesTest.o: WavetablesTest.cpp $(SRC)Wavetables.hpp Assert.hpp
	$(CC) -c WavetablesTest.cpp $(LDFLAGS)
	
Wavetables.o: $(SRC)Wavetables.cpp $(SRC)Wavetables.hpp
	$(CC) -c $(SRC)Wavetables.cpp $(LDFLAGS)

clean:
	rm -rf $(EXE) *.o
	
depend:
	gccmakedep -- $(LDFLAGS) -- $(SRCS)

