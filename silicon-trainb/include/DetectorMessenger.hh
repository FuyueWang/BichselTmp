#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"


class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
    ~DetectorMessenger();
    void SetNewValue(G4UIcommand*, G4String);

private:
    DetectorConstruction*    fDetectorConstruction;
    G4UIdirectory*           fSiDirectory;
    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
    G4UIcmdWithADoubleAndUnit* fPixelxCmd;
    G4UIcmdWithADoubleAndUnit* fPixelyCmd;
    G4UIcmdWithADoubleAndUnit* fPixelzCmd;
    G4UIcmdWithADouble* fThreCmd;
    G4UIcmdWithoutParameter*   fUpdateCmd;
    G4UIcmdWith3VectorAndUnit* fPixelsizeCmd;

};
#endif
