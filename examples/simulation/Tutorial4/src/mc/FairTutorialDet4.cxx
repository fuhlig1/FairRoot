/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "FairTutorialDet4.h"

#include "FairDetectorList.h"             // for DetectorId::kTutDet
#include "FairLogger.h"                   // for FairLogger, etc
#include "FairModule.h"                   // for FairModule
#include "FairRootManager.h"              // for FairRootManager
#include "FairRun.h"                      // for FairRun
#include "FairRuntimeDb.h"                // for FairRuntimeDb
#include "FairStack.h"                    // for FairStack
#include "FairTutorialDet4GeoHandler.h"   // for FairTutorialDet4GeoHandler
#include "FairTutorialDet4GeoPar.h"       // for FairTutorialDet4GeoPar
#include "FairTutorialDet4Point.h"   // for FairTutorialDet4Point

#include <TString.h>           // for TString, operator<<, Form
#include <TVirtualMC.h>        // for TVirtualMC
#include <TVirtualMCStack.h>   // for TVirtualMCStack

FairTutorialDet4::FairTutorialDet4()
    : FairDetector("TutorialDet", kTRUE, kTutDet)
    , fTrackID(-1)
    , fVolumeID(-1)
    , fPos()
    , fMom()
    , fTime(-1.)
    , fLength(-1.)
    , fELoss(-1)
    , fFairTutorialDet4PointCollection(new TClonesArray("FairTutorialDet4Point"))
    , fGeoHandler(new FairTutorialDet4GeoHandler())
{}

FairTutorialDet4::FairTutorialDet4(const char* name, Bool_t active)
    : FairDetector(name, active, kTutDet)
    , fTrackID(-1)
    , fVolumeID(-1)
    , fPos()
    , fMom()
    , fTime(-1.)
    , fLength(-1.)
    , fELoss(-1)
    , fFairTutorialDet4PointCollection(new TClonesArray("FairTutorialDet4Point"))
    , fGeoHandler(new FairTutorialDet4GeoHandler())
{}

FairTutorialDet4::FairTutorialDet4(const FairTutorialDet4& rhs)
    : FairDetector(rhs)
    , fTrackID(-1)
    , fVolumeID(-1)
    , fPos()
    , fMom()
    , fTime(-1.)
    , fLength(-1.)
    , fELoss(-1)
    , fFairTutorialDet4PointCollection(new TClonesArray("FairTutorialDet4Point"))
    , fGeoHandler(new FairTutorialDet4GeoHandler())
{}

FairTutorialDet4::~FairTutorialDet4()
{
    LOG(debug4) << "Entering Destructor of FairTutorialDet4";
    if (fFairTutorialDet4PointCollection) {
        fFairTutorialDet4PointCollection->Delete();
        delete fFairTutorialDet4PointCollection;
    }
    LOG(debug4) << "Leaving Destructor of FairTutorialDet4";
}

void FairTutorialDet4::SetParContainers()
{
}

void FairTutorialDet4::Initialize()
{
    Bool_t isSimulation = kTRUE;
    fGeoHandler->Init(isSimulation);
}

void FairTutorialDet4::InitParContainers()
{
}

Bool_t FairTutorialDet4::ProcessHits(FairVolume* /*vol*/)
{
    /** This method is called from the MC stepping */

    // Set parameters at entrance of volume. Reset ELoss.
    if (TVirtualMC::GetMC()->IsTrackEntering()) {
        fELoss = 0.;
        fTime = TVirtualMC::GetMC()->TrackTime() * 1.0e09;
        fLength = TVirtualMC::GetMC()->TrackLength();
        TVirtualMC::GetMC()->TrackPosition(fPos);
        TVirtualMC::GetMC()->TrackMomentum(fMom);
    }

    // Sum energy loss for all steps in the active volume
    fELoss += TVirtualMC::GetMC()->Edep();

    // Create FairTutorialDet4Point at exit of active volume
    if (TVirtualMC::GetMC()->IsTrackExiting() || TVirtualMC::GetMC()->IsTrackStop()
        || TVirtualMC::GetMC()->IsTrackDisappeared()) {
        fTrackID = TVirtualMC::GetMC()->GetStack()->GetCurrentTrackNumber();
        // fVolumeID = vol->getMCid();
        fVolumeID = fGeoHandler->GetUniqueDetectorId();
        if (fELoss == 0.) {
            return kFALSE;
        }

        AddHit(fTrackID,
               fVolumeID,
               TVector3(fPos.X(), fPos.Y(), fPos.Z()),
               TVector3(fMom.Px(), fMom.Py(), fMom.Pz()),
               fTime,
               fLength,
               fELoss);

        // Increment number of tutorial det points in TParticle
        FairStack* stack = static_cast<FairStack*>(TVirtualMC::GetMC()->GetStack());
        stack->AddPoint(kTutDet);
    }

    return kTRUE;
}

void FairTutorialDet4::EndOfEvent() { fFairTutorialDet4PointCollection->Clear(); }

void FairTutorialDet4::Register()
{
    /** This will create a branch in the output tree called
      FairTutorialDet4Point, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

    FairRootManager::Instance()->Register("TutorialDetPoint", "TutorialDet", fFairTutorialDet4PointCollection, kTRUE);
}

TClonesArray* FairTutorialDet4::GetCollection(Int_t iColl) const
{
    if (iColl == 0) {
        return fFairTutorialDet4PointCollection;
    } else {
        return nullptr;
    }
}

void FairTutorialDet4::Reset() { fFairTutorialDet4PointCollection->Clear(); }

void FairTutorialDet4::ConstructGeometry()
{
    TString fileName = GetGeometryFileName();
    if (fileName.EndsWith(".geo")) {
        LOG(fatal) << "Constructing of Tutorial4 geometry from ASCII file not supported";
        LOG(fatal) << "Exiting.";
        exit(1);
    } else if (fileName.EndsWith(".root")) {
        LOG(info) << "Constructing Tutorial4 geometry from ROOT file " << fileName;
        ConstructRootGeometry();
    } else {
        LOG(fatal) << "Geometry format not supported.";
    }
}

Bool_t FairTutorialDet4::IsSensitive(const std::string& name)
{
    if (name.find("tut4") != std::string::npos) {
        return kTRUE;
    }
    return kFALSE;
}

FairTutorialDet4Point* FairTutorialDet4::AddHit(Int_t trackID,
                                                Int_t detID,
                                                TVector3 pos,
                                                TVector3 mom,
                                                Double_t time,
                                                Double_t length,
                                                Double_t eLoss)
{
    TClonesArray& clref = *fFairTutorialDet4PointCollection;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) FairTutorialDet4Point(trackID, detID, pos, mom, time, length, eLoss);
}

FairModule* FairTutorialDet4::CloneModule() const { return new FairTutorialDet4(*this); }

ClassImp(FairTutorialDet4);
