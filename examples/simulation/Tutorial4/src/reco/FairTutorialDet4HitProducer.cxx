/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// ------------------------------------------------------------
// -----     FairTutorialDet4HitProducer header file      -----
// -----          Created 10.08.21 by F. Uhlig            -----
// ------------------------------------------------------------
#include "FairTutorialDet4HitProducer.h"

#include "FairLogger.h"                   // for FairLogger, etc
#include "FairRootManager.h"              // for FairRootManager
#include "FairRunAna.h"                   // for FairRunAna
#include "FairRuntimeDb.h"                // for FairRuntimeDb
#include "FairTutorialDet4GeoHandler.h"   // for FairTutorialDet4GeoHandler
#include "FairTutorialDet4Hit.h"          // for FairTutorialDet4Hit
#include "FairTutorialDet4Digi.h"         // for FairTutorialDet4Digi

#include <TClonesArray.h>   // for TClonesArray
#include <TMath.h>          // for Cos, Sin
#include <TMathBase.h>      // for Abs
#include <TRandom.h>        // for TRandom, gRandom
#include <TVector3.h>       // for TVector3

FairTutorialDet4HitProducer::FairTutorialDet4HitProducer()
    : FairTask("Missallign Hit Producer for the TutorialDet")
    , fDigiArray(nullptr)
    , fHitArray(nullptr)
    , fGeoHandler(new FairTutorialDet4GeoHandler)
{}

void FairTutorialDet4HitProducer::SetParContainers()
{
}

InitStatus FairTutorialDet4HitProducer::ReInit()
{
    return kSUCCESS;
}

InitStatus FairTutorialDet4HitProducer::Init()
{
    // Get RootManager
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman) {
        LOG(fatal) << "RootManager not instantised!";
        return kFATAL;
    }

    // Get input array
    fDigiArray = static_cast<TClonesArray*>(ioman->GetObject("TutorialDetDigi"));
    if (!fDigiArray) {
        LOG(fatal) << "No TutorialDetDigi array!";
        return kFATAL;
    }

    // Create and register output array
    fHitArray = new TClonesArray("FairTutorialDet4Hit");
    ioman->Register("TutorialDetHit", "TutorialDet", fHitArray, kTRUE);

    LOG(info) << "HitProducerIdealMissallign: Initialisation successfull";

    return kSUCCESS;
}

void FairTutorialDet4HitProducer::Exec(Option_t* /*opt*/)
{
    fHitArray->Clear();

    // Declare some variables
    FairTutorialDet4Digi* digi = nullptr;
    Int_t detID = 0;   // Detector ID
    // Int_t trackID = 0;        // Track index
    Double_t x, y, z;    // Position
    Double_t dx = 0.1;   // Position error
    // Double_t tof = 0.;        // Time of flight
    TVector3 pos, dpos;   // Position and error vectors

    // Loop over TofDigis
    Int_t nHits = 0;
    Int_t nDigis = fDigiArray->GetEntriesFast();
    for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
        digi = static_cast<FairTutorialDet4Digi*>(fDigiArray->At(iDigi));
        if (!digi) {
            continue;
        }

        // Detector ID
        detID = digi->GetDetectorID();

        LOG(info) << "Detector(culumn, row): " << detID << "(" << digi->GetColumnNumber() << ", " << digi->GetRowNumber() << ")"; 

        // Determine hit position
/*
        x = point->GetX();
        y = point->GetY();
        z = point->GetZ();

        LOG(info) << "Position: " << x << ", " << y << ", " << z;
*/

/*
        Double_t local[3] = {x, y, z};
        Double_t global[3];

        fGeoHandler->LocalToGlobal(local, global, detID);

        x = global[0] + GetHitErr(0.1);
        y = global[1] + GetHitErr(0.1);
        z = global[2];
*/

/*
        LOG(info) << "Position: " << x << ", " << y << ", " << z;
        LOG(info) << "****";
*/
        // Time of flight
        // tof = point->GetTime();

        // Create new hit
/*        pos.SetXYZ(x, y, z);
        dpos.SetXYZ(dx, dx, 0.);
        new ((*fHitArray)[nHits]) FairTutorialDet4Hit(detID, iPoint, pos, dpos);
        nHits++;
*/
    }
    // Event summary
    LOG(debug) << "Create " << nHits << " TutorialDetHits out of " << nDigis << " TutorilaDetDigis created.";
}


ClassImp(FairTutorialDet4HitProducer);
