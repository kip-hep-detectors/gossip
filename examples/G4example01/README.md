# Basic example for Geant4 integration of GosSiP

## Simulation Setup
LYSO scintillator + SiPM (for PET)
The scintillation yield is scaled down by a factor of 10 to increase simulation speed!
This can be changed in the `Ex01PhysicsList`.

## SiPM
The SiPM is implemented in the `SiPM` class and the corresponding sensitive detector class `SiPMSD`, a hit class `SiPMHit` and messenger class `SiPMMessenger`.
The input files `input/SiData.root` and `input/SpectralSensitivity.root` are required, as well as the example SiPM parameter file `input/MPPC_3x3.txt`.

There are two options to simulate the SiPM output:

#### Option1:
You can write out the photon information and simulate the SiPM output outside of the Geant4 simulation.
This is more flexible since you can change the sipm simulation parameters without having to re-run the G4 simulation.
Enable this via the UI command `/sipm/writePhotons true` or use the `WritePhotons(true)` function in the SiPMSD.
Set the output file via the UI command `/sipm/filenamePhotons` or `SetFilePhotons(string name)` in the SiPMSD.
The program `processPhotonOutput` shows how to work with the output data.

#### Option2:
You can directly simulate the sipm output during the Geant4 run.
This is only recommended if you want to process huge amounts of data and cannot store all the photon information on disk, as required for option 1.
Enable this via `/sipm/runGossip true` or use the `RunGossip(true)` function in the SiPMSD.
Set the output file via the UI command `/sipm/filenameGossip` or `SetFileGossip(string name)` in the SiPMSD.
The program `processGossipOutput` shows how to work with the output data.

Alternatively, you can also gather all the information from the HitCollection in the EventAction and process the data the way you want.
The `Ex01EventAction` class gives an example how this could look like.
