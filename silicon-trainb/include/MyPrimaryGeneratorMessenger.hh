#ifndef MyPrimaryGeneratorMessenger_h
#define MyPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
class MyPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADouble;

class MyPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  MyPrimaryGeneratorMessenger(MyPrimaryGeneratorAction* );
  ~MyPrimaryGeneratorMessenger();
  void SetNewValue(G4UIcommand*, G4String);

private:
  MyPrimaryGeneratorAction*  fPrimaryGenerator;
  G4UIdirectory*             fSiDirectory;
  G4UIdirectory*             fGeneDirectory;
  G4UIcmdWithADouble*        fEtaCmd;
  G4UIcmdWithADouble*        fFaiCmd;
  G4UIcmdWith3VectorAndUnit* fPixelsizeCmd;
};
#endif
