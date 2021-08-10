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
    Double_t row;
    Double_t column;

    // Loop over TofDigis
    Int_t nHits = 0;
    Int_t nDigis = fDigiArray->GetEntriesFast();
    for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
        digi = static_cast<FairTutorialDet4Digi*>(fDigiArray->At(iDigi));
        if (!digi) {
            continue;
        }

        // Detector ID
        detID  = digi->GetDetectorID();
        row    = digi->GetRowNumber();
        column = digi->GetColumnNumber();

        LOG(info) << "Detector(culumn, row): " << detID << "(" << column << ", " << row << ")";

        std::pair<TVector3, TVector3> position = fGeoHandler->CalculateGlobalPosFromPixel(column , row, detID);

        LOG(info) << "Position: " << position.first.X()
                  << ", " << position.first.Y()
                  << ", " << position.first.Z();

        // Create new hit
        new ((*fHitArray)[nHits]) FairTutorialDet4Hit(detID, digi->GetMCIndex(), position.first, position.second);
        nHits++;
    }
    // Event summary
    LOG(debug) << "Create " << nHits << " TutorialDetHits out of " << nDigis << " TutorilaDetDigis created.";
}


ClassImp(FairTutorialDet4HitProducer);
