/********************************************************************************
 * Copyright (C) 2014-2023 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#if !defined(__CLING__) || defined(__ROOTCLING__)
#include "FairBoxGenerator.h"
#include "FairCave.h"
#include "FairConstField.h"
#include "FairMagnet.h"
#include "FairParRootFileIo.h"
#include "FairPrimaryGenerator.h"
#include "FairRootFileSink.h"
#include "FairRunSim.h"
#include "FairSystemInfo.h"
#include "FairTestDetector.h"
#endif

#include <TStopwatch.h>
#include <TString.h>
#include <TSystem.h>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;

void run_sim(Int_t nEvents = 100, TString mcEngine = "TGeant4")
{
    TStopwatch timer;
    timer.Start();
    gDebug = 0;

    // Use non default gconfig and geometry directories
    TString dir = getenv("VMCWORKDIR");
    TString tutdir = dir + "/";

    TString tut_geomdir = tutdir + "/common/geometry";
    gSystem->Setenv("GEOMPATH", tut_geomdir.Data());

    TString tut_configdir = tutdir + "/common/gconfig";
    gSystem->Setenv("CONFIG_DIR", tut_configdir.Data());

    // create Instance of Run Manager class
    FairRunSim run{};
    run.SetUseFairLinks(kTRUE);
    //  FairLinkManager::Instance()->AddIncludeType(0);
    // set the MC version used
    // ------------------------

    run.SetName(mcEngine);

    TString outFile = "data/testrun_";
    outFile = outFile + mcEngine + ".root";

    TString geoFile = "data/geofile_";
    geoFile = geoFile + mcEngine + "_full.root";

    TString parFile = "data/testparams_";
    parFile = parFile + mcEngine + ".root";

    run.SetSink(std::make_unique<FairRootFileSink>(outFile));
    run.SetGenerateRunInfo(kTRUE);   // Create FairRunInfo file

    // -----   Magnetic field   -------------------------------------------
    // Constant Field
    FairConstField *fMagField = new FairConstField();
    fMagField->SetField(0., 10., 0.);                        // values are in kG
    fMagField->SetFieldRegion(-50, 50, -50, 50, 350, 450);   // values are in cm (xmin,xmax,ymin,ymax,zmin,zmax)
    run.SetField(fMagField);
    // --------------------------------------------------------------------

    // Set Material file Name
    //-----------------------
    run.SetMaterials("media.geo");

    // Create and add detectors
    //-------------------------
    FairModule *Cave = new FairCave("CAVE");
    Cave->SetGeometryFileName("cave.geo");
    run.AddModule(Cave);

    FairModule *Magnet = new FairMagnet("MAGNET");
    Magnet->SetGeometryFileName("magnet.geo");
    run.AddModule(Magnet);

    FairDetector *Torino = new FairTestDetector("TORINO", kTRUE);
    Torino->SetGeometryFileName("torino.geo");
    run.AddModule(Torino);

    // Create and Set Event Generator
    //-------------------------------

    FairPrimaryGenerator *primGen = new FairPrimaryGenerator();
    run.SetGenerator(primGen);

    // Box Generator
    FairBoxGenerator *boxGen = new FairBoxGenerator(2212, 10);   // 13 = muon; 1 = multipl.
    boxGen->SetPRange(2., 2.);                                   // GeV/c //setPRange vs setPtRange
    boxGen->SetPhiRange(0, 360);                                 // Azimuth angle range [degree]
    boxGen->SetThetaRange(3, 10);                                // Polar angle in lab system range [degree]
    boxGen->SetCosTheta();                                       // uniform generation on all the solid angle(default)

    // boxGen->SetXYZ(0., 0.37, 0.);
    primGen->AddGenerator(boxGen);

    run.SetStoreTraj(kTRUE);

    run.Init();

    // -Trajectories Visualization (TGeoManager Only )
    // -----------------------------------------------

    // Set cuts for storing the trajectpries
    /* FairTrajFilter* trajFilter = FairTrajFilter::Instance();
     trajFilter->SetStepSizeCut(0.01); // 1 cm
     trajFilter->SetVertexCut(-2000., -2000., 4., 2000., 2000., 100.);
     trajFilter->SetMomentumCutP(10e-3); // p_lab > 10 MeV
     trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
     trajFilter->SetStorePrimaries(kTRUE);
     trajFilter->SetStoreSecondaries(kTRUE);
   */

    // Fill the Parameter containers for this run
    //-------------------------------------------

    FairRuntimeDb* rtdb = run.GetRuntimeDb();
    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo *output = new FairParRootFileIo(kParameterMerged);
    output->open(parFile);
    rtdb->setOutput(output);

    rtdb->saveOutput();
    rtdb->print();

    // Transport nEvents
    // -----------------

    //  Int_t nEvents = 1;
    run.Run(nEvents);

    run.CreateGeometryFile(geoFile);

    // -----   Finish   -------------------------------------------------------
    run.ls();

    cout << endl << endl;

    // Extract the maximal used memory an add is as Dart measurement
    // This line is filtered by CTest and the value send to CDash
    FairSystemInfo sysInfo;
    Float_t maxMemory = sysInfo.GetMaxMemory();
    cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
    cout << maxMemory;
    cout << "</DartMeasurement>" << endl;

    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();

    Float_t cpuUsage = ctime / rtime;
    cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
    cout << cpuUsage;
    cout << "</DartMeasurement>" << endl;

    cout << endl << endl;
    cout << "Output file is " << outFile << endl;
    cout << "Parameter file is " << parFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << endl << endl;
    cout << "Macro finished successfully." << endl;

    // ------------------------------------------------------------------------
}
