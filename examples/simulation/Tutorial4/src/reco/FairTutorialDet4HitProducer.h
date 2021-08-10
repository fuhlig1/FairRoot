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

/** FairTutorialDet4HitProducerIdealMissallign.h
 *@author Florian Uhlig <f.uhlig@gsi.de>
 **
 ** The hit producer produces hits of type CbmTutorialDetHit
 ** by calculation the hit positions from digi values. 
 ** The digi information is transformed to local coordinates and
 ** then transformed into global ccordinates. 
 **/

#ifndef FAIRTUTORIALDET4HITPRODUCER_H
#define FAIRTUTORIALDET4HITPRODUCER_H 1

#include "FairTask.h"   // for InitStatus, FairTask

#include <Rtypes.h>    // for Bool_t, Double_t, etc
#include <TArrayD.h>   // for TArrayD

class FairTutorialDet4GeoHandler;
class TClonesArray;

class FairTutorialDet4HitProducer : public FairTask
{
  public:
    /** Default constructor **/
    FairTutorialDet4HitProducer();

    /** Destructor **/
    ~FairTutorialDet4HitProducer() {}

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    virtual void SetParContainers();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

  private:
    /** Input array of CbmTofPoints **/
    TClonesArray* fDigiArray;   //!

    /** Output array of CbmTofHits **/
    TClonesArray* fHitArray;   //!

    FairTutorialDet4GeoHandler* fGeoHandler;

  private:

    FairTutorialDet4HitProducer(const FairTutorialDet4HitProducer&);
    FairTutorialDet4HitProducer& operator=(const FairTutorialDet4HitProducer&);

    ClassDef(FairTutorialDet4HitProducer, 1);
};

#endif
