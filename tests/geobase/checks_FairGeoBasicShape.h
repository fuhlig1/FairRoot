/********************************************************************************
 *   Copyright (C) 2024 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH     *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "FairGeoBasicShape.h"
#include "FairGeoRotation.h"
#include "FairGeoTransform.h"
#include "FairGeoVector.h"
#include "FairGeoVolume.h"
#include "generate_TestInput.h"

#include <TArrayD.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstdio>
#include <string>

using Catch::Matchers::WithinRel;

#ifndef TESTS_GEOBASE_CHECKS_FAIRGEOBASICSHAPE_H
#define TESTS_GEOBASE_CHECKS_FAIRGEOBASICSHAPE_H

namespace fairroot::tests
{

auto check_FairGeoTransform(FairGeoTransform& _trans)
{
    const FairGeoRotation& rot = _trans.getRotMatrix();
    REQUIRE(rot.isUnitMatrix());

    const FairGeoVector& vec = _trans.getTransVector();
    REQUIRE_THAT(vec.getValues(0), WithinRel(0., 0.001));
    REQUIRE_THAT(vec.getValues(1), WithinRel(0., 0.001));
    REQUIRE_THAT(vec.getValues(2), WithinRel(0., 0.001));

    const FairGeoVector& vec_cm = _trans.getTranslation();
    REQUIRE_THAT(vec_cm.getValues(0), WithinRel(0., 0.001));
    REQUIRE_THAT(vec_cm.getValues(1), WithinRel(0., 0.001));
    REQUIRE_THAT(vec_cm.getValues(2), WithinRel(0., 0.001));
}

auto check_FairGeoShape_Construction(FairGeoBasicShape& shape,
                                     std::string name,
                                     int numPoints,
                                     int numParams,
                                     TArrayD* _param)
{

    SECTION("construction")
    {
        // Check that members are correctly set by constructor
        REQUIRE(shape.GetName() == name);
        REQUIRE(shape.getNumPoints() == numPoints);
        REQUIRE(shape.getNumParam() == numParams);

        TArrayD* param = shape.getParam();
        if (!_param) {
            REQUIRE(param == _param);
        } else {
            REQUIRE(param->GetSize() == _param->GetSize());
        }

        FairGeoTransform* centerPos = shape.getCenterPosition();
        check_FairGeoTransform(*centerPos);

        FairGeoTransform* voluPos = shape.getVoluPosition();
        check_FairGeoTransform(*voluPos);
    }
}

auto check_FairGeoShape_ReadWrite(FairGeoBasicShape& shape,
                                  FairGeoVolume& volume,
                                  std::string shapeName,
                                  int numPoints,
                                  std::vector<std::vector<float>>& parameters)
{
    // generate temporary file with parameters below describing the shape defined by the shapeName
    // with the respective coordinates
    // The name of the temporary file is returned
    std::string fileName = fairroot::tests::generateVolumeParameter(shapeName, parameters);

    // open the file with the shape/volume parameters
    // read the content from file and close the file
    std::fstream geoFile{};
    geoFile.open(fileName, std::ios::in);
    shape.readPoints(&geoFile, &volume);
    geoFile.close();

    REQUIRE(volume.getNumPoints() == numPoints);
    for (int i = 0; i < numPoints; ++i) {
        FairGeoVector* test = volume.getPoint(i);
        if ("BOX" == shapeName || "TRAP" == shapeName || "TRD1" == shapeName) {
            REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
            REQUIRE_THAT(test->Y(), Catch::Matchers::WithinRel(parameters.at(1).at(i), 0.001f));
            REQUIRE_THAT(test->Z(), Catch::Matchers::WithinRel(parameters.at(2).at(i), 0.001f));
        } else if ("CONE" == shapeName || "CONS" == shapeName || "ELTU" == shapeName || "TUBE" == shapeName
                   || "TUBS" == shapeName) {
            if (0 == i || 2 == i) {
                REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
                REQUIRE_THAT(test->Y(), Catch::Matchers::WithinRel(parameters.at(1).at(i), 0.001f));
                REQUIRE_THAT(test->Z(), Catch::Matchers::WithinRel(parameters.at(2).at(i), 0.001f));
            } else {
                REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
                REQUIRE_THAT(test->Y(), Catch::Matchers::WithinRel(parameters.at(1).at(i), 0.001f));
            }
        } else if ("TORUS" == shapeName) {
            REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
        } else if ("SPHE" == shapeName) {
            REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
            REQUIRE_THAT(test->Y(), Catch::Matchers::WithinRel(parameters.at(1).at(i), 0.001f));
        } else if ("PCON" == shapeName) {
            if (0 == i) {
                REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
            } else if (1 == i) {
                REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
                REQUIRE_THAT(test->Y(), Catch::Matchers::WithinRel(parameters.at(1).at(i), 0.001f));
            } else {
                REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
                REQUIRE_THAT(test->Y(), Catch::Matchers::WithinRel(parameters.at(1).at(i), 0.001f));
                REQUIRE_THAT(test->Z(), Catch::Matchers::WithinRel(parameters.at(2).at(i), 0.001f));
            }
        } else if ("PGON" == shapeName) {
            if (0 == i) {
                REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
            } else {
                REQUIRE_THAT(test->X(), Catch::Matchers::WithinRel(parameters.at(0).at(i), 0.001f));
                REQUIRE_THAT(test->Y(), Catch::Matchers::WithinRel(parameters.at(1).at(i), 0.001f));
                REQUIRE_THAT(test->Z(), Catch::Matchers::WithinRel(parameters.at(2).at(i), 0.001f));
            }
        }
    }

    // Use the print function to generate the screen output
    // Currently I don't know how to test this
    // Probably not very important since the functionality isn't
    // really used
    shape.printPoints(&volume);
    shape.printParam();

    // Write the parmaters to an output file
    std::string testFileName = fairroot::tests::generateTempFileName();
    std::fstream testFile{};
    testFile.open(testFileName, std::ios::out);
    shape.writePoints(&testFile, &volume);
    testFile.close();

    // Check if the two files are identical
    REQUIRE(fairroot::tests::compareFiles(fileName, testFileName));

    // Remove the temporary files
    //    std::cout << shapeName << ": " << fileName.c_str() << "\n";
    CHECK(std::remove(fileName.c_str())==0);
    CHECK(std::remove(testFileName.c_str())==0);
}

}   // namespace fairroot::tests

#endif
