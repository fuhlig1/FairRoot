# Environment Variables

In the following section the environment variables used by FairRoot are documented. Some of these variables are mandatory for proper working of
FairRoot, some of them are optional. 

## Build time environment variables

* **SIMPATH:** This variable is mandatory during the CMake configuration stage. The variable defines the FairSoft installation directory such that CMake
is able to find all the needed FairRoot dependencies. The directory is used read-only.

## Run time environment variables (FairRoot)

* **VMCWORKDIR:** This variable is mandatory at run time and is needed such that FairRoot can find user defined information at run-time. FairRoot expect for example the VMC configurations and the geometry definitions in specific directories below VMCWORKDIR. FairRoot uses the files inside this directory read-only but the user is free to use the directory also for writting. Inside VMCWORKDIR the following directories and files are expected.
  
     * **gconfig:** This directory contains the configurations for the MC engines.
         * **g3Config.C:** Macro which creates the Geant3 Virtual Monte Carlo session and which contains the default configuration for Geant3. This file is used if no other config file is defined explicitely and when using the old ROOT macro based config scheme.
         * **g4Config.C:** Macro which creates the Geant4 Virtual Monte Carlo session and which contains the default configuration for Geant4. The file is used if no other config file is defined explicitely and when using the old ROOT macro based config scheme. From this macro also the Geant4 config file g4config.in is passed to Geant4.
         * **g4config.in:** Config file with Geant4 commands which is executed from g4Config.C.
         * **flConfig.C:** Default configuration for Fluka which is used if no other config file is defined explicitely and when using the old ROOT macro based config scheme.
         * **SetCuts.C:** Configuration macro for setting common cuts and processes for G3, G4 and Fluka. Specific cuts and processes for Geant3, Geant4 or Fluka should be set in the g3Config.C, g4Config.C or flConfig.C
         * **Geane.C:** Default configuration for Geane which is used if no other config file is defined explicitetly.         
         * **g4ConfigPostInit.C:** Macro which loads the Geant4 post configuration options from g4configPostInit.in. The macro is executed after the VMC is fully initialised. 
         * **g4configPostInit.in:** 
         * **g3Config.yaml:** Default configuration for Geant3 which is used if no other config file is defined explicitely and when using the new yaml based config scheme.
         * **g4Config.yaml:** Default configuration for Geant4 which is used if no other config file is defined explicitely and when using the new yaml based config scheme.

     * **input:** This directory contains input data which is needed for the FairUrqmdGenerator.
         * **urqmd_pdg.dat:** Data file which defines the mapping between UrQMD ids and PDG ids.
         * **urqmd.ftn14:** Input file for the FairUrqmdGenerator.
     * **geometry:** This directory contains the files needed when sing the old ASCII geometry package.
         * **media.geo:** File which defines all medias used in the geometry files.
         *  
Depending on the configuration scheme and the transport engine the files in the table are mandatory


|  - | Geant3 | Geant4 | Fluka |
| C | g3Config.C + SetCuts.C | g4Config.C + SetCuts.C | flConfig.C |
| yaml | g3Config.yaml | g4Config.yaml | --- |

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

