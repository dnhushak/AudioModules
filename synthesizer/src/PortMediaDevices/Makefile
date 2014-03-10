CXX = g++
LDFLAGS = -lportmidi -lportaudio -Wall -g
CFLAGS = -I../coreAudioDevices -I../coreMidiDevices
CXXFLAGS =

SRCDIR = ./
OUTDIR = ../../bin/
EXE = 

HEADER = .hpp
SOURCE = .cpp
BINARY = .o

# Grab all files
FILES = 



# Generate list of source headers with extensions
HEADERS = $(addprefix $(SRCDIR), $(foreach SRCDIR, $(dirs), wildcard $(dir)/*.$(HEADER)))
# Generate list of source files with extensions
SOURCES = $(addprefix $(SRCDIR), $(foreach SRCDIR, $(dirs), wildcard $(dir)/*.$(SOURCE)))
# Get just the base names without extensions
OBJECTS = $(basename $(SOURCES))
# Generate binaries
BINARYS = $(addprefix $(OUTDIR), $(addsuffix $(BINARY), $(OBJECTS)))

all:
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c *.cpp $(LDFLAGS)
	mv *.o $(OUTDIR)

clean:
	rm -rf $(EXE) *.o
	
depend:
	gccmakedep -- $(LDFLAGS) -- $(SOURCES)
