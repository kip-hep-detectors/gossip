name := GossipEx01
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

# Set compilation flags for the root libraries
CPPFLAGS += $(shell root-config --cflags)
EXTRALIBS += $(shell root-config --libs)
EXTRALIBS += -lgossip

.PHONY: all
all: lib bin processGossipOutput processPhotonOutput

include $(G4INSTALL)/config/binmake.gmk


processPhotonOutput: processPhotonOutput.cpp
	@echo "Compiling $< ..."
	g++ $(CPPFLAGS) $(EXTRALIBS) $^ -o $@


processGossipOutput: processGossipOutput.cpp
	@echo "Compiling $< ..."
	g++ $(CPPFLAGS) $(EXTRALIBS) $^ -o $@

cleanall:
	make clean
	rm processPhotonOutput
	rm processGossipOutput
