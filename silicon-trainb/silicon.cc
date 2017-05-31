//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: exampleN01.cc,v 1.6 2006/06/29 17:47:10 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - exampleN01
// --------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "QGSP_BIC_HP.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "RecordHits.hh"
#include "RecordDigits.hh"
#include "MyRunAction.hh"
#include "MyEventAction.hh"
#include "PhysicsList.hh"
//#include "ExN02PhysicsList.hh"

#include "G4StepLimiterPhysics.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv)
{
  // Construct the default run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // set mandatory initialization classes
  
  G4VUserDetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  
   G4VModularPhysicsList* physics = new PhysicsList();
  runManager->SetUserInitialization(physics);

  // set mandatory user action class  
  RecordHits* hitrec = new RecordHits();
  RecordDigits* digrec=new RecordDigits();

  G4VUserPrimaryGeneratorAction* gen_action = new MyPrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);
  MyRunAction* run_action = new MyRunAction(hitrec,digrec);
  runManager->SetUserAction(run_action);
  
  MyEventAction* event_action = new MyEventAction(hitrec,digrec);
  runManager->SetUserAction(event_action);

  runManager->Initialize();

#ifdef G4VIS_USE
    // Initialize visualization
    //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Get the pointer to the UI manager and set verbosities
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();



  if (argc!=1)   // batch mode
  {
	  G4String command = "/control/execute ";
	  G4String fileName = argv[1];
	  UImanager->ApplyCommand(command+fileName);
  }
  else
  
  {  // interactive mode : define UI session
  
#ifdef G4UI_USE
	  G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
	  UImanager->ApplyCommand("/control/execute vis.mac");
#endif
  
  ui->SessionStart();
  delete ui;
#endif
  }
  


  // Job termination
  //
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  //

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  delete hitrec;
  delete digrec;

  return 0;
}


