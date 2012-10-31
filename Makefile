#
#         Makefile for sipmMC
#

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
LIBS         := -L$(gossip)/lib -lHitMatrix -lPhotonList -lPhotonSource -lsipmMC -ldaqMC -lgossipGUI -lDict
INC          := -I$(gossip)/include
LDFLAGS      :="-Wl,--no-as-needed"
CC = g++

all: Dict.cxx Dict.o libDict.so HitMatrix.o libHitMatrix.so PhotonList.o libPhotonList.so PhotonSource.o libPhotonSource.so sipmMC.o libsipmMC.so daqMC.o libdaqMC.so gossipGUI.o libgossipGUI.so gossip


Dict.cxx: $(gossip)/include/HitMatrix.h $(gossip)/include/PhotonList.h $(gossip)/include/PhotonSource.h $(gossip)/include/sipmMC.h $(gossip)/include/daqMC.h $(gossip)/include/gossipGUI.h $(gossip)/include/LinkDef.h
	@echo "Generating Dictionary $@..."
	@rootcint -f src/$@ -c $^ 

Dict.o: src/Dict.cxx
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c $^
	mv $@ lib

libDict.so: lib/Dict.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

HitMatrix.o: src/HitMatrix.cpp include/HitMatrix.h
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/HitMatrix.cpp
	mv $@ lib

libHitMatrix.so: lib/HitMatrix.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

PhotonList.o: src/PhotonList.cpp include/PhotonList.h
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/PhotonList.cpp
	mv $@ lib

libPhotonList.so: lib/PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

PhotonSource.o: src/PhotonSource.cpp include/PhotonSource.h lib/PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/PhotonSource.cpp
	mv $@ lib

libPhotonSource.so: lib/PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

sipmMC.o: src/sipmMC.cpp include/sipmMC.h lib/HitMatrix.o lib/PhotonList.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/sipmMC.cpp
	mv $@ lib

libsipmMC.so: lib/sipmMC.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

daqMC.o: src/daqMC.cpp include/daqMC.h lib/sipmMC.o lib/HitMatrix.o lib/PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/daqMC.cpp
	mv $@ lib

libdaqMC.so: lib/daqMC.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

gossipGUI.o: src/gossipGUI.cpp include/gossipGUI.h lib/daqMC.o lib/sipmMC.o lib/HitMatrix.o lib/PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/gossipGUI.cpp
	mv $@ lib

libgossipGUI.so: lib/gossipGUI.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^

gossip: main.cpp
	@echo "Compiling $< ..."
	@$(CC) $(INC) $(LDFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $^ -o $@ $(LIBS)


clean:
	@rm -f src/Dict.cxx src/Dict.h lib/Dict.o lib/libDict.so lib/HitMatrix.o lib/libHitMatrix.so lib/PhotonList.o lib/libPhotonList.so lib/PhotonSource.o lib/libPhotonSource.so lib/sipmMC.o lib/libsipmMC.so lib/daqMC.o lib/libdaqMC.so lib/gossipGUI.o lib/libgossipGUI.so gossip



############### End of Makefile ################

