#File Extensions
HEADER = .hpp
SOURCE = .cpp
BINARY = .o

#Directories
SRCEDIR = ../src/
AUDIDIR = $(SRCEDIR)coreAudioDevices/
MIDIDIR = $(SRCEDIR)coreMidiDevices/
PORTDIR = $(SRCEDIR)PortMediaDevices/
DIRLIST = $(SRCEDIREDIR) $(AUDIDIR) $(MIDIDIR) $(PORTDIR)

#Final Executable Name
EXE = Synthesizer

#List of Headers
HEADERS = $(foreach DIRLIST, $(dirs), $(wildcard $(dir)/*.$(HEADER)))
SOURCES = $(foreach DIRLIST, $(dirs), $(wildcard $(dir)/*.$(SOURCE)))

OBJECTS = $(wildcard *$(BINARY))

#Compiler type and flags
CXX = g++
LDFLAGS1 = -lportaudio -Wall -g
LDFLAGS2 = -lportmidi -Wall -g
LDFLAGS3 = -Wall -g
LDFLAGS4 = -lportmidi -lportaudio -Wall -g
CFLAGS = -I$(SRCEDIR) -I$(AUDIDIR) -I$(MIDIDIR) -I$(PORTDIR)

Synthesizer: $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) -o $(EXE) $(LDFLAGS4)

clean:
	rm -rf $(EXE) *.o
	
depend:
	gccmakedep -- $(LDFLAGS3) -- $(SOURCES)
