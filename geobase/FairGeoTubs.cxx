/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
//*-- AUTHOR : Ilse Koenig
//*-- Modified : 11/11/2003 by Ilse Koenig
//*-- Modified : 28/06/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// FairGeoTubs
//
// class for the GEANT shape TUBS
//
// The size of a TUBS is defined by 4 'points'.
//   point 0:   origin of starting circle of the tubs;
//   point 1:   inner radius of starting circle,
//              outer radius of starting circle;
//              (z-component not used)
//   point 2:   origin of ending circle of the tubs;
//   point 3:   starting angle of the segment,
//              ending angle of the segment;
//              (z-component not used)
// Warning: The x- and y-values of point 0 and 2 have to be the same!!!!
//          A rotation has to be desribed via the rotation matrix.
//
// The intrinsic coordinate system of a TUBS, which sits in the CAVE and is
// not rotated, is identical with the laboratory system.
//
///////////////////////////////////////////////////////////////////////////////
#include "FairGeoTubs.h"

#include "FairGeoTransform.h"           // for FairGeoTransform
#include "FairGeoVector.h"              // for FairGeoVector
#include "FairGeoVolume.h"              // for FairGeoVolume

#include "TArrayD.h"                    // for TArrayD
#include "TMathBase.h"                  // for Abs
#include "TString.h"                    // for TString

#include <stdio.h>                      // for printf, sprintf, sscanf
#include <string.h>                     // for strlen
#include <ostream>                      // for basic_ostream::write

ClassImp(FairGeoTubs)

FairGeoTubs::FairGeoTubs()
  :FairGeoBasicShape()
{
  // constructor
  fName="TUBS";
  nPoints=4;
  nParam=5;
  param=new TArrayD(nParam);
}


FairGeoTubs::~FairGeoTubs()
{
  // default destructor
  if (param) {
    delete param;
    param=0;
  }
  if (center) {
    delete center;
    center=0;
  }
  if (position) {
    delete position;
    position=0;
  }
}


Int_t FairGeoTubs::readPoints(fstream* pFile,FairGeoVolume* volu)
{
  // reads the 4 'points' decribed above from ascii file
  // if the array of points is not existing in the volume it is created and
  // the values are stored inside
  // returns the number of points
  if (!pFile) { return 0; }
  if (volu->getNumPoints()!=nPoints) { volu->createPoints(nPoints); }
  Double_t x,y,z;
  const Int_t maxbuf=155;
  Text_t buf[maxbuf];
  for(Int_t i=0; i<nPoints; i++) {
    pFile->getline(buf,maxbuf);
    sscanf(buf,"%lf%lf%lf",&x,&y,&z);
    volu->setPoint(i,x,y,z);
    i++;
    pFile->getline(buf,maxbuf);
    sscanf(buf,"%lf%lf",&x,&y);
    volu->setPoint(i,x,y,0.0);
  }
  return nPoints;
}


Bool_t FairGeoTubs::writePoints(fstream* pFile,FairGeoVolume* volu)
{
  // writes the 4 'points' decribed above to ascii file
  if (!pFile) { return kFALSE; }
  Text_t buf[155];
  for(Int_t i=0; i<nPoints; i++) {
    FairGeoVector& v=*(volu->getPoint(i));
    if (i==0 || i==2) { sprintf(buf,"%9.3f%10.3f%10.3f\n",v(0),v(1),v(2)); }
    else { sprintf(buf,"%9.3f%10.3f\n",v(0),v(1)); }
    pFile->write(buf,strlen(buf));
  }
  return kTRUE;
}


void FairGeoTubs::printPoints(FairGeoVolume* volu)
{
  // prints volume points to screen
  for(Int_t i=0; i<nPoints; i++) {
    FairGeoVector& v=*(volu->getPoint(i));
    if (i==0 || i==2) { printf("%9.3f%10.3f%10.3f\n",v(0),v(1),v(2)); }
    else { printf("%9.3f%10.3f\n",v(0),v(1)); }
  }
}


TArrayD* FairGeoTubs::calcVoluParam(FairGeoVolume* volu)
{
  // calculates the parameters needed to create the shape TUBS
  Double_t fac=10.;
  FairGeoVector& v1=*(volu->getPoint(1));
  param->AddAt(v1(0)/fac,0);
  param->AddAt(v1(1)/fac,1);
  FairGeoVector v=*(volu->getPoint(2)) - *(volu->getPoint(0));
  param->AddAt(TMath::Abs(v(2))/fac/2.,2);
  FairGeoVector& v3=*(volu->getPoint(3));
  param->AddAt(v3(0),3);
  param->AddAt(v3(1),4);
  return param;
}


void FairGeoTubs::calcVoluPosition(FairGeoVolume* volu,
                                   const FairGeoTransform& dTC,const FairGeoTransform& mTR)
{
  // calculates the position of the center of the volume in the intrinsic
  // coordinate system and stores it in the data element 'center'
  // calls the function posInMother(...) to calculate the position of the
  // volume in its mother
  Double_t t[3]= {0.,0.,0.};
  FairGeoVector v=*(volu->getPoint(2)) + *(volu->getPoint(0));
  t[2]=v(2)/2.;
  center->clear();
  center->setTransVector(t);
  posInMother(dTC,mTR);
}
