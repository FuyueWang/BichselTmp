#ifndef RecordHits_h
#define RecordHits_h 1


#include "MyTrackerHit.hh"
#include "TFile.h"
#include "TTree.h"
#include "Rtypes.h"
#include "TROOT.h"
#include "MyDigitizer.hh"
class RecordHits

{
public:
  RecordHits();
  ~RecordHits();

  void RecordHitinROOT(MyTrackerHitsCollection* hitsCollection,std::vector<BichselHits> BichselhitsCollection);
  void WriteCloseROOTFile();

private:
  TFile *outfile;
  TTree *tree;
  std::vector<Int_t> Nbx,Nby,particle,track,BichselNbx,BichselNby;
  std::vector<Double_t> px,py,pz,dep,Bichselpx,Bichselpy,Bichselpz,Bichseldep;

};

#endif

