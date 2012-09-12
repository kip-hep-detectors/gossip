#
#         Makefile for sipmMC
#

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
LIBS         := -L$(gossip)/lib -lHitMatrix -lPhotonList -lPhotonSource -lsipmMC -ldaqMC -lsipmGUI -lDict
INC          := -I$(gossip)/include
LDFLAGS      :="-Wl,--no-as-needed"
CC = g++

all: Dict.cxx Dict.o libDict.so HitMatrix.o libHitMatrix.so PhotonList.o libPhotonList.so PhotonSource.o libPhotonSource.so sipmMC.o libsipmMC.so daqMC.o libdaqMC.so sipmGUI.o libsipmGUI.so gossip


Dict.cxx: $(gossip)/include/HitMatrix.hh $(gossip)/include/PhotonList.hh $(gossip)/include/PhotonSource.hh $(gossip)/include/sipmMC.hh $(gossip)/include/daqMC.hh $(gossip)/include/sipmGUI.hh $(gossip)/include/LinkDef.h
	@echo "Generating Dictionary $@..."
	@rootcint -f src/$@ -c $^ 

Dict.o: src/Dict.cxx
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c $^
	mv $@ lib

libDict.so: lib/Dict.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

HitMatrix.o: src/HitMatrix.cc include/HitMatrix.hh
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/HitMatrix.cc
	mv $@ lib

libHitMatrix.so: lib/HitMatrix.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

PhotonList.o: src/PhotonList.cc include/PhotonList.hh
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/PhotonList.cc
	mv $@ lib

libPhotonList.so: lib/PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

PhotonSource.o: src/PhotonSource.cc include/PhotonSource.hh lib/PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/PhotonSource.cc
	mv $@ lib

libPhotonSource.so: lib/PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

sipmMC.o: src/sipmMC.cc include/sipmMC.hh lib/HitMatrix.o lib/PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/sipmMC.cc
	mv $@ lib

libsipmMC.so: lib/sipmMC.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

daqMC.o: src/daqMC.cc include/daqMC.hh lib/sipmMC.o lib/HitMatrix.o lib/PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/daqMC.cc
	mv $@ lib

libdaqMC.so: lib/daqMC.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

sipmGUI.o: src/sipmGUI.cc include/sipmGUI.hh lib/daqMC.o lib/sipmMC.o lib/HitMatrix.o lib/PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/sipmGUI.cc
	mv $@ lib

libsipmGUI.so: lib/sipmGUI.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

gossip: main.cc
	@echo "Compiling $< ..."
	@$(CC) $(INC) $(LDFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $^ -o $@ $(LIBS)


clean:
	@rm -f src/Dict.cxx src/Dict.h lib/Dict.o lib/libDict.so lib/HitMatrix.o lib/libHitMatrix.so lib/PhotonList.o lib/libPhotonList.so lib/PhotonSource.o lib/libPhotonSource.so lib/sipmMC.o lib/libsipmMC.so lib/daqMC.o lib/libdaqMC.so lib/sipmGUI.o lib/libsipmGUI.so gossip



############### End of Makefile ################

