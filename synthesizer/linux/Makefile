
# x86-Linux-specific makefile
CC = g++
LDFLAGS = -lportaudio

SRC = ../src/

OBJECTS = AudioProcessor.o 

    
AudioProcessor.o: $(SRC)AudioProcessor.cpp  
	$(CC) -c $(SRC)AudioProcessor.cpp $(LDFLAGS)

