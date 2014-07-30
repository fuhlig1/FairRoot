/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                      FairTask header file                      -----
// -----          Created 12/01/04  by M. Al-Turany / D. Bertini       -----
// -------------------------------------------------------------------------


/** FairTask
 * @author M. Al-Turany, Denis Bertini
 * @since 12.01.04
 **
 ** Base class for tasks in the cbmroot framework.
 ** Derived classes should implement the Exec method.
 **/

#ifndef FAIRTASK_H
#define FAIRTASK_H

#include "TTask.h"                      // for TTask

#include "FairRootManager.h"            // for FairRootManager

#include "Rtypes.h"                     // for Int_t, FairTask::Class, etc
#include "TString.h"                    // for TString
#include "TStopwatch.h"

#include <vector>

class FairLogger;

enum InitStatus {kSUCCESS, kERROR, kFATAL};

class FairTask : public TTask
{

  public:

    /** Default constructor **/
    FairTask();


    /** Standard constructor
    *@param name        Name of task
    *@param iVerbose    Verbosity level
    **/
    FairTask(const char* name, Int_t iVerbose = 1);


    /** Destructor **/
    virtual ~FairTask();


    /** Initialisation at begin of run. For this task and all of the
        subtasks. **/
    void InitTask();


    /** Reinitialisation. For this task and all of the subtasks. **/
    void ReInitTask();


    /** Execute the task and all its subtasks
        This is a copy from TTask which measures the time for
        the Exec function of the task and stores the information.
    **/
    void ExecuteTasks(Option_t* option = "0");

    /** Loop over all tasks and print the measured time **/
    void PrintStatistics();

    /** Set parameters. For this task and all of the subtasks. **/
    void SetParTask();

    /** Action at end of run. For this task and all of the subtasks. **/
    virtual void FinishTask();

    /** Action at end of event. For this task and all of the subtasks. **/
    virtual void FinishEvent();

    /** Set verbosity level. For this task and all of the subtasks. **/
    void SetVerbose(Int_t iVerbose);

    void SetInputPersistance(Bool_t val) {fInputPersistance = val;}

    void CheckInputPersistance(TString branchName) {
      FairRootManager* ioman = FairRootManager::Instance();
      fInputPersistance = ioman->CheckBranch(branchName);
    }


  protected:

    Int_t        fVerbose;  //  Verbosity level
    Int_t        fInputPersistance; ///< Indicates if input branch is persistant
    FairLogger*  fLogger; //!

    /** Intialisation at begin of run. To be implemented in the derived class.
    *@value  Success   If not kSUCCESS, task will be set inactive.
    **/
    virtual InitStatus Init() { return kSUCCESS; };


    /** Reinitialisation. To be implemented in the derived class.
    *@value  Success   If not kSUCCESS, task will be set inactive.
    **/
    virtual InitStatus ReInit() { return kSUCCESS; };


    /** Intialise parameter containers.
        To be implemented in the derived class.
    **/
    virtual void SetParContainers() { };


    /** Action after each event. To be implemented in the derived class **/
    virtual void Finish() { };

    //  /** Action after each event. To be implemented in the derived class **/
    //  virtual void FinishTask() { };


    /** Recursive intialisation of subtasks at begin of run **/
    void InitTasks();


    /** Recursive reinitialisation of subtasks **/
    void ReInitTasks();


    /** Recursive parameter initialisation for subtasks **/
    void SetParTasks();

    /** Recursive finish of subtasks **/
    void FinishTasks();

    /** Recursive FinishEvent of subtasks **/
    void FinishEvents();

    void AddCpuTime(Double_t time) { fCpuTime.push_back(time);}
    void AddRealTime(Double_t time) { fRealTime.push_back(time);}

  private:
    TStopwatch fStopwatch;
    std::vector<Double_t> fCpuTime;
    std::vector<Double_t> fRealTime;

    /** Print the measured time for a task**/
    void PrintStatistic();

    FairTask(const FairTask&);
    FairTask& operator=(const FairTask&);

    ClassDef(FairTask,3);

};


#endif
