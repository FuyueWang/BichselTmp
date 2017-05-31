// MyRunAction.hh

#ifndef MyRunAction_h
#define MyRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class RecordDigits;
class RecordHits;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MyRunAction : public G4UserRunAction
{
  public:
   MyRunAction(RecordHits* hitrec,RecordDigits* digirec);
   ~MyRunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);


  private: 
    RecordDigits* digiRecorder;
    RecordHits* hitRecorder;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
