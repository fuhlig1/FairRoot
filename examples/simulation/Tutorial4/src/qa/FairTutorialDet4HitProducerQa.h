#ifndef FAIRTUTORIALDET4HITPRODUCERQA_H
#define FAIRTUTORIALDET4HITPRODUCERQA_H 1

#include "FairTask.h"

#include <TH1.h>

class TClonesArray;
//class TH1F;

class FairTutorialDet4HitProducerQa : public FairTask {
public:
  /* Defaul constructor */
  FairTutorialDet4HitProducerQa();

  /* Standard constructor */
  FairTutorialDet4HitProducerQa(const char* name, const char* title = "FairTask");

  /* Destructor */
  virtual ~FairTutorialDet4HitProducerQa();

  /* Initialisation */
  InitStatus Init();

  /* Execution */
  virtual void Exec(Option_t* option);

  /* Finish at the end of each event */
  virtual void Finish();


private:
  /* Data branches*/
  TClonesArray* fHitArray{nullptr};
  TClonesArray* fPointArray{nullptr};

  /* Test histograms*/
  TH1F* fPullX;  // = ((Hit - Point) / HitError)  in X
  TH1F* fPullY;  // = ((Hit - Point) / HitError)  in Y
  TH1F* fResX;   // = (Hit - Point) in X
  TH1F* fResY;   // = (Hit - Point) in X

private:
  FairTutorialDet4HitProducerQa(const FairTutorialDet4HitProducerQa&);
  FairTutorialDet4HitProducerQa& operator=(const FairTutorialDet4HitProducerQa&);

  //public:

  /* Write test histograms */
  void WriteHistograms();


  ClassDef(FairTutorialDet4HitProducerQa, 1)
};


#endif
