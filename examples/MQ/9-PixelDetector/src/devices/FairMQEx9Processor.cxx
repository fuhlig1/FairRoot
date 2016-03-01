/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "FairMQEx9Processor.h"


FairMQEx9Processor::FairMQEx9Processor() : 	fOutput(nullptr),
										    fRootParFileName(),
                                            fAsciiParFileName(),
										    fInputClassName(),
                                            fParamName(),
                                            fGeoParamName("FairGeoParSet"),
                                            fCurrentRunId(-1),
										    fSerializer(),
										    fDeSerializer(),
                                            fDigiPar(),
                                            fGeoPar(),
                                            //fParamDeserializer(),
                                            fHitFinder(new PixelFindHits())
{

}

FairMQEx9Processor::~FairMQEx9Processor()
{
    if(fHitFinder)
    {
        delete fHitFinder;
        fHitFinder=nullptr;
    }

}


void FairMQEx9Processor::Init()
{
	fDeSerializer.InitContainer(fInputClassName);
    //fParamDeserializer.InitContainer("PixelDigiPar");
    fHitFinder->InitMQ(fRootParFileName,fAsciiParFileName);
}

void FairMQEx9Processor::Run()
{
    int receivedMsgs = 0;
    int sentMsgs = 0;

    const FairMQChannel& inputChannel = fChannels["data-in"].at(0);
    const FairMQChannel& outputChannel = fChannels["data-out"].at(0);

    while (CheckCurrentState(RUNNING))
    {
        std::unique_ptr<FairMQMessage> msg(fTransportFactory->CreateMessage());

        if (inputChannel.Receive(msg) > 0)
        {
            receivedMsgs++;
            // Deserialize data into TClonesArray
            TClonesArray* input = fDeSerializer.DeserializeMsg(msg.get());


            

            // Execute hit finder task
            fOutput=fHitFinder->ExecMQ(input);
            // if output not empty serialize and send
            if(!fOutput->IsEmpty())
            {
                fSerializer.SetMessage(msg.get());
                outputChannel.Send(fSerializer.SerializeMsg(fOutput));
                sentMsgs++;
            }
        }
    }

    MQLOG(INFO) << "Received " << receivedMsgs << " and sent " << sentMsgs << " messages!";
}




void FairMQEx9Processor::CustomCleanup(void *data, void *hint)
{
    delete (std::string*)hint;
}





void FairMQEx9Processor::UpdateParameters()
{
    if(fDigiPar)
    {
        delete fDigiPar;
        fDigiPar=nullptr;
    }

    if(fGeoPar)
    {
        delete fGeoPar;
        fGeoPar=nullptr;
    }

    fDigiPar = new PixelDigiPar();
    fGeoPar = new FairGeoParSet();

    UpdateParameter<PixelDigiPar>(fParamName,fDigiPar);
    UpdateParameter<FairGeoParSet>(fGeoParamName,fGeoPar);
    fHitFinder->UpdateParameter(fDigiPar, fGeoPar);

}



void FairMQEx9Processor::SetProperty(const int key, const std::string& value)
{
    switch (key)
    {
        case RootParam :
            fRootParFileName = value;
            break;

        case AsciiParam :
            fAsciiParFileName = value;
            break;

        case InputClassName :
            fInputClassName = value;
            break;

        case ParamName :
            fParamName = value;
            break;
        
        default:
            FairMQDevice::SetProperty(key, value);
            break;
    }
}



std::string FairMQEx9Processor::GetProperty(const int key, const std::string& default_)
{
    switch (key)
    {
        case RootParam :
            return fRootParFileName;

        case AsciiParam :
            return fAsciiParFileName;

        case InputClassName :
            return fInputClassName;

        case ParamName :
            return fParamName;

        default:
            return FairMQDevice::GetProperty(key, default_);
    }
}


void FairMQEx9Processor::SetProperty(const int key, const int value)
{
    FairMQDevice::SetProperty(key, value);
}

int FairMQEx9Processor::GetProperty(const int key, const int value)
{
    return FairMQDevice::GetProperty(key, value);
}







