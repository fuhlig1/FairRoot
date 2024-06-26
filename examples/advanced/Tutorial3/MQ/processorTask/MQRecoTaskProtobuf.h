/********************************************************************************
 * Copyright (C) 2014-2023 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// Implementation of MQRecoTask::Exec() with Google Protocol Buffers transport data format

#ifndef FAIR_TESTDETECTOR_PROCESSORTASK_PROTOBUF_H
#define FAIR_TESTDETECTOR_PROCESSORTASK_PROTOBUF_H

#ifdef PROTOBUF

#include "Payload.h"
#include "Payload.pb.h"

template<>
void MQRecoTask<TestDetectorProtobuf>::Exec(Option_t* opt)
{
    fRecoTask.fDigiArray->Clear();

    TestDetectorProto::DigiPayload dp;
    dp.ParseFromArray(fPayload->GetData(), fPayload->GetSize());

    int numEntries = dp.digi_size();

    for (int i = 0; i < numEntries; ++i) {
        const TestDetectorProto::Digi& digi = dp.digi(i);
        new ((*fRecoTask.fDigiArray)[i]) FairTestDetectorDigi(digi.fx(), digi.fy(), digi.fz(), digi.ftimestamp());
        static_cast<FairTestDetectorDigi*>(((*fRecoTask.fDigiArray)[i]))->SetTimeStampError(digi.ftimestamperror());
    }

    if (!fRecoTask.fDigiArray) {
        LOG(error) << "MQRecoTask::Exec(): No Point array!";
    }

    fRecoTask.Exec(opt);

    TestDetectorProto::HitPayload hp;

    for (int i = 0; i < numEntries; ++i) {
        FairTestDetectorHit* hit = static_cast<FairTestDetectorHit*>(fRecoTask.fHitArray->At(i));
        if (!hit) {
            continue;
        }
        TestDetectorProto::Hit* h = hp.add_hit();
        h->set_detid(hit->GetDetectorID());
        h->set_mcindex(hit->GetRefIndex());
        h->set_posx(hit->GetX());
        h->set_posy(hit->GetY());
        h->set_posz(hit->GetZ());
        h->set_dposx(hit->GetDx());
        h->set_dposy(hit->GetDy());
        h->set_dposz(hit->GetDz());
    }

    std::string* str = new std::string();
    hp.SerializeToString(str);

    fPayload->Rebuild(
        const_cast<char*>(str->c_str()),
        str->length(),
        [](void* /* data */, void* obj) { delete static_cast<std::string*>(obj); },
        str);
}

#endif   // PROTOBUF

#endif   // FAIR_TESTDETECTOR_PROCESSORTASK_PROTOBUF_H
