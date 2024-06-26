/********************************************************************************
 * Copyright (C) 2017-2023 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/**
 * FairMQTransportDevice.h
 *
 * @since 2018-02-01
 * @author R. Karabowicz
 */

#ifndef FAIRMQTRANSPORTDEVICE_H_
#define FAIRMQTRANSPORTDEVICE_H_

#include "FairMQRunDevice.h"
#include "FairRunSim.h"

#include <Rtypes.h>
#include <TString.h>
#include <cstdint>
#include <memory>
#include <string>

class FairMCSplitEventHeader;
class FairField;
class FairParIo;
class TObjArray;
class FairMCApplication;
class FairGenericStack;
class TVirtualMC;

class FairMQTransportDevice : public FairMQRunDevice
{
  public:
    FairMQTransportDevice();
    ~FairMQTransportDevice() override;

    // ------ FairRunSim setters ------
    void SetNofEvents(int64_t nofev) { fNofEvents = nofev; };
    void SetTransportName(const std::string& str) { fTransportName = str; };
    void SetMaterials(const std::string& str) { fMaterialsFile = str; };
    void SetMagneticField(FairField* field) { fMagneticField = field; };
    void SetDetectorArray(TObjArray* array) { fDetectorArray = array; };
    void SetStoreTraj(bool flag = true) { fStoreTrajFlag = flag; };
    void SetTaskArray(TObjArray* array) { fTaskArray = array; };
    void SetFirstParameter(FairParIo* par) { fFirstParameter = par; };
    void SetSecondParameter(FairParIo* par) { fSecondParameter = par; };
    void SetUserConfig(const TString& Config) { fUserConfig = Config; }
    void SetUserCuts(const TString& Cuts) { fUserCuts = Cuts; }
    // ------ ---------- -------- ------

    void InitializeRun();

    void SetParamUpdateChannelName(TString tString) { fUpdateChannelName = tString; }

    void RunInPullMode(bool tb = true) { fRunConditional = !tb; };
    void RunInReqMode(bool tb = true) { fRunConditional = tb; };

  protected:
    bool TransportData(fair::mq::Parts&, int);
    //  bool TransportData(fair::mq::MessagePtr&, int);
    void Init() override;
    void InitTask() override;
    void ResetTask() override;
    void PreRun() override;
    void PostRun() override;
    bool ConditionalRun() override;

  private:
    UInt_t fRunId;
    UInt_t fTransportDeviceId;
    std::string fGeneratorChannelName;
    std::string fUpdateChannelName;

    bool fRunConditional;   // if true run ConditionalRun, if false run TransportData
    bool fRunInitialized;   // false, set to true after initialization in the run stage (!)

    TVirtualMC* fVMC;
    FairGenericStack* fStack;
    FairMCApplication* fMCApplication;
    std::unique_ptr<FairRunSim> fRunSim;
    // ------ FairRunSim settings ------
    int64_t fNofEvents;
    std::string fTransportName;
    std::string fMaterialsFile;
    FairField* fMagneticField;
    TObjArray* fDetectorArray;
    bool fStoreTrajFlag;
    TObjArray* fTaskArray;
    FairParIo* fFirstParameter;    // first (prefered) input for parameters
    FairParIo* fSecondParameter;   // second input (used if not found in first input)
    TString fUserConfig;           //!                  /** Macro for geant configuration*/
    TString fUserCuts;             //!                  /** Macro for geant cuts*/
    // ------ ---------- -------- ------

    FairMCSplitEventHeader* fMCSplitEventHeader;

    void UpdateParameterServer();

    FairMQTransportDevice(const FairMQTransportDevice&);
    FairMQTransportDevice& operator=(const FairMQTransportDevice&);
};

#endif /* FAIRMQTRANSPORTDEVICE_H_ */
