// nTPCRecorders.hh

#ifndef RecordDigits_h
#define RecordDigits_h 1

// The RecordersBase object.  We're implementing this abstract class
// with methods that fill histograms.
#include "G4Types.hh"
#include "G4String.hh"
// for saving data to file
#include <stdio.h>
#include <string>
// ROOT classes
#include "Rtypes.h"
#include "TROOT.h"
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "MyDigi.hh"
// Forward declarations of G4 classes that are arguments to
// RecordersBase methods.
class G4Run;
class G4Event;

class RecordDigits
{
public:
  RecordDigits();
  ~RecordDigits();

  void RecordDigitinROOT(MyDigitsCollection* digitsCollection);
  void WriteCloseROOTFile();

private:
  TFile *outfile;
  TTree *tree;
  std::vector<Double_t> pixel,Bichselpixel;
  Int_t clusterx,clustery,clustersize,Bichselclusterx,Bichselclustery,Bichselclustersize,Bichseleff,Geant4eff; 
  Double_t truthposx,truthposy,energy,Bichselenergy;
};

#endif





