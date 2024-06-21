# Environment Variables

In the following section the environment variables used by FairRoot are
documented. Some of these variables are mandatory for proper working of
FairRoot, some of them are optional. 

## Build time environment variables

* **SIMPATH:** This variable is mandatory during the CMake configuration stage. The variable defines the FairSoft installation directory such that CMake
is able to find all the needed FairRoot dependencies. The directory is used read-only.

## Run time environment variables (FairRoot)

* **VMCWORKDIR:** This variable is mandatory at run time and is needed such that FairRoot can find user defined information at run-time. FairRoot expect for example the 
VMC configurations and the geometry definitions in specific directories below VMCWORKDIR. FairRoot uses the files inside this directory read-only but the user is free to
use the directory also for writting.

## Run time environment variables (Dependencies)

* **G3SYS:** This variable is mandatory when running Geant3 simulations and defines the data directory which contains some files needed by Geant3.

* **G4NEUTRONHPDATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories.
* **G4LEDATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories.
* **G4LEVELGAMMADATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories.
* **G4RADIOACTIVEDATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories.
* **G4PARTICLEXSDATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories.
* **G4PIIDATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories.
* **G4REALSURFACEDATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories. 
* **G4SAIDXSDATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories.
* **G4ABLADATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories. 
* **G4INCLDATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories.
* **G4ENSDFSTATEDATA:** This variable is mandatory when running Geant4 simulations and defines one of the several Geant4 data directories.

