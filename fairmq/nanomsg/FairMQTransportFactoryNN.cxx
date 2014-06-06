/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENCE"                       *
 ********************************************************************************/
/**
 * FairMQTransportFactoryNN.cxx
 *
 * @since 2014-01-20
 * @author: A. Rybalchenko
 */

#include "FairMQTransportFactoryNN.h"

FairMQTransportFactoryNN::FairMQTransportFactoryNN()
{
    LOG(INFO) << "Using nanonsg library";
}

FairMQMessage* FairMQTransportFactoryNN::CreateMessage()
{
    return new FairMQMessageNN();
}

FairMQMessage* FairMQTransportFactoryNN::CreateMessage(size_t size)
{
    return new FairMQMessageNN(size);
}

FairMQMessage* FairMQTransportFactoryNN::CreateMessage(void* data, size_t size)
{
    return new FairMQMessageNN(data, size);
}

FairMQSocket* FairMQTransportFactoryNN::CreateSocket(const string& type, int num, int numIoThreads)
{
    return new FairMQSocketNN(type, num, numIoThreads);
}

FairMQPoller* FairMQTransportFactoryNN::CreatePoller(const vector<FairMQSocket*>& inputs)
{
    return new FairMQPollerNN(inputs);
}
