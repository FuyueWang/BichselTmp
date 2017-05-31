/**
 * Author:
 *    
 *
 *  allpix Authors:
 *   John Idarraga <idarraga@cern.ch>
 *   Mathieu Benoit <benoit@lal.in2p3.fr>
 */

#ifndef MyDigitizer_h
#define MyDigitizer_h 1

// allpix Interface
#include "G4VDigitizerModule.hh"
// digits for this digitizer
#include "MyDigi.hh"
#include "G4PrimaryVertex.hh"
#include "MyBichselTool.hh"
#include <map>
#include <vector>
#include "G4PrimaryVertex.hh"

using namespace std;

/**
 *  Digitizer My implementation
 */

struct BichselHits
{
  G4ThreeVector pos;
  pair<G4int,G4int> PixelNb;
  G4double energy;
};


class MyDigitizer : public G4VDigitizerModule {

public:

  MyDigitizer(const G4String& modName,const G4String& digsCollectionName);
  virtual ~MyDigitizer();

  void Digitize ();
  void SetDetectorDigitInputs(G4double){};
  G4int GetCluster_x (vector<pair<G4int,G4int>> cluster);
  G4int GetCluster_y (vector<pair<G4int,G4int>> cluster);
  G4int IsInVector(vector<pair<G4int,G4int>> a,pair<G4int,G4int> b);
  G4int ChargetoToT(G4String whichmodel,std::vector<G4double>* Pixel,Int_t* clusterx,Int_t *clustery,Int_t *clustersize);
  MyDigitsCollection* GetDigitsCollection(){return digitsCollection;};
  std::vector<BichselHits> GetBichHitsCollection(){return BichselhitsCollection;};

  void SetPrimaryVertex(G4PrimaryVertex* Vertex){PrimaryVertex=Vertex;};

private:
  MyDigitsCollection* digitsCollection;
  G4String m_hitsColName;
  MyBichselTool Bichsel;
  G4String digitizerName;
  G4double pixelx,pixely,pixelz;
  G4double elec,noise,threshold,diff;
  bool doBichsel,doDiff,deltaray;
  G4int pixelsizex,pixelsizey,centerpixelshiftx,centerpixelshifty,Nbofbit;
  std::vector<BichselHits> BichselhitsCollection;
  G4PrimaryVertex* PrimaryVertex;
};

#endif
