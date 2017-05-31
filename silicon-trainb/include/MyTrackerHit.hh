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
// $Id: B2TrackerHit.hh 69505 2013-05-07 01:57:27Z asaim $
//
/// \file MyTrackerHit.hh
/// \brief Definition of the MyTrackerHit class

#ifndef MyTrackerHit_h
#define MyTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class MyTrackerHit : public G4VHit
{
  public:
    MyTrackerHit();
    MyTrackerHit(const MyTrackerHit&);
    ~MyTrackerHit();

    // operators
    const MyTrackerHit& operator=(const MyTrackerHit&);
    G4int operator==(const MyTrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
  void SetTrackID  (G4int track)      { trackID = track; };
  void SetParentID  (G4int track)      { parentID = track; };
  void SetPixelNbX(G4int pix)      { pixelNbX = pix; };
  void SetPixelNbY(G4int pix)      { pixelNbY = pix; };
  void SetPostPixelNbX(G4int postPix) { postPixelNbX = postPix; };
  void SetPostPixelNbY(G4int postPix) { postPixelNbY = postPix; };
  void SetEdep     (G4double de)      { edep = de; };
  void SetBichselEdep(G4double de)      { bichseledep = de; }
  void SetPos      (G4ThreeVector xyz){ pos = xyz; };
  void SetPrePos   (G4ThreeVector xyz){ prepos = xyz; };
  void SetBichselPosz(G4double z){Bichselposz=z;};
  void SetPosWithRespectToPixel (G4ThreeVector pxzy) { m_posWithRespectToPixel = pxzy; };
  void SetPosInLocalReferenceFrame (G4ThreeVector xyz) { m_posInLocalReferenceFrame = xyz; };
  void SetProcessName(G4String process) { processName = process; };
  void SetTrackPdgId(G4int pdgId) { pdgIdTrack = pdgId; };
  void SetTrackVolumeName(G4String vn) { trackVolumeName = vn; };
  void SetParentVolumeName(G4String vn) { parentVolumeName = vn; } ;
  void SetDetId(G4int i){ detID = i; };
  void SetKinEParent(G4double kinE){ kinEParent = kinE; };
  // void SetKineticEnergy     (G4double kinE) {kinE = kinE; };
  void SetPixelsize(G4ThreeVector xyz){ pixelsize = xyz; };
  void SetDigitizerThreshold(G4double thre)      { threshold = thre; };

  G4int GetTrackID()    { return trackID; };
  G4int GetParentID()    { return parentID; };
  G4int GetPixelNbX()  { return pixelNbX; };
  G4int GetPixelNbY()  { return pixelNbY; };
  G4int GetPostPixelNbX()  { return postPixelNbX; };
  G4int GetPostPixelNbY()  { return postPixelNbY; };
  G4int GetDetId() { return detID; };
  G4double GetEdep()    { return edep; };
  G4double GetBichselEdep() {return bichseledep; }
  G4ThreeVector GetPos(){ return pos; };
  G4ThreeVector GetPrePos(){ return prepos; };
  G4double GetBichselPosz(){return Bichselposz;};
  G4ThreeVector GetPosWithRespectToPixel() { return m_posWithRespectToPixel; };
  G4ThreeVector GetPosInLocalReferenceFrame() {return m_posInLocalReferenceFrame; };
  G4String GetProcessName() { return processName; };
  G4int GetTrackPdgId() { return pdgIdTrack; };
  G4String GetTrackVolumeName() {return trackVolumeName;};
  G4String GetParentVolumeName() {return parentVolumeName;};
  G4double GetKinEParent() { return kinEParent; };
  // G4double GetKineticEnergy(){ return kinE; };
  G4ThreeVector GetPixelsize(){ return pixelsize; };
  G4double GetDigitizerThreshold()      { return threshold; };

private:
  
  G4int         trackID;
  G4int         detID;
  G4int         parentID;
  G4int         pixelNbX;
  G4int         pixelNbY;
  G4int         postPixelNbX;
  G4int         postPixelNbY;
  G4double      edep;
  G4double      bichseledep; 
  G4ThreeVector pos,prepos;
  G4double      Bichselposz;
  G4ThreeVector m_posWithRespectToPixel;
  G4ThreeVector m_posInLocalReferenceFrame;
  G4String      processName;
  G4int         pdgIdTrack;
  G4String      trackVolumeName;
  G4String      parentVolumeName;
  G4double     	kinEParent;
  G4double      threshold;
  G4ThreeVector pixelsize;
  // G4double      kinE;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<MyTrackerHit> MyTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<MyTrackerHit>* MyTrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* MyTrackerHit::operator new(size_t)
{
  if(!MyTrackerHitAllocator)
      MyTrackerHitAllocator = new G4Allocator<MyTrackerHit>;
  return (void *) MyTrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void MyTrackerHit::operator delete(void *hit)
{
  MyTrackerHitAllocator->FreeSingle((MyTrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
