/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                FairPlutoGenerator source file                  -----
// -----          Created 13/07/04  by V. Friese / D.Bertini           -----
// -------------------------------------------------------------------------
#include "FairPlutoGenerator.h"

#include "FairPrimaryGenerator.h"       // for FairPrimaryGenerator
#include "FairLogger.h"                 // for logging

#include "PParticle.h"                  // for PParticle

#include "Riosfwd.h"                    // for ostream
#include "TClonesArray.h"               // for TClonesArray
#include "TDatabasePDG.h"               // for TDatabasePDG
#include "TFile.h"                      // for TFile, gFile
#include "TLorentzVector.h"             // for TLorentzVector
#include "TTree.h"                      // for TTree
#include "TVector3.h"                   // for TVector3

#include <stddef.h>                     // for NULL

// -----   Default constructor   ------------------------------------------
FairPlutoGenerator::FairPlutoGenerator()
  :FairGenerator(),
   iEvent(0),
   fFileName(NULL),
   fInputFile(NULL),
   fInputTree(NULL),
   fParticles(NULL)
{
}
// ------------------------------------------------------------------------



// -----   Standard constructor   -----------------------------------------
FairPlutoGenerator::FairPlutoGenerator(const Char_t* fileName)
  :FairGenerator(),
   iEvent(0),
   fFileName(fileName),
   fInputFile(new TFile(fileName)),
   fInputTree(NULL),
   fParticles(new TClonesArray("PParticle",100))
{
  fInputTree = (TTree*) fInputFile->Get("data");
  fInputTree->SetBranchAddress("Particles", &fParticles);
}
// ------------------------------------------------------------------------



// -----   Destructor   ---------------------------------------------------
FairPlutoGenerator::~FairPlutoGenerator()
{
  CloseInput();
}
// ------------------------------------------------------------------------



// -----   Public method ReadEvent   --------------------------------------
Bool_t FairPlutoGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{

  // Check for input file
  if ( ! fInputFile ) {
    LOG(ERROR) << "FairPlutoGenerator: Input file nor open!" 
	       << FairLogger::endl;
    return kFALSE;
  }

  // Check for number of events in input file
  if ( iEvent > fInputTree->GetEntries() ) {
    LOG(ERROR) << "FairPlutoGenerator: No more events in input file!" 
	       << FairLogger::endl;
    CloseInput();
    return kFALSE;
  }
  TFile*  g=gFile;
  fInputFile->cd();
  fInputTree->GetEntry(iEvent++);
  g->cd();

  // Get PDG database
  TDatabasePDG* dataBase = TDatabasePDG::Instance();

  // Get number of particle in TClonesrray
  Int_t nParts = fParticles->GetEntriesFast();

  // Loop over particles in TClonesArray
  for (Int_t iPart=0; iPart < nParts; iPart++) {
    PParticle* part = (PParticle*) fParticles->At(iPart);
    Int_t pdgType = dataBase->ConvertGeant3ToPdg( part->ID() );

    // Check if particle type is known to database
    if ( ! pdgType ) {
      LOG(WARNING) << "FairPlutoGenerator: Unknown type " << part->ID()
		   << ", skipping particle." << FairLogger::endl;
      continue;
    }

    TLorentzVector mom = part->Vect4();
    Double_t px = mom.Px();
    Double_t py = mom.Py();
    Double_t pz = mom.Pz();

    TVector3 vertex = part->getVertex();
    Double_t vx = vertex.x();
    Double_t vy = vertex.y();
    Double_t vz = vertex.z();

    // Give track to PrimaryGenerator
    primGen->AddTrack(pdgType, px, py, pz, vx, vy, vz);

  }        //  Loop over particle in event

  return kTRUE;

}
// ------------------------------------------------------------------------




// -----   Private method CloseInput   ------------------------------------
void FairPlutoGenerator::CloseInput()
{
  if ( fInputFile ) {
    LOG(INFO) << "FairPlutoGenerator: Closing input file " << fFileName
	      << FairLogger::endl;
    fInputFile->Close();
    delete fInputFile;
  }
  fInputFile = NULL;
}
// ------------------------------------------------------------------------



ClassImp(FairPlutoGenerator)
