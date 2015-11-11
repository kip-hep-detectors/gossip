![Logo](gossip_logo.png "GosSiP Logo")

#Introduction
**What can the simulation do for you?**

The simulation models the response (signal waveform and charge) of a SiPM to an arbitrary
light pulse. In order to model a specific SiPM, you have to provide a set of basic parameters
for the simulation input. For a precise simulating of the response, the input parameters have
to be carefully measured!

**What can the simulation NOT do?**

The simulation does not simulate the avalanche process itself and therefore cannot predict the
basic parameters (e.g. PDE, gain, dark-rate, single pixel waveform,...) and their voltage and
temperature dependence. This would require significantly more simulation effort and depends
on the details of the doping structure which is typically not accessible. In the GosSiP simulation
the avalanche process is parameterised by the basic SiPM parameters, which can easily be
measured (or taken from the data sheet).

**What are typical application?**

* Quickly compare different sensors for a specific application.
* Simulate the performance of a detector system using SiPMs (e.g. SiPM + scintillator).
* Analyse which SiPM parameters / noise sources limit the performance for your application.

# Getting Started

## Requirements:
- cmake
- [ROOT](https://root.cern.ch/)

## Getting the code:
from [Github](https://github.com/kip-hep-detectors/gossip):
```bash
git clone https://github.com/kip-hep-detectors/test.git
```
or from [Bitbucket](https://bitbucket.org/kip-hep-detectors/gossip):
```bash
git clone git@bitbucket.org:kip-hep-detectors/gossip.git
```


##Installation on Linux, OSX

**Create a build directory:**
```bash
cd gossip
mkdir build
cd build
```


**Run cmake:**
```bash
cmake ..
make
sudo make install
cd build
```


**You're ready to go:**
```bash
gossip
```


##Uninstall
In your build directory, run:
```bash
sudo make uninstall
```


##Installation on Windows
You need to install [Cygwin](https://www.cygwin.com/) to emulate a Unix shell. Follow
the Linux installation instructions using the Cygwin terminal.
