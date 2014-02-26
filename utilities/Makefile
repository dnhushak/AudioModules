CC = g++
LDFLAGS = -lportaudio -Wall -ggdb -g
SRCS = PATest.cpp
EXE = PATest

PATest:
	$(CC) -o $(EXE) $(SRCS) $(LDFLAGS)

clean:
	rm -rf $(EXE)

depend:
	gccmakedep -- $(LDFLAGS) -- $(SRCS)