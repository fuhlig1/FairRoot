/********************************************************************************
 *    Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
void run_digi(TString mcEngine = "TGeant3", Bool_t AlignDone = true)
{
    // ----  Load libraries   -------------------------------------------------
    FairLogger* logger = FairLogger::GetLogger();
    //  logger->SetLogFileName("MyLog.log");
    logger->SetLogToScreen(kTRUE);
    //  logger->SetLogToFile(kTRUE);
    //  logger->SetLogVerbosityLevel("HIGH");
    //  logger->SetLogFileLevel("DEBUG4");
    logger->SetLogScreenLevel("INFO");

    // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
    Int_t iVerbose = 0;   // just forget about it, for the moment

    TString dir = getenv("VMCWORKDIR");

    TString Align = "";
    if (AlignDone) {
        Align = "align_";
    }
    TString InDir = "./data/";
    // Input file (MC Events)
    TString inFile = "testrun_";
    inFile = InDir + inFile + Align + mcEngine + ".root";

    // Output file name
    TString outFile = "testdigi_";
    outFile = InDir + outFile + Align + mcEngine + ".root";

    // Parameter file
    TString parFile = "testparams_";
    parFile = InDir + parFile + Align + mcEngine + ".root";


    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;

    // -----   Reconstruction run   -------------------------------------------
    FairRunAna* fRun = new FairRunAna();
    FairFileSource* fFileSource = new FairFileSource(inFile);
    fRun->SetSource(fFileSource);
    fRun->SetSink(new FairRootFileSink(outFile));

    FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
    FairParRootFileIo* parInput1 = new FairParRootFileIo();
    parInput1->open(parFile.Data());
    rtdb->setFirstInput(parInput1);
    rtdb->setOutput(parInput1);
    rtdb->saveOutput();


   if (AlignDone) {
      // read matrices from disk
      std::map<std::string, TGeoHMatrix>* matrices{nullptr};

      TString matrix_filename{"matrices.root"};
      matrix_filename = dir + "/simulation/Tutorial4/parameters/" + matrix_filename;
      LOG(info) << "Filename: " << matrix_filename.Data();

      TFile *misalignmentMatrixRootfile = new TFile(matrix_filename.Data(), "READ");
      if (misalignmentMatrixRootfile->IsOpen()) {
        gDirectory->GetObject("MisalignMatrices", matrices);
        misalignmentMatrixRootfile->Close();
      } else {
       LOG(error) << "Could not open file " << matrix_filename << "\n Exiting";
       exit(1);
      }
      if (matrices) {
        fRun->AddAlignmentMatrices(*matrices);
      } else {
       LOG(error) << "Alignment required but no matrices found." <<  "\n Exiting";
       exit(1);
      }

    }

    // -----   TorinoDetector hit  producers   ---------------------------------
    FairTutorialDet4Digitizer* digitizer = new FairTutorialDet4Digitizer();
    fRun->AddTask(digitizer);

    fRun->Init();

    timer.Start();
    fRun->Run();

    // -----   Finish   -------------------------------------------------------

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
