/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "NewTask.h"

// ---- Default constructor -------------------------------------------
NewTask::NewTask()
  :FairTask("NewTask")
{
  LOG(DEBUG) << "Default Constructor of NewTask" << FairLogger::endl;
}

// ---- Destructor ----------------------------------------------------
NewTask::~NewTask()
{
  LOG(DEBUG) << "Destructor of NewTask" << FairLogger::endl;
}

// ----  Initialisation  ----------------------------------------------
void NewTask::SetParContainers()
{
  LOG(DEBUG) << "SetParContainers of NewTask" << FairLogger::endl;
  // Load all necessary parameter containers from the runtime data base
  /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <NewTaskDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus NewTask::Init()
{
  LOG(DEBUG) << "Initilization of NewTask" << FairLogger::endl;

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Get a pointer to the previous already existing data level
  /*
    <InputDataLevel> = (TClonesArray*) ioman->GetObject("InputDataLevelName");
    if ( ! <InputLevel> ) {
    LOG(ERROR) << "No InputDataLevelName array!\n NewTask will be inactive" << FairLogger::endl;
    return kERROR;
    }
  */

  // Create the TClonesArray for the output data and register
  // it in the IO manager
  /*
    <OutputDataLevel> = new TClonesArray("OutputDataLevelName", 100);
    ioman->Register("OutputDataLevelName","OutputDataLevelName",<OutputDataLevel>,kTRUE);
  */

  // Do whatever else is needed at the initilization stage
  // Create histograms to be filled
  // initialize variables

  return kSUCCESS;

}

// ---- ReInit  -------------------------------------------------------
InitStatus NewTask::ReInit()
{
  LOG(DEBUG) << "Reinitilization of NewTask" << FairLogger::endl;
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void NewTask::Exec(Option_t* option)
{
  LOG(DEBUG) << "Exec of NewTask" << FairLogger::endl;
}

// ---- Finish --------------------------------------------------------
void NewTask::Finish()
{
  LOG(DEBUG) << "Finish of NewTask" << FairLogger::endl;
}

ClassImp(NewTask)
