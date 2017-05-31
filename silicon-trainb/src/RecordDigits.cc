// nTPCRecorders.cc

#include "RecordDigits.hh"

// Needed to convert numbers to strings
#include <iostream>
#include <string>

// Include files for the G4 classes
#include "globals.hh"
#include "G4ios.hh"

// ROOT classes
#include "Rtypes.h"
#include "TROOT.h"
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
//#include "TNtuple.h"
// for saving data to file
#include <stdio.h>
// for output format
#include "G4UnitsTable.hh"
#include "memory.h"

#include "MyDigi.hh"
//--------------------------------------------
RecordDigits::RecordDigits()
{
  //outfile is used to restore the tree! 
  outfile = new TFile("GeantData.root","RECREATE");
  tree=new TTree("GeantTreeB","background tree");

  // tree->Branch("BichselEff",&Bichseleff);
  // tree->Branch("Geant4Eff",&Geant4eff);
  tree->Branch("pixel",&pixel);
  tree->Branch("clusterx",&clusterx);
  tree->Branch("clustery",&clustery);
  tree->Branch("clustersize",&clustersize);
  // tree->Branch("BichselPixel",&Bichselpixel);
  // tree->Branch("Bichselclusterx",&Bichselclusterx);
  // tree->Branch("Bichselclustery",&Bichselclustery);
  // tree->Branch("Bichselclustersize",&Bichselclustersize);
  tree->Branch("truthposx",&truthposx);
  tree->Branch("truthposy",&truthposy);
  tree->Branch("energy", &energy);
  // tree->Branch("Bichselenergy", &Bichselenergy);
}

RecordDigits::~RecordDigits() 
{
}

//------------Invoke for every Event-----------------------
void RecordDigits::RecordDigitinROOT(MyDigitsCollection* digitsCollection){
  G4int nDigits = digitsCollection->entries();
  for (G4int itr  = 0 ; itr < nDigits ; itr++){
    // Bichselpixel=(*digitsCollection)[itr]->GetBichselPixelToT();
    // Bichselclusterx=(*digitsCollection)[itr]->GetBichselClusterx();
    // Bichselclustery=(*digitsCollection)[itr]->GetBichselClustery();
    // Bichselclustersize=(*digitsCollection)[itr]->GetBichselClustersize();
    pixel=(*digitsCollection)[itr]->GetPixelToT();
    clusterx=(*digitsCollection)[itr]->GetClusterx();
    clustery=(*digitsCollection)[itr]->GetClustery();
    clustersize=(*digitsCollection)[itr]->GetClustersize();
    truthposx=(*digitsCollection)[itr]->GetTruthPosX();
    truthposy=(*digitsCollection)[itr]->GetTruthPosY();
    energy=(*digitsCollection)[itr]->GetPixelEnergyDep();
    // Bichselenergy=(*digitsCollection)[itr]->GetBichselPixelEnergyDep();
    // Bichseleff=(*digitsCollection)[itr]->GetBichselEff();
    // Geant4eff=(*digitsCollection)[itr]->GetGeant4Eff();
    // G4cout<<"Record clustersize="<<clustersize<<" Bichselclustersize="<<Bichselclustersize<<G4endl;
    outfile->cd();
    tree->Fill();
    // Bichseleff=999;
    // Geant4eff=999;
    // Bichselpixel.clear();
    // Bichselclusterx=999;
    // Bichselclustery=999;
    // Bichselclustersize=999;
    pixel.clear();
    clusterx=999;
    clustery=999;
    clustersize=999;
    truthposx=999;
    truthposy=999;
    energy=999;
    // Bichselenergy=999;
  }
}


void RecordDigits::WriteCloseROOTFile()
{
  G4cout << "CloseROOTFile:----------------------RUN ENDS----------------------- " << G4endl;
  outfile->cd();
  tree->Write();
  outfile->Close();
}
