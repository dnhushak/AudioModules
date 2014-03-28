#Core Audio Devices makefile
CXX = g++
LDFLAGS = -Wall
PAFLAGS = -lportaudio
PMFLAGS = -lportmidi
CFLAGS = -g

#File Extensions
HEADER = .hpp
SOURCE = .cpp
BINARY = .o

#Directories
SRCEDIR = ../src/
AUDIDIR = $(SRCEDIR)coreAudioDevices/
MIDIDIR = $(SRCEDIR)coreMidiDevices/
PORTDIR = $(SRCEDIR)PortMediaDevices/
SNTHDIR = $(SRCEDIR)synthesisTools/
COREDIR = $(SRCEDIR)coreDevices/
DIRLIST = $(SRCEDIR) $(AUDIDIR) $(MIDIDIR) $(PORTDIR) $(SNTHDIR) $(COREDIR)

#Final Executable Name
EXE = Chipophone

# Generate list of source headers with extensions
HEADERS = $(foreach DIR, $(DIRLIST), $(wildcard $(DIR)*$(HEADER)))
SOURCES = $(foreach DIR, $(DIRLIST), $(wildcard $(DIR)*$(SOURCE)))
OBJECTS = $(addsuffix $(BINARY), $(notdir $(basename $(SOURCES))))
INCLUDE = $(foreach DIR, $(DIRLIST),-I$(DIR))

$(EXE): $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) -o $(EXE) $(LDFLAGS) $(PAFLAGS) $(PMFLAGS)

%$(BINARY) : $(SRCEDIR)%$(SOURCE)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS) -o $@
	
%$(BINARY) : $(AUDIDIR)%$(SOURCE)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS) -o $@
	
%$(BINARY) : $(MIDIDIR)%$(SOURCE)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS) -o $@

%$(BINARY) : $(PORTDIR)%$(SOURCE)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS) -o $@

%$(BINARY) : $(SNTHDIR)%$(SOURCE)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS) -o $@

%$(BINARY) : $(COREDIR)%$(SOURCE)
	$(CXX) $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS) -o $@


clean:
	rm -rf $(EXE) *.o
	
depend:
	gccmakedep -- $(LDFLAGS3) -- $(SOURCES) $(HEADERS)