#
#         Makefile for SiPM_MC
#

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
CC = g++

all: Dict.cxx Dict.o HitMatrix.o SiPM_MC.o sipm


Dict.cxx: HitMatrix.h SiPM_MC.h SiPM_GUI.h LinkDef.h
	@echo "Generating Dictionary $@..."
	@rootcint -f $@ -c $^

Dict.o: Dict.cxx
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) -c $^

HitMatrix.o: HitMatrix.cpp HitMatrix.h
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) -c HitMatrix.cpp

SiPM_MC.o: SiPM_MC.cpp SiPM_MC.h HitMatrix.o
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) -fopenmp -c SiPM_MC.cpp

sipm: SiPM_GUI.cpp SiPM_GUI.h HitMatrix.o SiPM_MC.o Dict.o
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $^ -fopenmp -o $@
	./sipm

clean:
	@rm -f Dict.o HitMatrix.o SiPM_MC.o Dict.cxx sipm Dict.h



############### End of Makefile ################

