/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----            FairBoxGenerator source file                        -----
// -----          Created 09/09/04  by Yu.Kharlov
// -------------------------------------------------------------------------

/* $Id: FairBoxGenerator.cxx,v 1.4 2006/07/18 09:28:06 prokudin Exp $ */

/* History of cvs commits:
 *
 * $Log: FairBoxGenerator.cxx,v $
 * Revision 1.4  2006/07/18 09:28:06  prokudin
 * Should be * instead /
 *
 * Revision 1.3  2006/07/14 11:23:57  kharlov
 * Add protection for simultaneously set ranges; split vertex and kinematics ranges
 *
 * Revision 1.2  2006/03/29 16:25:50  kharlov
 * New functionality added
 *
 */

#include "FairBoxGenerator.h"

#include "FairPrimaryGenerator.h"

#include <TRandom.h>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>
#include <TMath.h>
#include <TNamed.h>

#include <cmath> // for cos, acos
#include <cstdio> // for printf

FairBoxGenerator::FairBoxGenerator() :
  FairBaseMCGenerator(),fPtMin(0),fPtMax(0),
  fPhiMin(0),fPhiMax(0),fEtaMin(0),fEtaMax(0),fYMin(0),fYMax(0),
  fPMin(0),fPMax(0),fThetaMin(0),fThetaMax(0),
  fEkinMin(0),fEkinMax(0),
  fEtaRangeIsSet(0),fYRangeIsSet(0),fThetaRangeIsSet(0),
  fCosThetaIsSet(0),fPtRangeIsSet(0),fPRangeIsSet(0),
  fDebug(0),fEkinRangeIsSet(0)
{
  // Default constructor
}

FairBoxGenerator::FairBoxGenerator(Int_t pdgid, Int_t mult) :
        FairBaseMCGenerator(),
  fPtMin(0),fPtMax(0),
  fPhiMin(0),fPhiMax(0),fEtaMin(0),fEtaMax(0),fYMin(0),fYMax(0),
  fPMin(0),fPMax(0),fThetaMin(0),fThetaMax(0),
  fEkinMin(0),fEkinMax(0),
  fEtaRangeIsSet(0), fYRangeIsSet(0),fThetaRangeIsSet(0),
  fCosThetaIsSet(0), fPtRangeIsSet(0), fPRangeIsSet(0),
 fDebug(0), fEkinRangeIsSet(0)
{
  // Constructor. Set default kinematics limits
  SetPDGType(pdgid);
  SetMultiplicity(mult);
  SetPhiRange();
}

FairBoxGenerator::FairBoxGenerator(const FairBoxGenerator& rhs) :
        FairBaseMCGenerator(rhs),
  fPtMin(rhs.fPtMin),fPtMax(rhs.fPtMax),
  fPhiMin(rhs.fPhiMin),fPhiMax(rhs.fPhiMax),fEtaMin(rhs.fEtaMin),
  fEtaMax(rhs.fEtaMax),fYMin(rhs.fYMin),fYMax(rhs.fYMax),
  fPMin(rhs.fPMin),fPMax(rhs.fPMax),fThetaMin(rhs.fThetaMin),
  fThetaMax(rhs.fThetaMax),
  fEkinMin(rhs.fEkinMin),fEkinMax(rhs.fEkinMax),
  fEtaRangeIsSet(rhs.fEtaRangeIsSet), fYRangeIsSet(rhs.fYRangeIsSet),
  fThetaRangeIsSet(rhs.fThetaRangeIsSet),fCosThetaIsSet(rhs.fCosThetaIsSet),
  fPtRangeIsSet(rhs.fPtRangeIsSet), fPRangeIsSet(rhs.fPRangeIsSet),
  fDebug(rhs.fDebug), fEkinRangeIsSet(rhs.fEkinRangeIsSet)
{
  // Copy constructor
}

FairBoxGenerator& FairBoxGenerator::operator=(const FairBoxGenerator& rhs)
{
  // Assignment operator

  // check assignment to self
  if (this == &rhs) return *this;

  // base class assignment
  FairBaseMCGenerator::operator=(rhs);

  // assignment operator
  fPtMin = rhs.fPtMin;
  fPtMax = rhs.fPtMax;
  fPhiMin = rhs.fPhiMin;
  fPhiMax = rhs.fPhiMax;
  fEtaMin = rhs.fEtaMin;
  fEtaMax = rhs.fEtaMax;
  fYMin = rhs.fYMin;
  fYMax = rhs.fYMax;
  fPMin = rhs.fPMin;
  fPMax = rhs.fPMax;
  fThetaMin = rhs.fThetaMin;
  fThetaMax = rhs.fThetaMax;
  fEkinMin = rhs.fEkinMin;
  fEkinMax = rhs.fEkinMax;
  fEtaRangeIsSet = rhs.fEtaRangeIsSet;
  fYRangeIsSet = rhs.fYRangeIsSet;
  fThetaRangeIsSet = rhs.fThetaRangeIsSet;
  fCosThetaIsSet = rhs.fCosThetaIsSet;
  fPtRangeIsSet = rhs.fPtRangeIsSet;
  fPRangeIsSet = rhs.fPRangeIsSet;
  fDebug = rhs.fDebug;
  fEkinRangeIsSet = rhs.fEkinRangeIsSet;

  return *this;
}

void FairBoxGenerator::SetXYZ(Double32_t x, Double32_t y, Double32_t z){
    SetVertex(x,y,z, 0, 0, 0, kBox);
}

