#include "MyPrimaryGeneratorMessenger.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADouble.hh"

MyPrimaryGeneratorMessenger::MyPrimaryGeneratorMessenger(MyPrimaryGeneratorAction *gene)
  : G4UImessenger(),
    fPrimaryGenerator(gene)
{
  fSiDirectory = new G4UIdirectory("/Si/");
  fSiDirectory->SetGuidance("UI commands specific to this example.");

  fGeneDirectory = new G4UIdirectory("/Si/gene/");
  fGeneDirectory->SetGuidance("Primary Generator control");


  fEtaCmd = new G4UIcmdWithADouble("/Si/gene/eta",this);
  fEtaCmd->SetGuidance("Define eta");
  fEtaCmd->SetParameterName("eta",false);
  fEtaCmd->AvailableForStates(G4State_Idle);

  fFaiCmd = new G4UIcmdWithADouble("/Si/gene/fai",this);
  fFaiCmd->SetGuidance("Define fai");
  fFaiCmd->SetParameterName("fai",false);
  fFaiCmd->AvailableForStates(G4State_Idle);

  fPixelsizeCmd = new G4UIcmdWith3VectorAndUnit("/Si/gene/pixelsize",this);
  fPixelsizeCmd->SetGuidance("Define pixelsize");
  fPixelsizeCmd->SetParameterName("pixelx","pixely","pixelz",false);
  fPixelsizeCmd->SetUnitCategory("Length");
  fPixelsizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

MyPrimaryGeneratorMessenger::~MyPrimaryGeneratorMessenger(){
  delete fEtaCmd;
  delete fFaiCmd;
  delete fPixelsizeCmd;
  delete fGeneDirectory;
  delete fSiDirectory;
}

void MyPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,G4String newValue){
  if(command == fEtaCmd){
    fPrimaryGenerator->SetEta(fEtaCmd->GetNewDoubleValue(newValue));
  }
 if(command == fFaiCmd){
    fPrimaryGenerator->SetFai(fFaiCmd->GetNewDoubleValue(newValue));
  }
  if(command == fPixelsizeCmd){
    fPrimaryGenerator->SetPixelsize(fPixelsizeCmd->GetNew3VectorValue(newValue));
  }
}
