# $Id: GNUmakefile,v 1.1 1999-01-07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := GossipEx01
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

# Set compilation flags for the root libraries
CPPFLAGS += $(shell root-config --cflags)
CPPFLAGS += "-Wl,--no-as-needed"
EXTRALIBS += $(shell root-config --libs)
EXTRALIBS += $(shell root-config --glibs)
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

cleanall: clean
	rm processPhotonOutput
	rm processGossipOutput
