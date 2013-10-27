# Makefile for tests x86-Linux-specific makefile

CC = g++
LDFLAGS = -lportaudio -Wall
SRC = ../src/
OBJECTS = Wavetables.o WavetablesTest.o Mixer.o MixerTest.o MockIAudio.o PolyvoiceTest.o PolyVoice.o

default: TestWavetables TestMixer TestPolyvoice
	./TestWavetables
	./TestMixer
	./TestPolyvoice

# Test for listing MIDI devices
PrintMidiDevices:
    gcc -o printMidiDevices printMidiDevices.c -lportmidi

# Tests for the wavetables
TestWavetables: WavetablesTest.o Wavetables.o
	$(CC) WavetablesTest.o Wavetables.o -o TestWavetables $(LDFLAGS)
	
WavetablesTest.o: WavetablesTest.cpp $(SRC)Wavetables.hpp Assert.hpp
	$(CC) -c WavetablesTest.cpp $(LDFLAGS)

Wavetables.o: $(SRC)Wavetables.cpp $(SRC)Wavetables.hpp
	$(CC) -c $(SRC)Wavetables.cpp $(LDFLAGS)

# Tests for the mixer
TestMixer: MixerTest.o Mixer.o MockIAudio.o
	$(CC) MixerTest.o Mixer.o MockIAudio.o -o TestMixer $(LDFLAGS)

MixerTest.o: MixerTest.cpp $(SRC)Mixer.hpp MockIAudio.hpp Assert.hpp
	$(CC) -c MixerTest.cpp $(LDFLAGS)

Mixer.o: $(SRC)Mixer.cpp $(SRC)Mixer.hpp
	$(CC) -c $(SRC)Mixer.cpp $(LDFLAGS)

MockIAudio.o: MockIAudio.cpp MockIAudio.hpp
	$(CC) -c MockIAudio.cpp $(LDFLAGS)
	
# Tests for the polyvoice
TestPolyvoice: PolyvoiceTest.o PolyVoice.o Wavetables.o
	$(CC) PolyvoiceTest.o PolyVoice.o Wavetables.o -o TestPolyvoice $(LDFLAGS)

PolyvoiceTest.o: PolyvoiceTest.cpp $(SRC)PolyVoice.hpp Assert.hpp
	$(CC) -c PolyvoiceTest.cpp $(LDFLAGS)

PolyVoice.o: $(SRC)PolyVoice.cpp $(SRC)PolyVoice.hpp $(SRC)Wavetables.hpp
	$(CC) -c $(SRC)PolyVoice.cpp $(LDFLAGS)

# Remove generated files
clean:
	rm -rf $(EXE) *.o
	rm *~
	rm TestWavetables
	rm TestMixer
