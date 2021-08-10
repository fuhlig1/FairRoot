#include "FairTutorialDet4HitProducerQa.h"

#include "FairTutorialDet4Hit.h"
#include "FairTutorialDet4Point.h"

#include "FairBaseParSet.h"
#include "FairDetector.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "TClonesArray.h"
#include "TH1F.h"
#include "TMath.h"
#include "TObjArray.h"

#include <iostream>
using std::cout;
using std::endl;

// ---- Default constructor -------------------------------------------------

FairTutorialDet4HitProducerQa::FairTutorialDet4HitProducerQa() : FairTutorialDet4HitProducerQa("TrdHitProducerQa", "") {}
// --------------------------------------------------------------------------


// ---- Standard constructor ------------------------------------------------
FairTutorialDet4HitProducerQa::FairTutorialDet4HitProducerQa(const char* name, const char*)
  : FairTask(name)
  , fHitArray{nullptr}
  , fPointArray{nullptr}
  , fPullX{new TH1F("fPullX", "", 500, -10, 10)}
  , fPullY{new TH1F("fPullY", "", 500, -10, 10)}
  , fResX{new TH1F("fResX", "", 500, -10, 10)}
  , fResY{new TH1F("fResY", "", 500, -10, 10)}
{
}
// --------------------------------------------------------------------------


// ---- Destructor ---------------------------------------------------------
FairTutorialDet4HitProducerQa::~FairTutorialDet4HitProducerQa() {}
// --------------------------------------------------------------------------


// ---- Initialisation ------------------------------------------------------
InitStatus FairTutorialDet4HitProducerQa::Init()
{
   // Get RootManager
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman) {
        LOG(fatal) << "RootManager not instantised!";
        return kFATAL;
    }

    // Get the point array
    fPointArray = static_cast<TClonesArray*>(ioman->GetObject("TutorialDetPoint"));
    if (!fPointArray) {
        LOG(fatal) << "No TutorialDetPoint array!";
        return kFATAL;
    }

    // Get the hit array
    fHitArray = static_cast<TClonesArray*>(ioman->GetObject("TutorialDetHit"));
    if (!fHitArray) {
        LOG(fatal) << "No TutorialDetHit array!";
        return kFATAL;
    }

    return kSUCCESS;
}

// --------------------------------------------------------------------------


// ---- Task execution ------------------------------------------------------
void FairTutorialDet4HitProducerQa::Exec(Option_t*)
{
  // Declare variables


  FairTutorialDet4Hit* hit{nullptr};
  FairTutorialDet4Point* point{nullptr};

  // Event counters
  Int_t nHit = fHitArray->GetEntriesFast();

  // Loop over TRD hits
  for (Int_t iHit = 0; iHit < nHit; iHit++) {
    hit = static_cast<FairTutorialDet4Hit*>(fHitArray->At(iHit));
    if (!hit) continue;

    point = static_cast<FairTutorialDet4Point*>(fPointArray->At(hit->GetRefIndex()));
    if (!point) continue;

    Int_t detID = hit->GetDetectorID();

    if (detID == 1) {

      // compute the residulas and pulls for X and Y coordinate
      Double_t hitPosX = hit->GetX();
      Double_t pointPosX = point->GetX();
      Double_t resX = hitPosX - pointPosX;
      Double_t pullX  = resX / hit->GetDx();

      Double_t hitPosY = hit->GetY();
      Double_t pointPosY = point->GetY();
      Double_t resY = hitPosY - pointPosY;
      Double_t pullY  = resY / hit->GetDy();

      // fill histograms
      fResX->Fill(resX);
      fResY->Fill(resY);
      fPullX->Fill(pullX);
      fPullY->Fill(pullY);
    }
  }
}

// --------------------------------------------------------------------------


// ---- Finish --------------------------------------------------------------
void FairTutorialDet4HitProducerQa::Finish() { WriteHistograms(); }
// --------------------------------------------------------------------------


// ---- Prepare test histograms ---------------------------------------------


// ---- Write test histograms ------------------------------------------------

void FairTutorialDet4HitProducerQa::WriteHistograms()
{
  if (fPullX) fPullX->Write();
  if (fPullY) fPullY->Write();

  if (fResX) fResX->Write();
  if (fResY) fResY->Write();
}

// --------------------------------------------------------------------------

ClassImp(FairTutorialDet4HitProducerQa);
