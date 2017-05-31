#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
DetectorMessenger::DetectorMessenger(DetectorConstruction *det) 
  : G4UImessenger(),
    fDetectorConstruction(det)
{

  fSiDirectory = new G4UIdirectory("/Si/");
  fSiDirectory->SetGuidance("UI commands specific to this example.");

  fDetDirectory = new G4UIdirectory("/Si/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fPixelsizeCmd = new G4UIcmdWith3VectorAndUnit("/Si/det/pixelsize",this);
  fPixelsizeCmd->SetGuidance("Define pixelsize");
  fPixelsizeCmd->SetParameterName("pixelx","pixely","pixelz",false);
  fPixelsizeCmd->SetUnitCategory("Length");
  fPixelsizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // fPixelxCmd = new G4UIcmdWithADoubleAndUnit("/Si/det/pixelx",this);
  // fPixelxCmd->SetGuidance("Define pixelx");
  // fPixelxCmd->SetParameterName("pixelx",false);
  // fPixelxCmd->SetUnitCategory("Length");
  // fPixelxCmd->AvailableForStates(G4State_Idle);

  // fPixelyCmd = new G4UIcmdWithADoubleAndUnit("/Si/det/pixely",this);
  // fPixelyCmd->SetGuidance("Define pixely");
  // fPixelyCmd->SetParameterName("pixely",false);
  // fPixelyCmd->SetUnitCategory("Length");
  // fPixelyCmd->AvailableForStates(G4State_Idle);

  // fPixelzCmd = new G4UIcmdWithADoubleAndUnit("/Si/det/pixelz",this);
  // fPixelzCmd->SetGuidance("Define pixelz");
  // fPixelzCmd->SetParameterName("pixelz",false);
  // fPixelzCmd->SetUnitCategory("Length");
  // fPixelzCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fThreCmd = new G4UIcmdWithADouble("/Si/det/threshold",this);
  fThreCmd->SetGuidance("Define threshold");
  fThreCmd->SetParameterName("threshold",false);
  // fThreCmd->SetUnitCategory("Length");
  fThreCmd->AvailableForStates(G4State_Idle);

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/Si/det/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_Idle);

  fUpdateCmd = new G4UIcmdWithoutParameter("/Si/det/update",this);
  fUpdateCmd->SetGuidance("Update detector geometry.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
  fUpdateCmd->AvailableForStates(G4State_Idle);
}


DetectorMessenger::~DetectorMessenger()
{
  delete fPixelsizeCmd;
  delete fThreCmd;
//  delete fPixelxCmd;
//  delete fPixelyCmd;
//  delete fPixelzCmd;
  delete fStepMaxCmd;
  delete fUpdateCmd;
  delete fSiDirectory;
  delete fDetDirectory;
}


void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
 //  if( command == fPixelxCmd ) {
 //    fDetectorConstruction
 //      ->SetPixelx(fPixelxCmd->GetNewDoubleValue(newValue));
 //  }   
 // if( command == fPixelyCmd ) {
 //    fDetectorConstruction
 //      ->SetPixely(fPixelyCmd->GetNewDoubleValue(newValue));
 //  }   
 // if( command == fPixelzCmd ) {
 //    fDetectorConstruction
 //      ->SetPixelz(fPixelzCmd->GetNewDoubleValue(newValue));
 //  }   

  if( command == fPixelsizeCmd ){
    fDetectorConstruction 
      ->SetDetectorPixelsize(fPixelsizeCmd->GetNew3VectorValue(newValue));
  }
  if( command == fThreCmd ){
    fDetectorConstruction 
      ->SetDigitizerThreshold(fThreCmd->GetNewDoubleValue(newValue));
  }
  if( command == fStepMaxCmd ) {
    fDetectorConstruction
      ->SetMaxStepSize(fStepMaxCmd->GetNewDoubleValue(newValue));
  }  
   if( command == fUpdateCmd )
   { fDetectorConstruction->UpdateGeometry(); }
    
}
