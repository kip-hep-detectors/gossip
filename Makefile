#
#         Makefile for sipmMC
#

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
CC = g++


all: Dict.cxx Dict.o HitMatrix.o PhotonList.o PhotonSource.o sipmMC.o daqMC.o sipm


Dict.cxx: HitMatrix.hh PhotonList.hh PhotonSource.hh sipmMC.hh daqMC.hh sipmGUI.hh LinkDef.h
	@echo "Generating Dictionary $@..."
	@rootcint -f $@ -c $^

Dict.o: Dict.cxx
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) -c $^

HitMatrix.o: HitMatrix.cc HitMatrix.hh
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) -c HitMatrix.cc

PhotonList.o: PhotonList.cc PhotonList.hh
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) -c PhotonList.cc

PhotonSource.o: PhotonSource.cc PhotonSource.hh PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) -c PhotonSource.cc

sipmMC.o: sipmMC.cc sipmMC.hh HitMatrix.o PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) -c sipmMC.cc

daqMC.o: daqMC.cc daqMC.hh sipmMC.o HitMatrix.o PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) -c daqMC.cc

sipm: sipmGUI.cc sipmGUI.hh sipmMC.o daqMC.o PhotonSource.o HitMatrix.o PhotonList.o Dict.o
	@echo "Compiling $< ..."
	@$(CC) $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $^ -o $@
	./sipm


clean:
	@rm -f Dict.cxx Dict.o Dict.h HitMatrix.o PhotonList.o PhotonSource.o sipmMC.o daqMC.o sipm



############### End of Makefile ################

