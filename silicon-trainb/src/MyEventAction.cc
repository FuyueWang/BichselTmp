//nTPCEventAction.cc

#include "MyEventAction.hh"

#include "RecordDigits.hh"
#include "RecordHits.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4PrimaryVertex.hh"
#include "DetectorConstruction.hh"
#include "MyDigitizer.hh"
#include "G4DigiManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
MyEventAction::MyEventAction(RecordHits* hitrec,RecordDigits* digirec)
  : hitRecorder(hitrec),
    digiRecorder(digirec)
{
  digitizerName="MyDigitizer";
  digitsCollectionName="DigitCollection";

  DetectorConstruction* det=new DetectorConstruction;
  m_hitsColName=det->GetHitCollectionName();

  myDM = new MyDigitizer(digitizerName,digitsCollectionName);
  G4DigiManager::GetDMpointer()->AddNewModule(myDM);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
MyEventAction::~MyEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void MyEventAction::BeginOfEventAction(const G4Event* evt)
{
  //G4cout<<"eventID="<<evt->GetEventID()<<G4endl;
  // recorders->RecordBeginOfEvent(evt);
  G4PrimaryVertex* primaryVertex=evt->GetPrimaryVertex();
  // G4cout<<"event Action verx="<<primaryVertex->GetX0()<<" very="<<primaryVertex->GetY0()<<" verz="<<primaryVertex->GetZ0()<<G4endl;
  myDM->SetPrimaryVertex(primaryVertex);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void MyEventAction::EndOfEventAction(const G4Event* evt)
{
  myDM->Digitize();

  MyDigitsCollection* digitsCollection=myDM->GetDigitsCollection();

  // G4DigiManager * digiMan = G4DigiManager::GetDMpointer();
  // G4int hcID = digiMan->GetHitsCollectionID(m_hitsColName);
  // MyTrackerHitsCollection* hitsCollection = (MyTrackerHitsCollection*)(digiMan->GetHitsCollection(hcID));

  digiRecorder->RecordDigitinROOT(digitsCollection);
  // hitRecorder->RecordHitinROOT(hitsCollection,myDM->GetBichHitsCollection());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
