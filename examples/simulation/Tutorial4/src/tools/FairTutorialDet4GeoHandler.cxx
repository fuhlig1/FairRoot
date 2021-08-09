/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                 CbmTrdGeoHandler source file                  -----
// -----                 Created 13/08/10  by F. Uhlig                 -----
// -------------------------------------------------------------------------

#include "FairTutorialDet4GeoHandler.h"

#include "FairLogger.h"   // for FairLogger, etc

#include <TGeoBBox.h>      // for TGeoBBox
#include <TGeoManager.h>   // for TGeoManager, gGeoManager
#include <TGeoNode.h>      // for TGeoNode
#include <TGeoVolume.h>    // for TGeoVolume
#include <TVirtualMC.h>    // for TVirtualMC
#include <cstdio>          // for printf
#include <cstring>         // for strlen, strncpy
#include <utility>         // for std::pair, std::make_pair

FairTutorialDet4GeoHandler::FairTutorialDet4GeoHandler()
    : TObject()
    , fIsSimulation(kFALSE)
    , fLastUsedDetectorID(0)
    , fGeoPathHash(0)
    , fCurrentVolume(nullptr)
    , fVolumeShape(nullptr)
    , fGlobal()
    , fGlobalMatrix(nullptr)
{}

Int_t FairTutorialDet4GeoHandler::Init(Bool_t isSimulation)
{
    // Int_t geoVersion = CheckGeometryVersion();

    fIsSimulation = isSimulation;

    return 1;
}

void FairTutorialDet4GeoHandler::LocalToGlobal(Double_t* local, Double_t* global, Int_t detID)
{
    TString path = ConstructFullPathFromDetID(detID);
    if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
    gGeoManager->LocalToMaster(local, global);
}

void FairTutorialDet4GeoHandler::GlobalToLocal(Double_t* global, Double_t* local, Int_t detID)
{
    TString path = ConstructFullPathFromDetID(detID);
    if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
    gGeoManager->MasterToLocal(global, local);
}


std::pair<UShort_t, UShort_t> FairTutorialDet4GeoHandler::CalculatePixelFromGlobalPos(Double_t* global, Int_t detID)
{
        Double_t local[3] = {0.,0.,0.};

        LOG(info) << "Detector ID" << detID;
        GlobalToLocal(global, local, detID);

        LOG(info) << "Position(global): " << global[0] << ", " << global[1] << ", " << global[2];
        LOG(info) << "Position(local): "  << local[0]  << ", " << local[1]  << ", " << local[2];

        return CalculatePixel(local, detID);
}

std::pair<UShort_t, UShort_t> FairTutorialDet4GeoHandler::CalculatePixel(Double_t* local, Int_t detID)
{

   // Calculate the pixel from the lowel left corner of the detector
   Double_t pixelSizeX=0.1; // 1mm, 0.1cm
   Double_t pixelSizeY=0.1; // 1mm, 0.1cm

   // Transform to local coordinate system with the origin in the
   // lower left corner of the detector
   TString path = ConstructFullPathFromDetID(detID);
   Double_t localX = local[0] + GetSizeX(path);
   Double_t localY = local[1] + GetSizeY(path);

   LOG(info) << "Position(local lower left): "  << localX  << ", " << localY  << ", " << local[2];

   // Calculate the pixel from the local position
   // The pixel is defined by row and columnn number
   UShort_t column = static_cast<UShort_t>( std::floor(localX/pixelSizeX) ) + 1;
   UShort_t row    = static_cast<UShort_t>( std::floor(localY/pixelSizeY) ) + 1;

   LOG(info) << "Pixel(column(x), row(y)): "  << column  << ", " << row;

   return std::make_pair(column, row);
}

Float_t FairTutorialDet4GeoHandler::GetSizeX(TString volName)
{
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  Float_t sizex = fVolumeShape->GetDX();
  return sizex;
}

Float_t FairTutorialDet4GeoHandler::GetSizeY(TString volName)
{
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  Float_t sizey = fVolumeShape->GetDY();
  return sizey;
}

Float_t FairTutorialDet4GeoHandler::GetSizeZ(TString volName)
{
  if (fGeoPathHash != volName.Hash()) { NavigateTo(volName); }
  Float_t sizez = fVolumeShape->GetDZ();
  return sizez;
}

