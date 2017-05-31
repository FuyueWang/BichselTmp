/**
 *  Author John Idarraga <idarraga@cern.ch>
 */
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
// $Id: AllPixTrackerSD.cc,v 1.9 2006/06/29 17:48:27 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MyTrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4DecayTable.hh"
#include "G4LogicalVolume.hh"
#include "DetectorConstruction.hh"
#include "MyTrackerHit.hh"

#include "TMath.h"
#include "TString.h"

// temp
G4double g_temp_edep = 0.;
G4int g_temp_pdgId = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyTrackerSD::MyTrackerSD(const G4String& name,const G4String& hitsCollectionName,G4ThreeVector absPos,DetectorConstruction *gD)
		// G4ThreeVector relPosSD,
  :G4VSensitiveDetector(name),
   hitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName); // only one hit collection per detector for now

  m_absolutePos = absPos;
  m_gD = gD; // Geo description
  firstStrikePrimary = false;
  _totalEdep = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyTrackerSD::~MyTrackerSD(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * Runs once per event.  The hitsCollections pointers are retrieved
 *	I have a set of pointers to control that I get them right
 */
void MyTrackerSD::Initialize(G4HCofThisEvent* HCE)
{
  // Create the hit collection
  hitsCollection =new MyTrackerHitsCollection(SensitiveDetectorName, collectionName[0]);
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, hitsCollection );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MyTrackerSD::ProcessHits(G4Step * aStep, G4TouchableHistory *)
{
  // track
  G4Track * aTrack = aStep->GetTrack();
  // particle
  G4ParticleDefinition * aParticle = aTrack->GetDefinition();
  // create a hit instance
  MyTrackerHit * newHit = new MyTrackerHit();

  if ( aTrack->GetTrackID() == 1 && ! firstStrikePrimary ) {
    _kinEPrimary = aTrack->GetKineticEnergy()/keV;
    _kinEPrimary -= (aStep->GetDeltaEnergy()/keV);
    firstStrikePrimary = true;
  }
  newHit->SetPixelsize(G4ThreeVector(m_gD->GetPixelx(),m_gD->GetPixely(),m_gD->GetPixelz()));
  newHit->SetDigitizerThreshold(m_gD->GetDigitizerThreshold());
  // Work with the Hit
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;
  G4StepPoint * preStepPoint = aStep->GetPreStepPoint();
  G4StepPoint * postStepPoint = aStep->GetPostStepPoint();

  const G4TouchableHandle touchablepre = preStepPoint->GetTouchableHandle();
  const G4TouchableHandle touchablepost = postStepPoint->GetTouchableHandle();

  G4int copyIDy_pre  = -1;
  G4int copyIDx_pre  = -1;
  G4int copyIDy_post = -1;
  G4int copyIDx_post = -1;
  G4ThreeVector correctedPos(0,0,0);
  G4ThreeVector PosOnChip(0,0,0);


  // This positions are global, I will bring them to pixel-centered frame
  // I can use the physical volumes for that
  G4ThreeVector prePos = preStepPoint->GetPosition();

  // Absolute center of Si wafer
  G4ThreeVector absCenterOfDetector = m_absolutePos ;

  //The center of the detector is in the edge of two nearby pixels, not in the center of one pixel
  correctedPos = prePos;  
  correctedPos -= absCenterOfDetector;
 
  // Now let's finally provide pixel-centered coordinates for each hit
  // Build the center of the Pixel
  G4ThreeVector centerOfPixel(
			      m_gD->GetPixelx()*TMath::FloorNint(correctedPos.x() / m_gD->GetPixelx()) + m_gD->GetPixelx()/2.,
			      m_gD->GetPixely()*TMath::FloorNint(correctedPos.y() / m_gD->GetPixely()) +m_gD->GetPixely()/2.,
			      0); // in the middle of the tower

  correctedPos = correctedPos - centerOfPixel;

  // depth 1 --> x
  // depth 0 --> y
  copyIDy_pre  = touchablepre->GetCopyNumber();
  copyIDx_pre  = touchablepre->GetCopyNumber(1);


  // Look at the touchablepost only if in the same volume, i.e. in the sensitive Si Box
  // If the hit is in a different pixel, it is still the same phys volume
  // The problem is that if I the postStep is in a different volume, GetCopyNumber(1)
  //  doesn't make sense anymore.
  G4ThreeVector postPos(0,0,0);
  if(preStepPoint->GetPhysicalVolume() == postStepPoint->GetPhysicalVolume()){
    postPos = postStepPoint->GetPosition();
    copyIDy_post = touchablepost->GetCopyNumber();
    copyIDx_post = touchablepost->GetCopyNumber(1);
  }

  // process
  const G4VProcess * aProcessPointer = aStep->GetPostStepPoint()->GetProcessDefinedStep();


  //newHit->SetDetId(atoi(detId_S.c_str()));
  newHit->SetTrackID(aTrack->GetTrackID());
  newHit->SetParentID(aTrack->GetParentID());
  newHit->SetPixelNbX(copyIDx_pre);
  newHit->SetPixelNbY(copyIDy_pre);
  newHit->SetPostPixelNbX(copyIDx_post);
  newHit->SetPostPixelNbY(copyIDy_post);
  newHit->SetEdep(edep);
  _totalEdep += edep;
  newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetPrePos(aStep->GetPreStepPoint()->GetPosition());

  newHit->SetPosWithRespectToPixel( correctedPos );
  newHit->SetPosInLocalReferenceFrame(PosOnChip);

  newHit->SetProcessName(aProcessPointer->GetProcessName());
  newHit->SetTrackPdgId(aParticle->GetPDGEncoding());

  newHit->SetKinEParent( _kinEPrimary );

  /////////////////////////////////////////////
  g_temp_edep = edep;
  g_temp_pdgId = aParticle->GetPDGEncoding();
  /////////////////////////////////////////////


  newHit->SetTrackVolumeName(aTrack->GetVolume()->GetName());
  newHit->SetParentVolumeName(aTrack->GetLogicalVolumeAtVertex()->GetName());
  hitsCollection->insert(newHit);

  // G4cout << "     entries --> " << hitsCollection->entries() << G4endl;

  // newHit->Print();
  // newHit->Draw();

  if ( _totalEdep > _kinEPrimary ) {
    cout << "[WARNING] totalEdep = " << _totalEdep << ", kinEPrimary = " << _kinEPrimary << endl;
  }

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyTrackerSD::EndOfEvent(G4HCofThisEvent* HCE )
{
  // G4int NbHits = hitsCollection->entries();
  // if(NbHits > 0)
  // 	G4cout << "--------> Hits Collection : " << collectionName[0] << " has " << NbHits << " hits " << G4endl;

  firstStrikePrimary = false;
  _totalEdep = 0.;
  _kinEPrimary = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

