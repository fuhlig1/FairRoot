/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "FairTutorialDet4Digitizer.h"

#include "FairTutorialDet4Point.h"
#include "FairTutorialDet4Digi.h"
#include "FairRootManager.h"

#include <TClonesArray.h>
#include <TVector3.h>

#include <utility>         // for std::pair

// ---- Default constructor -------------------------------------------
FairTutorialDet4Digitizer::FairTutorialDet4Digitizer()
    : FairTask("FairTutorialDet4Digitizer")
    , fGeoHandler(new FairTutorialDet4GeoHandler())
{
    LOG(debug) << "Defaul Constructor of FairTutorialDet4Digitizer";
}

// ---- Destructor ----------------------------------------------------
FairTutorialDet4Digitizer::~FairTutorialDet4Digitizer() { LOG(debug) << "Destructor of FairTutorialDet4Digitizer"; }

// ----  Initialisation  ----------------------------------------------
void FairTutorialDet4Digitizer::SetParContainers()
{
    LOG(debug) << "SetParContainers of FairTutorialDet4Digitizer";
    // Load all necessary parameter containers from the runtime data base
    /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <FairTutorialDet4DigitizerDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus FairTutorialDet4Digitizer::Init()
{
   // Get RootManager
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman) {
        LOG(fatal) << "RootManager not instantised!";
        return kFATAL;
    }

    // Get input array
    fPointArray = static_cast<TClonesArray*>(ioman->GetObject("TutorialDetPoint"));
    if (!fPointArray) {
        LOG(fatal) << "No TutorialDetPoint array!";
        return kFATAL;
    }

    // Create and register output array
    fDigiArray = new TClonesArray("FairTutorialDet4Digi");
    ioman->Register("TutorialDetDigi", "TutorialDet", fDigiArray, kTRUE);

    return kSUCCESS;
}

// ---- ReInit  -------------------------------------------------------
InitStatus FairTutorialDet4Digitizer::ReInit()
{
    LOG(debug) << "Initilization of FairTutorialDet4Digitizer";
    return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void FairTutorialDet4Digitizer::Exec(Option_t* /*option*/) 
{ 

    fDigiArray->Clear();

    // Declare some variables
    FairTutorialDet4Point* point = nullptr;
    Int_t detID = 0;   // Detector ID
    Double_t x, y, z;    // Position
    TVector3 pos, dpos;   // Position and error vectors

    // Loop over TutorialDerPoints
    Int_t nDigis = 0;
    Int_t nPoints = fPointArray->GetEntriesFast();
    LOG(debug) << "Number of points: " << nPoints;
    for (Int_t iPoint = 0; iPoint < nPoints; iPoint++) {
        point = static_cast<FairTutorialDet4Point*>(fPointArray->At(iPoint));
        if (!point) {
            continue;
        }

        // Detector ID
        detID = point->GetDetectorID();

        // Determine hit position
        x = point->GetX();
        y = point->GetY();
        z = point->GetZ();

        Double_t global[3] = {x, y, z};

        std::pair<UShort_t, UShort_t> pixels = fGeoHandler->CalculatePixelFromGlobalPos(global, detID);

        LOG(debug) << "Pixel(column(x), row(y)): "  << pixels.first  << ", " << pixels.second;

        new ((*fDigiArray)[nDigis])  FairTutorialDet4Digi(detID,
                                                          pixels.second,
                                                          pixels.first,
                                                          point->GetTime(),
                                                          point->GetEnergyLoss(),
                                                          iPoint
                                                         );

        nDigis++;

    }
}

// ---- Finish --------------------------------------------------------
void FairTutorialDet4Digitizer::Finish() { LOG(debug) << "Finish of FairTutorialDet4Digitizer"; }

ClassImp(FairTutorialDet4Digitizer);
