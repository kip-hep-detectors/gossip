ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
LIBS         := -lgossip

CC = g++

all: example1

example1: example1.cpp
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $(LIBS) $^ -o $@

clean:
	@rm -f example1



############### End of Makefile ################

