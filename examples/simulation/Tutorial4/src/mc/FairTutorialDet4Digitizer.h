/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef FAIRTUTORIALDET4DIGITIZER_H
#define FAIRTUTORIALDET4DIGITIZER_H

#include "FairTask.h"

#include "FairTutorialDet4GeoHandler.h"

class TClonesArray;

class FairTutorialDet4Digitizer : public FairTask
{
  public:
    /** Default constructor **/
    FairTutorialDet4Digitizer();

    /** Constructor with parameters (Optional) **/
    //  FairTutorialDet4Digitizer(Int_t verbose);

    /** Destructor **/
    ~FairTutorialDet4Digitizer();

    /** Initiliazation of task at the beginning of a run **/
    virtual InitStatus Init();

    /** ReInitiliazation of task when the runID changes **/
    virtual InitStatus ReInit();

    /** Executed for each event. **/
    virtual void Exec(Option_t* opt);

    /** Load the parameter container from the runtime database **/
    virtual void SetParContainers();

    /** Finish task called at the end of the run **/
    virtual void Finish();

  private:
    /** Input array of CbmTutorialDetPoints **/
    TClonesArray* fPointArray;   //!

    /** Output array of CbmTutorialDetDigis **/
    TClonesArray* fDigiArray;   //!

    FairTutorialDet4GeoHandler* fGeoHandler;

    /** Input array from previous already existing data level **/
    //  TClonesArray* <InputDataLevel>;

    /** Output array to  new data level**/
    //  TClonesArray* <OutputDataLevel>;

    void CalculatePixel(Double_t* local, Int_t detID);

    FairTutorialDet4Digitizer(const FairTutorialDet4Digitizer&);
    FairTutorialDet4Digitizer operator=(const FairTutorialDet4Digitizer&);

    ClassDef(FairTutorialDet4Digitizer, 1);
};

#endif
