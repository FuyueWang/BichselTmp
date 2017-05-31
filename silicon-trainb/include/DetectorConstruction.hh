
#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4UserLimits.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

  DetectorConstruction();
  ~DetectorConstruction();

  G4VPhysicalVolume* Construct();

  G4double GetPixelx(){ return pixelx; };
  G4double GetPixely(){ return pixely; };
  G4double GetPixelz(){ return pixelz; };
  G4double GetSiplatex(){ return siplatex; };
  G4double GetSiplatey(){ return siplatey; };
  G4double GetSiplatez(){ return siplatez; };
  G4int GetNbofPixely(){ return NbofPixely; };
  G4int GetNbofPixelx(){ return NbofPixelx; };
  G4int GetNbofPixelz(){ return NbofPixelz; };
  void SetMaxStepSize(G4double maxStep);
  // void SetPixelx(G4double value);
  // void SetPixely(G4double value);
  // void SetPixelz(G4double value);
  void SetDetectorPixelsize(G4ThreeVector pixelsize);
  void UpdateGeometry();
  G4String GetHitCollectionName(){return HitCollectionName;};
  G4double GetDigitizerThreshold(){return threshold;};
  void SetDigitizerThreshold(G4double thre);
  private:
  G4int NbofPixely,NbofPixelx,NbofPixelz;
  G4double pixelx,pixely,pixelz;
  G4double siplatex,siplatey,siplatez;
  DetectorMessenger *m_detectorMessenger;
  G4double m_maxStepLengthSensor;
  G4UserLimits* fStepLimit;
  G4String HitCollectionName;
  G4double threshold;
};

#endif

