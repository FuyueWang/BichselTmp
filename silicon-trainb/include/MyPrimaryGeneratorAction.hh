#ifndef MyPrimaryGeneratorAction_h
#define MyPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "MyPrimaryGeneratorMessenger.hh"
#include "G4ThreeVector.hh"
// ROOT classes
#include "Rtypes.h"
// #include "TROOT.h"
//#include "TNtuple.h"
// #include "TBranch.h"
#include "TFile.h"
#include "TTree.h"

class G4ParticleGun;
class G4Event;

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  MyPrimaryGeneratorAction();
  ~MyPrimaryGeneratorAction();
  void GeneratePrimaries(G4Event* anEvent);
  void SetEta(G4double value){ eta=value;}
  void SetFai(G4double value){ fai=value;}
  void SetPixelsize(G4ThreeVector pixelsize);
  
private:
  G4ParticleGun* particleGun;
  Double_t pixelx,pixely,pixelz,eta,fai;
  MyPrimaryGeneratorMessenger *m_primarygenMessenger;
};

#endif