TString FairTutorialDet4GeoHandler::ConstructFullPathFromDetID(Int_t detID)
{
    TString volStr = "/cave_1/tutorial4_0/tut4_det_";
    TString volPath = volStr;
    volPath += detID;
    return volPath;
}

Int_t FairTutorialDet4GeoHandler::GetUniqueDetectorId(TString volName)
{
    if (fGeoPathHash != volName.Hash()) {
        NavigateTo(volName);
    }
    return GetUniqueDetectorId();
}

Int_t FairTutorialDet4GeoHandler::GetUniqueDetectorId()
{

    Int_t detectorNr = 0;

    CurrentVolOffID(0, detectorNr);

    return detectorNr;
}

Int_t FairTutorialDet4GeoHandler::VolIdGeo(const char* name) const
{
    //
    // Return the unique numeric identifier for volume name
    //

    Int_t uid = gGeoManager->GetUID(name);
    if (uid < 0) {
        printf("VolId: Volume %s not found\n", name);
        return 0;
    }
    return uid;
}

Int_t FairTutorialDet4GeoHandler::VolId(const Text_t* name) const
{
    if (fIsSimulation) {
        return TVirtualMC::GetMC()->VolId(name);
    } else {
        //
        // Return the unique numeric identifier for volume name
        //
        std::string volname(name);
        while (!volname.empty() && volname.back() == ' ') {
            volname.pop_back();
        }
        return VolIdGeo(volname.c_str());
    }
}

Int_t FairTutorialDet4GeoHandler::CurrentVolID(Int_t& copy) const
{
    if (fIsSimulation) {
        return TVirtualMC::GetMC()->CurrentVolID(copy);
    } else {
        //
        // Returns the current volume ID and copy number
        //
        if (gGeoManager->IsOutside()) {
            return 0;
        }
        TGeoNode* node = gGeoManager->GetCurrentNode();
        copy = node->GetNumber();
        Int_t id = node->GetVolume()->GetNumber();
        return id;
    }
}

Int_t FairTutorialDet4GeoHandler::CurrentVolOffID(Int_t off, Int_t& copy) const
{
    if (fIsSimulation) {
        return TVirtualMC::GetMC()->CurrentVolOffID(off, copy);
    } else {
        //
        // Return the current volume "off" upward in the geometrical tree
        // ID and copy number
        //
        if (off < 0 || off > gGeoManager->GetLevel()) {
            return 0;
        }
        if (off == 0) {
            return CurrentVolID(copy);
        }
        TGeoNode* node = gGeoManager->GetMother(off);
        if (!node) {
            return 0;
        }
        copy = node->GetNumber();
        return node->GetVolume()->GetNumber();
    }
}

const char* FairTutorialDet4GeoHandler::CurrentVolName() const
{
    if (fIsSimulation) {
        return TVirtualMC::GetMC()->CurrentVolName();
    } else {
        //
        // Returns the current volume name
        //
        if (gGeoManager->IsOutside()) {
            return gGeoManager->GetTopVolume()->GetName();
        }
        return gGeoManager->GetCurrentVolume()->GetName();
    }
}

const char* FairTutorialDet4GeoHandler::CurrentVolOffName(Int_t off) const
{
    if (fIsSimulation) {
        return TVirtualMC::GetMC()->CurrentVolOffName(off);
    } else {
        //
        // Return the current volume "off" upward in the geometrical tree
        // ID, name and copy number
        // if name=0 no name is returned
        //
        if (off < 0 || off > gGeoManager->GetLevel()) {
            return 0;
        }
        if (off == 0) {
            return CurrentVolName();
        }
        TGeoNode* node = gGeoManager->GetMother(off);
        if (!node) {
            return 0;
        }
        return node->GetVolume()->GetName();
    }
}

void FairTutorialDet4GeoHandler::NavigateTo(TString volName)
{
    if (fIsSimulation) {
        LOG(fatal) << "This methode is not supported in simulation mode";
    } else {
        gGeoManager->cd(volName.Data());
        fGeoPathHash = volName.Hash();
        fCurrentVolume = gGeoManager->GetCurrentVolume();
        fVolumeShape = static_cast<TGeoBBox*>(fCurrentVolume->GetShape());
    }
}

ClassImp(FairTutorialDet4GeoHandler);
