/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                        FairTask source file                   -----
// -----          Created 12/01/04  by M. Al-Turany / D. Bertini       -----
// -------------------------------------------------------------------------

#include "FairTask.h"

#include "FairLogger.h"                 // for FairLogger, MESSAGE_ORIGIN

#include "TCollection.h"                // for TIter
#include "TList.h"                      // for TList
#include "TObject.h"                    // for TObject
#include "TROOT.h"

#include <iostream>                     // for cout, endl
#include <iomanip>                      //
#include <algorithm>                    // for begin, end
#include <numeric>                      // for accumulate

using std::cout;
using std::endl;

// -----   Default constructor   -------------------------------------------
FairTask::FairTask()
  : TTask(),
    fVerbose(0),
    fInputPersistance(-1),
    fLogger(FairLogger::GetLogger()),
    fStopwatch(),
    fCpuTime(),
    fRealTime()
{
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
FairTask::FairTask(const char* name, Int_t iVerbose)
  : TTask(name, "FairTask"),
    fVerbose(iVerbose),
    fInputPersistance(-1),
    fLogger(FairLogger::GetLogger()),
    fStopwatch(),
    fCpuTime(),
    fRealTime()
{

}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FairTask::~FairTask() { }
// -------------------------------------------------------------------------



// -----   Public method InitTask   ----------------------------------------
void FairTask::InitTask()
{
  if ( ! fActive ) { return; }
  InitStatus tStat = Init();
  if ( tStat == kFATAL ) {
    fLogger->Fatal(MESSAGE_ORIGIN,"Initialization of Task %s failed fatally", fName.Data());
  }
  if ( tStat == kERROR ) { fActive = kFALSE; }
  InitTasks();
}
// -------------------------------------------------------------------------



// -----   Public method ReInitTask   --------------------------------------
void FairTask::ReInitTask()
{
  if ( ! fActive ) { return; }
  InitStatus tStat = ReInit();
  if ( tStat == kFATAL ) {
    fLogger->Fatal(MESSAGE_ORIGIN,"Reinitialization of Task %s failed fatally", fName.Data());
  }
  if ( tStat == kERROR ) { fActive = kFALSE; }
  ReInitTasks();
}
// -------------------------------------------------------------------------



// -----   Public method SetParTask   --------------------------------------
void FairTask::SetParTask()
{
  if ( ! fActive ) { return; }
  SetParContainers();
  SetParTasks();
}
// -------------------------------------------------------------------------

// -----    Public method FinishEvent -------------------------------------
void FairTask::FinishEvent()
{
  if ( ! fActive ) { return; }
  FinishEvents();
  //  FinishTasks();
}

// -----    Public method FinishTask   -------------------------------------
void FairTask::FinishTask()
{
  if ( ! fActive ) { return; }
  Finish();
  FinishTasks();
}
// -------------------------------------------------------------------------



// -----   Public method SetVerbose   --------------------------------------
void FairTask::SetVerbose(Int_t iVerbose)
{
  fVerbose = iVerbose;
  TIter next(GetListOfTasks());
  FairTask* task;
  while((task=dynamic_cast <FairTask*> (next()))) { task->SetVerbose(iVerbose); }
}
// -------------------------------------------------------------------------

void FairTask::ExecuteTasks(Option_t *option)
{
  // Execute all the subtasks of a task.
  // This is copied from TTask to add functionality to measure runtime
  // of tasks
  TIter next(fTasks);
  FairTask *task;
  while((task=(FairTask*)next())) {
    if (fgBreakPoint) return;
    if (!task->IsActive()) continue;
    if (task->fHasExecuted) {
      task->ExecuteTasks(option);
      continue;
    }
    if (task->fBreakin == 1) {
      printf("Break at entry of task: %s\n",task->GetName());
      fgBreakPoint = this;
      task->fBreakin++;
      return;
    }

    if (gDebug > 1) {
      TROOT::IndentLevel();
      cout<<"Execute task:"<<task->GetName()<<" : "<<task->GetTitle()<<endl;
      TROOT::IncreaseDirLevel();
    }

    fStopwatch.Start(kTRUE); // reset the stop watch
    task->Exec(option);
    fStopwatch.Stop();

    task->AddCpuTime(fStopwatch.CpuTime());
    task->AddRealTime(fStopwatch.RealTime());

    task->fHasExecuted = kTRUE;
    task->ExecuteTasks(option);
    if (gDebug > 1) TROOT::DecreaseDirLevel();
    if (task->fBreakout == 1) {
      printf("Break at exit of task: %s\n",task->GetName());
      fgBreakPoint = this;
      task->fBreakout++;
      return;
    }
  }
}

// -----   Protected method InitTasks   ------------------------------------
void FairTask::InitTasks()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->InitTask(); }
}
// -------------------------------------------------------------------------



// -----   Protected method ReInitTasks   ----------------------------------
void FairTask::ReInitTasks()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->ReInitTask(); }
}
// -------------------------------------------------------------------------



// -----   Protected method SetParTasks   ----------------------------------
void FairTask::SetParTasks()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->SetParTask(); }
}
// -------------------------------------------------------------------------



// -----   Protected method FinishTasks   ----------------------------------
void FairTask::FinishTasks()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->FinishTask(); }
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->Finish(); }
}
// -------------------------------------------------------------------------

// -----   Protected method FinishEvents   ----------------------------------
void FairTask::FinishEvents()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->FinishEvent(); }
}
// -------------------------------------------------------------------------

void FairTask::PrintStatistics()
{
  TIter next(GetListOfTasks());
  FairTask* task;
  Ssiz_t length = 9; // length of taskname string
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) {
    TString name = task->GetName();
    if (name.Length() > length) length = name.Length();
  }
  LOG(INFO) << std::setw(length) << "Taskname" << " :       CPU Time       :       Real Time " << FairLogger::endl;
  LOG(INFO) << std::setw(length) << " " << " :  Min  : Mean :  Max  :  Min  : Mean :  Max   " << FairLogger::endl;
  next = GetListOfTasks();
  while( ( task=dynamic_cast<FairTask*>(next()) ) ) { task->PrintStatistic(); }
}
// -------------------------------------------------------------------------

void FairTask::PrintStatistic()
{
  if (fRealTime.size()  > 0) {
    Double_t max_cpu = *std::max_element(fCpuTime.begin(), fCpuTime.end());
    Double_t min_cpu = *std::min_element(fCpuTime.begin(), fCpuTime.end());
    Double_t max_real = *std::max_element(fRealTime.begin(), fRealTime.end());
    Double_t min_real = *std::min_element(fRealTime.begin(), fRealTime.end());
    Double_t sum_cpu = std::accumulate(fCpuTime.begin(), fCpuTime.end(), 0.0);
    Double_t mean_cpu =  sum_cpu / fCpuTime.size();
    Double_t sum_real = std::accumulate(fRealTime.begin(), fRealTime.end(), 0.0);
    Double_t mean_real =  sum_real / fRealTime.size();
    LOG(INFO) << GetName() << " : " << min_cpu << " : " << mean_cpu
              << " : " << max_cpu << " : " << min_real << " : "
              << mean_real << " : " << max_real << FairLogger::endl;
  }
}

ClassImp(FairTask)
