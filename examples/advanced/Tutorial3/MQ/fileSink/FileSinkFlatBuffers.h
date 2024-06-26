/********************************************************************************
 * Copyright (C) 2014-2024 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// Implementation of FileSink::Run() with Google FlatBuffers transport data format

#ifndef FAIR_TESTDETECTOR_FILESINK_FLATBUFFERS_H
#define FAIR_TESTDETECTOR_FILESINK_FLATBUFFERS_H

#ifdef FLATBUFFERS

#include "Payload.h"
#include "PayloadHit_generated.h"
#include "flatbuffers/flatbuffers.h"

template<>
void FileSink<TestDetectorFlatBuffers>::InitTask()
{
    OnData(fInChannelName, [this](fair::mq::MessagePtr& msg, int /*index*/) {
        ++fReceivedMsgs;
        fOutput->Delete();

        auto hitPayload = TestDetectorFlat::GetHitPayload(msg->GetData());
        auto hits = hitPayload->hits();

        // int numEntries = hits->size();

        for (auto it = hits->begin(); it != hits->end(); ++it) {
            TVector3 pos((*it)->x(), (*it)->y(), (*it)->z());
            TVector3 dpos((*it)->dx(), (*it)->dy(), (*it)->dz());
            new ((*fOutput)[it - hits->begin()]) FairTestDetectorHit((*it)->det_id(), (*it)->mc_index(), pos, dpos);
            // LOG(warn) << " " << (*it)->detID() << " " << (*it)->x() << " " << (*it)->y() << " " << (*it)->z() << " "
            // << (*it)->dx() << " " << (*it)->dy() << " " << (*it)->dz();
        }

        if (fOutput->IsEmpty()) {
            LOG(error) << "FileSink::Run(): No Output array!";
        }

        fTree.Fill();

        if (fMaxMsgs != 0) {
            if (fReceivedMsgs == fMaxMsgs) {
                return false;
            }
        }

        return true;
    });
}

#endif   // FLATBUFFERS

#endif   // FAIR_TESTDETECTOR_FILESINK_FLATBUFFERS_H
