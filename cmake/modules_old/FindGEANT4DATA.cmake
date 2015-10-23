 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# - Try to find GEANT4 data files
#


MESSAGE(STATUS "Looking for GEANT4 DATA files...")

FIND_PATH(GEANT4_DATA NAMES $ENV{G4PIIDATA} G4PII G4PII1.3 PATHS
  ${SIMPATH}/share/Geant4/data
  ${Geant4_DIR}/share/$ENV{GEANT4VERSION}/data
  NO_DEFAULT_PATH
)


Message("G4LEDATA: $ENV{G4LEDATA}")

If(DEFINED ENV{G4LEDATA})
  Message("G4LEDATA defined")
EndIf()

If(NOT ENV{G4LEDATA})
  Message("G4LEDATA not defined")
Else()
  Message("G4LEDATA defined")
EndIf()


If(NOT ENV{G4LEDATA})
  FIND_PATH(G4LEDATA NAMES README PATHS
    ${GEANT4_DATA}/G4EMLOW
    NO_DEFAULT_PATH
  )
Else()
  Set(G4LEDATA $ENV{G4LEDATA})
EndIf()

If (G4LEDATA)
  Message(STATUS "Found G4EMLOW data")
Else (G4LEDATA)
  Message(STATUS "NOT Found G4EMLOW data")
EndIf (G4LEDATA)

If(NOT ENV{G4LEVELGAMMADATA})
  FIND_PATH(G4LEVELGAMMADATA  NAMES README-LevelGammaData PATHS
    ${GEANT4_DATA}/PhotonEvaporation/
    NO_DEFAULT_PATH
  )
Else()
  Set(G4LEVELGAMMADATA $ENV{G4LEVELGAMMADATA})
EndIf()

If (G4LEVELGAMMADATA)
  Message(STATUS "Found PhotonEvaporation data")
Else (G4LEVELGAMMADATA)
  Message(STATUS "NOT Found PhotonEvaporation data")
EndIf (G4LEVELGAMMADATA)


If(NOT ENV{G4NEUTRONHPDATA})
  FIND_PATH(G4NEUTRONHPDATA NAMES README  PATHS
    ${GEANT4_DATA}/G4NDL
    NO_DEFAULT_PATH
  )
Else()
  Set(G4NEUTRONHPDATA $ENV{G4NEUTRONHPDATA})
EndIf()

If (G4NEUTRONHPDATA)
  Message(STATUS "Found G4NDL data")
  SET(G4NeutronHPCrossSections ${G4NEUTRONHPDATA})
Else ()
  Message(STATUS "NOT Found G4NDL data")
EndIf ()

If(NOT ENV{G4NEUTRONXSDATA})
  FIND_PATH(G4NEUTRONXSDATA NAMES cap1  PATHS
    ${GEANT4_DATA}/G4NEUTRONXS
    NO_DEFAULT_PATH
  )
Else()
  Set(G4NEUTRONXSDATA $ENV{G4NEUTRONXSDATA})
EndIf()

If (G4NEUTRONXSDATA)
  Message(STATUS "Found G4NEUTRONXS data")
Else (G4NEUTRONXSDATA)
  Message(STATUS "NOT Found G4NEUTRONXS data")
EndIf (G4NEUTRONXSDATA)

If(NOT ENV{G4PIIDATA})
  FIND_PATH(G4PIIDATA NAMES PIXE_DataLibrary.pdf documentation.pdf PATHS
    ${GEANT4_DATA}/G4PII
    NO_DEFAULT_PATH
  )
Else()
  Set(G4PIIDATA $ENV{G4PIIDATA})
EndIf()


If (G4PIIDATA)
  Message(STATUS "Found G4PII data")
Else (G4PIIDATA)
  Message(STATUS "NOT Found G4PII data")
EndIf (G4PIIDATA)


If(NOT ENV{G4RADIOACTIVEDATA})
  FIND_PATH(G4RADIOACTIVEDATA NAMES README_RDM PATHS
    ${GEANT4_DATA}/RadioactiveDecay
    NO_DEFAULT_PATH
  )
Else()
  Set(G4RADIOACTIVEDATA $ENV{G4RADIOACTIVEDATA})
EndIf()

If (G4RADIOACTIVEDATA)
  Message(STATUS "Found RadioactiveDecay data")
Else (G4RADIOACTIVEDATA)
  Message(STATUS "NOT Found RadioactiveDecay data")
EndIf (G4RADIOACTIVEDATA)


If(NOT ENV{G4REALSURFACEDATA})
  FIND_PATH(G4REALSURFACEDATA NAMES EtchedTiO.dat PATHS
    ${GEANT4_DATA}/RealSurface
    NO_DEFAULT_PATH
  )
Else()
  Set(G4REALSURFACEDATA $ENV{G4REALSURFACEDATA})
EndIf()

If (G4REALSURFACEDATA)
  Message(STATUS "Found RealSurface data")
Else (G4REALSURFACEDATA)
  Message(STATUS "NOT Found RealSurface data")
EndIf (G4REALSURFACEDATA)

If(NOT ENV{G4SAIDXSDATA})
  FIND_PATH(G4SAIDXSDATA NAMES README_Eta PATHS
    ${GEANT4_DATA}/G4SAIDDATA
    NO_DEFAULT_PATH
  )
Else()
  Set(G4SAIDXSDATA $ENV{G4SAIDXSDATA})
EndIf()

If (G4SAIDXSDATA)
  Message(STATUS "Found G4SAID data")
Else (G4SAIDXSDATA)
  Message(STATUS "NOT Found G4SAID data")
EndIf (G4SAIDXSDATA)
