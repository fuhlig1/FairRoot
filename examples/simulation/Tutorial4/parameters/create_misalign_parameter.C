/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include <RtypesCore.h>
#include <TGeoMatrix.h>
#include <TRandom.h>
#include <TFile.h>

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

TGeoHMatrix createRandomMatrix(double angleSigma, double shiftSigma) {

	double mean = 0;

	double sigmaX, sigmaY, sigmaZ;
	double shift[3];

	sigmaX = 0.;
	sigmaY = 0.;
//	sigmaZ = 0.);
	sigmaZ = gRandom->Gaus(mean, angleSigma);

	// can't move in z
	shift[0] = gRandom->Gaus(mean, shiftSigma);
	shift[1] = gRandom->Gaus(mean, shiftSigma);
	shift[2] = 0;

	TGeoHMatrix result;
	result.SetTranslation(shift);

	return result;
}

int create_misalign_parameter(Int_t nrOfDetLayers = 40) {

  std::string base_path{"/cave_1/tutorial4_0/tut4_det_"};
  std::vector<std::string> paths;
  std::map<std::string, TGeoHMatrix> matrices;            // you give this to fRun

  for (Int_t iLayer = 0; iLayer < nrOfDetLayers; ++iLayer) {
    std::string full_path = base_path + std::to_string(iLayer);
    paths.push_back(full_path);
  }

  int counter=0;
  // Create matrices and save to map
  // First and last detector anr not shifted
  for (auto &i : paths) {
    if ( counter == 0 || counter == nrOfDetLayers-1 ) {
      matrices[i] = createRandomMatrix(0., 0.);
    }
    else {
      matrices[i] = createRandomMatrix(0., 2.);
    }
    std::cout << i << std::endl;
    matrices[i].Print();
    ++counter;
  }

  // save matrices to disk
  TFile *misalignmentMatrixRootfile = new TFile("matrices.root", "RECREATE");
  if (misalignmentMatrixRootfile->IsOpen()) {
    gDirectory->WriteObject(&matrices, "MisalignMatrices");
    misalignmentMatrixRootfile->Write();
    misalignmentMatrixRootfile->Close();
  } 
  else {
   return 1;
  }

  std::ofstream myfile;
  myfile.open("shifts.txt");

  for (Int_t iLayer = 0; iLayer < nrOfDetLayers; ++iLayer) {
    TGeoHMatrix bla = matrices[paths[iLayer]];
    Double_t* shift = bla.GetTranslation();
    myfile << iLayer + 1   << "   " << shift[0] << std::endl;
    myfile << iLayer + 101 << "   " << shift[1] << std::endl;
    myfile << iLayer + 201 << "   " << shift[2] << std::endl;
/*        if (rotX)
            myfile << iLayer + 301 << "   " << RotX[iLayer] << endl;
        if (rotY)
            myfile << iLayer + 401 << "   " << RotY[iLayer] << endl;
        if (rotZ)
            myfile << iLayer + 501 << "   " << RotZ[iLayer] << endl;
*/
  }
  myfile.close();


  return 0; 
}

int main() {

 return create_misalign_parameter();

}