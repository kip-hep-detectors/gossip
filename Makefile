#
#            GosSiP Makefile
#

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
LIBS         := -L$(gossip)/lib -lGossip
INC          := -I$(gossip)/include
CC = g++

all: Dict.cpp Dict.o HitMatrix.o PhotonList.o PhotonSource.o sipmMC.o daqMC.o gossipGUI.o libGossip.so gossip

Dict.cpp: $(gossip)/include/HitMatrix.h $(gossip)/include/PhotonList.h $(gossip)/include/PhotonSource.h $(gossip)/include/sipmMC.h $(gossip)/include/daqMC.h $(gossip)/include/gossipGUI.h $(gossip)/include/LinkDef.h
	@echo "Generating Dictionary $@..."
	@rootcint -f src/$@ -c $^

Dict.o: src/Dict.cpp
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/Dict.cpp $(ROOTGLIBS)
	mv $@ lib

HitMatrix.o: src/HitMatrix.cpp include/HitMatrix.h
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/HitMatrix.cpp
	mv $@ lib

PhotonList.o: src/PhotonList.cpp include/PhotonList.h
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/PhotonList.cpp
	mv $@ lib

PhotonSource.o: src/PhotonSource.cpp include/PhotonSource.h
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/PhotonSource.cpp
	mv $@ lib

sipmMC.o: src/sipmMC.cpp include/sipmMC.h
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/sipmMC.cpp
	mv $@ lib

daqMC.o: src/daqMC.cpp include/daqMC.h
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/daqMC.cpp
	mv $@ lib

gossipGUI.o: src/gossipGUI.cpp include/gossipGUI.h
	@echo "Compiling $< ..."
	@$(CC) -fPIC $(INC) $(ROOTCFLAGS) -c src/gossipGUI.cpp
	mv $@ lib

libGossip.so: lib/gossipGUI.o lib/Dict.o lib/daqMC.o lib/sipmMC.o lib/HitMatrix.o lib/PhotonList.o lib/PhotonSource.o
	@echo "Compiling $< ..."
	@$(CC) -fPIC -shared $(INC) $(ROOTCFLAGS) -o lib/$@ $^ $(ROOTLIBS) $(ROOTGLIBS)

gossip: main.cpp
	@echo "Compiling $< ..."
	@$(CC) $(INC) $(ROOTCFLAGS) $^ -o $@ $(ROOTLIBS) $(ROOTGLIBS) $(LIBS)


clean:
	@rm -f src/Dict.cpp src/Dict_rdict.pcm lib/Dict.o lib/HitMatrix.o lib/PhotonList.o lib/PhotonSource.o lib/sipmMC.o lib/daqMC.o lib/gossipGUI.o lib/libGossip.so gossip



############### End of Makefile ################

