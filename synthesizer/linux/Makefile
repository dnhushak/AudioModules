
# x86-Linux-specific makefile
CC = g++
LDFLAGS = -lportaudio -lportmidi -Wall

SRC = ../src/
EXE = Synthesizer
HEADERS = $(SRC)AudioProcessor.hpp $(SRC)MIDIParser.hpp $(SRC)Mixer.hpp $(SRC)Module.hpp $(SRC)IAudio.hpp $(SRC)PolyVoice.hpp $(SRC)Wavetables.hpp $(SRC)Voice.hpp $(SRC)VoiceConfigReader.hpp $(SRC)chiputil.hpp $(SRC)MIDIController.hpp
SRCS = $(SRC)Synthesizer.cpp $(SRC)AudioProcessor.cpp $(SRC)MIDIParser.cpp $(SRC)Mixer.cpp $(SRC)Module.cpp $(SRC)IAudio.cpp $(SRC)PolyVoice.cpp $(SRC)Wavetables.cpp $(SRC)Voice.cpp $(SRC)VoiceConfigReader.cpp $(SRC)MIDIController.cpp

OBJECTS = Synthesizer.o AudioProcessor.o MIDIParser.o Mixer.o Module.o PolyVoice.o Wavetables.o IAudio.o Voice.o VoiceConfigReader.o  MIDIController.o

Synthesizer: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE) $(LDFLAGS)

Synthesizer.o: $(SRC)Synthesizer.cpp $(HEADERS)
	$(CC) -c $(SRC)Synthesizer.cpp $(LDFLAGS)

AudioProcessor.o: $(SRC)AudioProcessor.cpp $(HEADERS)
	$(CC) -c $(SRC)AudioProcessor.cpp $(LDFLAGS)
	
MIDIParser.o: $(SRC)MIDIParser.cpp $(HEADERS)
	$(CC) -c $(SRC)MIDIParser.cpp $(LDFLAGS)

Mixer.o: $(SRC)Mixer.cpp $(HEADERS)
	$(CC) -c $(SRC)Mixer.cpp $(LDFLAGS)

Module.o: $(SRC)Module.cpp $(HEADERS)
	$(CC) -c $(SRC)Module.cpp $(LDFLAGS)

PolyVoice.o: $(SRC)PolyVoice.cpp $(HEADERS)
	$(CC) -c $(SRC)PolyVoice.cpp $(LDFLAGS)

Wavetables.o: $(SRC)Wavetables.cpp $(HEADERS)
	$(CC) -c $(SRC)Wavetables.cpp $(LDFLAGS)

IAudio.o: $(SRC)IAudio.cpp $(HEADERS)
	$(CC) -c $(SRC)IAudio.cpp $(LDFLAGS)
	
Voice.o: $(SRC)Voice.cpp $(HEADERS)
	$(CC) -c $(SRC)Voice.cpp $(LDFLAGS)

VoiceConfigReader.o: $(SRC)VoiceConfigReader.cpp $(HEADERS)
	$(CC) -c $(SRC)VoiceConfigReader.cpp $(LDFLAGS)

MIDIController.o: $(SRC)MIDIController.cpp $(HEADERS)
	$(CC) -c $(SRC)MIDIController.cpp $(LDFLAGS)

install:
	cp $(EXE) /usr/bin/$(EXE)

clean:
	rm -rf $(EXE) *.o
	
depend:
	gccmakedep -- $(LDFLAGS) -- $(SRCS)
