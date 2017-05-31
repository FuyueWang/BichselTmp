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
// $Id: B2TrackerHit.cc 69505 2013-05-07 01:57:27Z asaim $
//
/// \file B2TrackerHit.cc
/// \brief Implementation of the MyTrackerHit class

#include "MyTrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<MyTrackerHit>* MyTrackerHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyTrackerHit::MyTrackerHit()
 : G4VHit()
   // fTrackID(-1),
   // fChamberNb(-1),
   // fEdep(0.),
   // fPos(G4ThreeVector())
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyTrackerHit::~MyTrackerHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyTrackerHit::MyTrackerHit(const MyTrackerHit& right)
  : G4VHit()
{
	trackID   = right.trackID;
	detID = right.detID;
	parentID = right.parentID;
	pixelNbX = right.pixelNbX;
	pixelNbY = right.pixelNbY;
	postPixelNbX = right.postPixelNbX;
	postPixelNbY = right.postPixelNbY;
	edep      = right.edep;
	pos       = right.pos;
	m_posWithRespectToPixel = right.m_posWithRespectToPixel;
	processName = right.processName;
	pdgIdTrack = right.pdgIdTrack;
	trackVolumeName = right.trackVolumeName;
	parentVolumeName = right.parentVolumeName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MyTrackerHit& MyTrackerHit::operator=(const MyTrackerHit& right)
{
  trackID   = right.trackID;
	detID = right.detID;
	parentID = right.parentID;
	pixelNbX = right.pixelNbX;
	pixelNbY = right.pixelNbY;
	postPixelNbX = right.postPixelNbX;
	postPixelNbY = right.postPixelNbY;
	edep      = right.edep;
	pos       = right.pos;
	m_posWithRespectToPixel = right.m_posWithRespectToPixel;
	processName = right.processName;
	pdgIdTrack = right.pdgIdTrack;
	trackVolumeName = right.trackVolumeName;
	parentVolumeName = right.parentVolumeName;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int MyTrackerHit::operator==(const MyTrackerHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyTrackerHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyTrackerHit::Print()
{
	G4cout << "  trackID: " << trackID << "  pixelNbX: " << pixelNbX
			<< "  pixelNbY: " << pixelNbY
			<< "  energy deposit: " << G4BestUnit(edep,"Energy")
			<< "  position: " << G4BestUnit(pos,"Length") << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
