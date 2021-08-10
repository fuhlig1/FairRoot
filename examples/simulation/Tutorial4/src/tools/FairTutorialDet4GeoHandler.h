/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                 FairTutorialDet4GeoHandler header file        -----
// -----                 Created 20/11/12  by F. Uhlig                 -----
// -------------------------------------------------------------------------

/** FairTutorialDet4GeoHandler.h
 ** Helper class to extract information from the GeoManager which is
 ** needed in many other TOF classes. This helper class should be a
 ** single place to hold all these functions.
 ** @author F. Uhlig <f.uhlig@gsi.de>
 **/

#ifndef FAIRTUTORIALDET4GEOHANDLER_H
#define FAIRTUTORIALDET4GEOHANDLER_H 1

#include <Rtypes.h>    // for Int_t, Double_t, Bool_t, etc
#include <TObject.h>   // for TObject
#include <TString.h>   // for TString
#include <TVector3.h>  // for TVector3

#include <utility>    // for std::pair, std::make_pair

class TGeoBBox;
class TGeoVolume;
class TGeoHMatrix;

class FairTutorialDet4GeoHandler : public TObject
{
  public:
    /** Constructor **/
    FairTutorialDet4GeoHandler();

    /** Destructor **/
    ~FairTutorialDet4GeoHandler() {}

    Int_t GetUniqueDetectorId();
    Int_t GetUniqueDetectorId(TString volName);

    // Int_t GetDetectorId(Int_t uniqueId);

    Int_t Init(Bool_t isSimulation = kFALSE);

    void FillDetectorInfoArray(Int_t uniqueId);

    // Implement Interface functions to the TGeoManager to be
    // the same as for the VMC
    Int_t CurrentVolOffID(Int_t off, Int_t& copy) const;
    Int_t CurrentVolID(Int_t& copy) const;
    Int_t VolId(const Text_t* name) const;
    Int_t VolIdGeo(const char* name) const;
    const char* CurrentVolName() const;
    const char* CurrentVolOffName(Int_t off) const;

    void LocalToGlobal(Double_t* local, Double_t* global, Int_t detID);
    void GlobalToLocal(Double_t* global, Double_t* local, Int_t detID);

    std::pair<UShort_t, UShort_t> CalculatePixelFromGlobalPos(Double_t* global, Int_t detID);
    std::pair<TVector3, TVector3> CalculateGlobalPosFromPixel(UShort_t column, UShort_t row, Int_t detID);
  private:
    Bool_t fIsSimulation{kFALSE};   //!

    Int_t fLastUsedDetectorID{0};   //!

    Double_t fpixelSizeX{0.1};      //! 1mm, 0.1cm
    Double_t fpixelSizeY{0.1};      //! 1mm, 0.1cm

    UInt_t fGeoPathHash{0};                //!
    TGeoVolume* fCurrentVolume{nullptr};   //!
    TGeoBBox* fVolumeShape{nullptr};       //!


    TString ConstructFullPathFromDetID(Int_t detID);
    void NavigateTo(TString volName);

    Float_t GetSizeX(TString volName);
    Float_t GetSizeY(TString volName);
    Float_t GetSizeZ(TString volName);
//    void GetDetectorSize(Int_t detID, Double_t* detSize);
    std::pair<UShort_t, UShort_t> CalculatePixel(Double_t* local, Int_t detID);

    FairTutorialDet4GeoHandler(const FairTutorialDet4GeoHandler&);
    FairTutorialDet4GeoHandler operator=(const FairTutorialDet4GeoHandler&);

    ClassDef(FairTutorialDet4GeoHandler, 1);
};

#endif   // FAIRTUTORIALDETGEOHANDLER_H
