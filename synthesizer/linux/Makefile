
# x86-Linux-specific makefile
CC = g++
LDFLAGS = -lportaudio

SRC = ../src/

OBJECTS = Synthesizer.o AudioProcessor.o 

Synthesizer: $(OBJECTS)
	$(CC) $(OBJECTS) -o Synthesizer

Synthesizer.o: $(SRC)Synthesizer.cpp $(SRC)AudioProcessor.o
	$(CC) -c $(SRC)Synthesizer.cpp $(LDFLAGS)

AudioProcessor.o: $(SRC)AudioProcessor.cpp  
	$(CC) -c $(SRC)AudioProcessor.cpp $(LDFLAGS)

