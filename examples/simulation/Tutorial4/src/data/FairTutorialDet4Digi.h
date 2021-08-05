/********************************************************************************
 *    Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef FAIRTUTORIALDET4DIGI_H
#define FAIRTUTORIALDET4DIGI_H 1

#include "FairMultiLinkedData_Interface.h"   // for FairMultiLinkedData

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, UShort_t, Long64_t, ULong64_t

#include <string>  // for string

class FairTutorialDet4Digi : public FairMultiLinkedData_Interface 
{

public:
  /** Default constructor **/
  FairTutorialDet4Digi() {}


  /** Standard constructor
   ** @param  address  Unique element address
   ** @param  channel  Channel number
   ** @param  time     Measurement time [ns]
   ** @param  charge   Charge [ADC units]
   **/
  FairTutorialDet4Digi(Int_t address, UShort_t row, UShort_t column, Double_t time, Double_t charge)
    : fTime(time)
    , fAddress(address)
    , fRow(row)
    , fColumn(column)
    , fCharge(charge)
  {
  }


  /** Destructor **/
  ~FairTutorialDet4Digi() {};


  /** Unique detector element address  (see CbmStsAddress)
   ** @value Unique address of readout channel
   **/
  Int_t GetAddress() const { return fAddress; }


  /** @brief row number in module 
   ** @value row number
   **/
  UShort_t GetRowNumber() const { return fRow; }

  /** @brief column number in module 
   ** @value column number
   **/
  UShort_t GetColumnNumber() const { return fColumn; }


  /** Charge
   ** @value Charge [ADC units]
   **/
  Double_t GetCharge() const { return fCharge; }


  /** Time of measurement
   ** @value Time [ns]
   **/
  Double_t GetTime() const { return fTime; }


  /** String output **/
  std::string ToString() const;


private:

  Double_t fTime{0.};  ///< Time [ns]
  Int_t fAddress{0};   ///< Unique element address
  UShort_t fRow{0};    ///< Row number
  UShort_t fColumn{0}; ///< Column number
  Double_t fCharge{0}; ///< Charge 


  ClassDef(FairTutorialDet4Digi, 1);
};


#endif
