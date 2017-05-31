#include "RecordHits.hh"

#include "Rtypes.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include "MyTrackerHit.hh"
#include "MyDigitizer.hh"
RecordHits::RecordHits()
{

  // outfile = new TFile("GeantDataHits.root","RECREATE");
  // tree=new TTree("GeantTreeB","background tree");

  // tree->Branch("track",&track);
  // tree->Branch("px",&px);
  // tree->Branch("py",&py);
  // tree->Branch("pz",&pz);
  // tree->Branch("dep",&dep); 
  // tree->Branch("Nbx",&Nbx);
  // tree->Branch("Nby",&Nby);
  // tree->Branch("particle",&particle);

  // tree->Branch("Bichselpx",&Bichselpx);
  // tree->Branch("Bichselpy",&Bichselpy);
  // tree->Branch("Bichselpz",&Bichselpz);
  // tree->Branch("Bichseldep",&Bichseldep); 
  // tree->Branch("BichselNbx",&BichselNbx);
  // tree->Branch("BichselNby",&BichselNby);
}


RecordHits::~RecordHits() 
{
}

//------------Invoke for every Event-----------------------
void RecordHits::RecordHitinROOT(MyTrackerHitsCollection* hitsCollection,std::vector<BichselHits> BichselhitsCollection){
  G4int nHits = hitsCollection->entries();
  for (G4int itr  = 0 ; itr < nHits ; itr++){
    track.push_back((*hitsCollection)[itr]->GetTrackID());
    px.push_back((*hitsCollection)[itr]->GetPosWithRespectToPixel().x());
    py.push_back((*hitsCollection)[itr]->GetPosWithRespectToPixel().y());
    pz.push_back((*hitsCollection)[itr]->GetPosWithRespectToPixel().z());
    dep.push_back((*hitsCollection)[itr]->GetEdep());
    Nbx.push_back((*hitsCollection)[itr]->GetPixelNbX());
    Nby.push_back((*hitsCollection)[itr]->GetPixelNbY());
    particle.push_back((*hitsCollection)[itr]->GetTrackPdgId());
  }
  nHits = BichselhitsCollection.size();
  for (G4int itr  = 0 ; itr < nHits ; itr++){
    Bichselpx.push_back(BichselhitsCollection[itr].pos.x());
    Bichselpy.push_back(BichselhitsCollection[itr].pos.y());
    Bichselpz.push_back(BichselhitsCollection[itr].pos.z());
    Bichseldep.push_back(BichselhitsCollection[itr].energy);
    BichselNbx.push_back(BichselhitsCollection[itr].PixelNb.first);
    BichselNby.push_back(BichselhitsCollection[itr].PixelNb.second);
  }

  outfile->cd();
  tree->Fill();

  track.clear();
  px.clear();
  py.clear();
  pz.clear();
  dep.clear();
  Nbx.clear();
  Nby.clear();
  particle.clear();
  Bichselpx.clear();
  Bichselpy.clear();
  Bichselpz.clear();
  Bichseldep.clear();
  BichselNbx.clear();
  BichselNby.clear();
}

void RecordHits::WriteCloseROOTFile()
{
  G4cout << "CloseHitsROOTFile:--------------------RUN ENDS----------------------- " << G4endl;
  outfile->cd();
  tree->Write();
  outfile->Close();
}
