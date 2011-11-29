#
#         Makefile for sipmMC
#

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
LIBS         := -L/home/patrick/SiPM_MC/ -lHitMatrix -lPhotonList -lPhotonSource -lsipmMC -ldaqMC -lsipmGUI -lDict
INC          := -I/home/patrick/SiPM_MC/
LDFLAGS      :="-Wl,--no-as-needed"
CC = g++


all: Dict.cxx Dict.o libDict.so HitMatrix.o libHitMatrix.so PhotonList.o libPhotonList.so PhotonSource.o libPhotonSource.so sipmMC.o libsipmMC.so daqMC.o libdaqMC.so sipmGUI.o libsipmGUI.so sipm


Dict.cxx: HitMatrix.hh PhotonList.hh PhotonSource.hh sipmMC.hh daqMC.hh sipmGUI.hh LinkDef.h
	@echo "Generating Dictionary $@..."
	@rootcint -f $@ -c $^

Dict.o: Dict.cxx
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(ROOTCFLAGS) -c $^

libDict.so: Dict.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(ROOTCFLAGS) -o $@ $^

HitMatrix.o: HitMatrix.cc HitMatrix.hh
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(ROOTCFLAGS) -c HitMatrix.cc

libHitMatrix.so: HitMatrix.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(ROOTCFLAGS) -o $@ $^

PhotonList.o: PhotonList.cc PhotonList.hh
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(ROOTCFLAGS) -c PhotonList.cc

libPhotonList.so: PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(ROOTCFLAGS) -o $@ $^

PhotonSource.o: PhotonSource.cc PhotonSource.hh PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(ROOTCFLAGS) -c PhotonSource.cc

libPhotonSource.so: PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(ROOTCFLAGS) -o $@ $^

sipmMC.o: sipmMC.cc sipmMC.hh HitMatrix.o PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(ROOTCFLAGS) -c sipmMC.cc

libsipmMC.so: sipmMC.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(ROOTCFLAGS) -o $@ $^

daqMC.o: daqMC.cc daqMC.hh sipmMC.o HitMatrix.o PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(ROOTCFLAGS) -c daqMC.cc

libdaqMC.so: daqMC.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(ROOTCFLAGS) -o $@ $^

sipmGUI.o: sipmGUI.cc sipmGUI.hh daqMC.o sipmMC.o HitMatrix.o PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(ROOTCFLAGS) -c sipmGUI.cc

libsipmGUI.so: sipmGUI.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(ROOTCFLAGS) -o $@ $^

sipm: main.cc
	@echo "Compiling $< ..."
	@$(CC) $(LDFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $^ -o $@ -L. -lHitMatrix -lPhotonList -lPhotonSource -lsipmMC -ldaqMC -lsipmGUI -lDict


clean:
	@rm -f Dict.cxx Dict.o libDict.so HitMatrix.o libHitMatrix.so PhotonList.o libPhotonList.so PhotonSource.o libPhotonSource.so sipmMC.o libsipmMC.so daqMC.o libdaqMC.so sipmGUI.o libsipmGUI.so sipm



############### End of Makefile ################

