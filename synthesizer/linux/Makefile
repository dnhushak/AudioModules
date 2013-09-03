
# x86-Linux-specific makefile
CC = g++
LDFLAGS = -lportaudio

SRC = ../src/

OBJECTS = Synthesizer.o AudioProcessor.o Mixer.o Module.o

Synthesizer: $(OBJECTS)
	$(CC) $(OBJECTS) -o Synthesizer $(LDFLAGS)

Synthesizer.o: $(SRC)Synthesizer.cpp $(SRC)AudioProcessor.hpp
	$(CC) -c $(SRC)Synthesizer.cpp $(LDFLAGS)

AudioProcessor.o: $(SRC)AudioProcessor.cpp $(SRC)Mixer.hpp $(SRC)Module.hpp
	$(CC) -c $(SRC)AudioProcessor.cpp $(LDFLAGS)

Mixer.o: $(SRC)Mixer.cpp $(SRC)IAudio.hpp
	$(CC) -c $(SRC)Mixer.cpp $(LDFLAGS)
	
Module.o: $(SRC)Module.hpp
	$(CC) -c $(SRC)Module.cpp $(LDFLAGS)
	
