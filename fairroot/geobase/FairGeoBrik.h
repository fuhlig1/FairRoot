/********************************************************************************
 * Copyright (C) 2014-2023 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef FAIRGEOBRIK_H
#define FAIRGEOBRIK_H

#include "FairGeoBasicShape.h"   // for FairGeoBasicShape

#include <Rtypes.h>   // for FairGeoBrik::Class, etc

class FairGeoTransform;
class FairGeoVolume;
class TArrayD;

class FairGeoBrik : public FairGeoBasicShape
{
  public:
    FairGeoBrik();
    ~FairGeoBrik() override;
    TArrayD* calcVoluParam(FairGeoVolume*) override;
    void calcVoluPosition(FairGeoVolume*, const FairGeoTransform&, const FairGeoTransform&) override;
    ClassDefOverride(FairGeoBrik, 0);   // class for geometry shape BOX or BRIK
};

#endif /* !FAIRGEOBRIK_H */
