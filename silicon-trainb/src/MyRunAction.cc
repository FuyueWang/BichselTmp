//nTPCRunAction.cc

#include "MyRunAction.hh"
#include "RecordDigits.hh"
#include "RecordHits.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
MyRunAction::MyRunAction(RecordHits* hitrec,RecordDigits* digirec)
  :hitRecorder(hitrec),
   digiRecorder(digirec)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
MyRunAction::~MyRunAction()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyRunAction::BeginOfRunAction(const G4Run* aRun)
{
  // recorders->RecordBeginOfRun(aRun);
  G4cout<<"Begin Running ..." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void MyRunAction::EndOfRunAction(const G4Run* aRun)
{
  digiRecorder->WriteCloseROOTFile();
  // hitRecorder->WriteCloseROOTFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