void FairBoxGenerator::SetBoxXYZ(Double32_t x1, Double32_t y1, Double32_t x2, Double32_t y2, Double32_t z){
    Double_t X1  = TMath::Min(x1,x2);
    Double_t X2 = TMath::Max(x1,x2);
    Double_t Y1  = TMath::Min(y1,y2);
    Double_t Y2 = TMath::Max(y1,y2);
    Double_t dX = 0.5*(X2 - X1);
    Double_t dY = 0.5*(Y2 - Y1);
    Double_t x   = 0.5*(X1+X2);
    Double_t y  = 0.5*(Y1+Y2);
    SetVertex(x, y, z, dX, dY, 0, kBox);
}

Bool_t  FairBoxGenerator::Init()
{
  // Initialize parent generator
  if(FairBaseMCGenerator::Init()==kFALSE) return kFALSE;
  // Check for particle type
  TDatabasePDG* pdgBase = TDatabasePDG::Instance();
  TParticlePDG* particle = pdgBase->GetParticle(GetPDGType());

  if (! particle) {
    Fatal("FairBoxGenerator","PDG code %d not defined.",GetPDGType());
  }

  if (fPhiMax-fPhiMin>360){
    Fatal("Init()","FairBoxGenerator: phi range is too wide: %f<phi<%f",
          fPhiMin,fPhiMax);
  }
  if (fEkinRangeIsSet){
    if (fPRangeIsSet){
      Fatal("Init()","FairBoxGenerator: Cannot set P and Ekin ranges simultaneously");
    } else {
      // Transform EkinRange to PRange, calculate momentum in GeV, p = √(K² + 2Kmc²)
      fPMin = TMath::Sqrt(fEkinMin*fEkinMin + 2*fEkinMin*GetPDGMass());
      fPMax = TMath::Sqrt(fEkinMax*fEkinMax + 2*fEkinMax*GetPDGMass());
      fPRangeIsSet = kTRUE;
      fEkinRangeIsSet = kFALSE;
    }
  }
  if (fPRangeIsSet && fPtRangeIsSet) {
    Fatal("Init()","FairBoxGenerator: Cannot set P and Pt ranges simultaneously");
  }
  if (fPRangeIsSet && fYRangeIsSet) {
    Fatal("Init()","FairBoxGenerator: Cannot set P and Y ranges simultaneously");
  }
  if ( (fThetaRangeIsSet && fYRangeIsSet) ||
       (fThetaRangeIsSet && fEtaRangeIsSet) ||
       (fYRangeIsSet     && fEtaRangeIsSet) ) {
    Fatal("Init()","FairBoxGenerator: Cannot set Y, Theta or Eta ranges simultaneously");
  }
  return kTRUE;
}

Bool_t FairBoxGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  // Generate one event: produce primary particles emitted from one vertex.
  // Primary particles are distributed uniformly along
  // those kinematics variables which were limitted by setters.
  // if SetCosTheta() function is used, the distribution will be uniform in
  // cos(theta)

  Double32_t pabs=0, phi, pt=0, theta=0, eta, y, mt, px, py, pz=0;
  GenerateEventParameters();
  // Generate particles
  for (Int_t k = 0; k < GetMultiplicity(); k++) {
    phi = gRandom->Uniform(fPhiMin,fPhiMax) * TMath::DegToRad();

    if      (fPRangeIsSet ) { pabs = gRandom->Uniform(fPMin,fPMax); }
    else if (fPtRangeIsSet) { pt   = gRandom->Uniform(fPtMin,fPtMax); }

    if      (fThetaRangeIsSet) {
      if (fCosThetaIsSet)
        theta = acos(gRandom->Uniform(cos(fThetaMin* TMath::DegToRad()),
                                      cos(fThetaMax* TMath::DegToRad())));
      else {
        theta = gRandom->Uniform(fThetaMin,fThetaMax) * TMath::DegToRad();
      }
    } else if (fEtaRangeIsSet) {
      eta   = gRandom->Uniform(fEtaMin,fEtaMax);
      theta = 2*TMath::ATan(TMath::Exp(-eta));
    } else if (fYRangeIsSet) {
      y     = gRandom->Uniform(fYMin,fYMax);
      mt = TMath::Sqrt(GetPDGMass()*GetPDGMass() + pt*pt);
      pz = mt * TMath::SinH(y);
    }

    if (fThetaRangeIsSet || fEtaRangeIsSet) {
      if      (fPRangeIsSet ) {
        pz = pabs*TMath::Cos(theta);
        pt = pabs*TMath::Sin(theta);
      } else if (fPtRangeIsSet) {
        pz = pt/TMath::Tan(theta);
      }
    }

    px = pt*TMath::Cos(phi);
    py = pt*TMath::Sin(phi);

    if (fDebug)
      printf("BoxGen: kf=%d, p=(%.2f, %.2f, %.2f) GeV, x=(%.1f, %.1f, %.1f) cm\n",
             GetPDGType(), px, py, pz, fX, fY, fZ);

    primGen->AddTrack(GetPDGType(), px, py, pz, fX, fY, fZ);
  }
  return kTRUE;

}

FairGenerator* FairBoxGenerator::CloneGenerator() const
{
  // Clone for worker (used in MT mode only)

  return new FairBoxGenerator(*this);
}

ClassImp(FairBoxGenerator)
