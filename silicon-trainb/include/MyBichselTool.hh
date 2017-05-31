#ifndef MyBichselTool_h
#define MyBichselTool_h 1
#include <vector>
#include "TROOT.h"
#include "TObject.h"
#include "Rtypes.h"
struct BichselData
{
  std::vector<double> Array_BetaGammaLog10;
  std::vector<std::vector<double> > Array_BetaGammaLog10_ColELog10;  // ColE = CollisionEnergy in eV
  std::vector<std::vector<double> > Array_BetaGammaLog10_IntXLog10;  // IntX = Integrated Xsection. The unit doesn't matter
  std::vector<double> Array_BetaGammaLog10_UpperBoundIntXLog10;      // upper bound of log10(IntX)
};

class MyBichselTool
{

 public:
  MyBichselTool();
  virtual ~MyBichselTool();
  std::vector<std::pair<Double_t,Double_t> > BichselSim(Double_t BetaGamma, int ParticleType, Double_t TotalLength, Double_t InciEnergy);
  Double_t GetTotalBichselEnergy(){return TotalBichselEnergy;};
  Int_t trfPDG(int pdgId);
  std::vector<std::pair<double,double> > ClusterHits(std::vector<std::pair<double,double> >& rawHitRecord, int n_pieces);
  void Initilize();

 private:
  std::pair<int,int> FastSearch(std::vector<double> vec, double item);
  Double_t GetUpperBound(Double_t BetaGammaLog10, BichselData& iData);
  double GetUpperBound(std::pair<int,int> indices_BetaGammaLog10, double BetaGammaLog10, BichselData& iData);
  double GetColE(std::pair<int,int> indices_BetaGammaLog10, double IntXLog10, BichselData& iData);
  double GetColE(double BetaGammaLog10, double IntXLog10, BichselData& iData) ;
  std::pair<int,int> GetBetaGammaIndices(Double_t BetaGammaLog10, BichselData& iData);
  BichselData BichselDataTransform(Int_t iParticleType);

  Double_t TotalBichselEnergy;//eV
  std::vector<BichselData> m_BichselData;   
  int                      m_nCols;            // number of collisions to simulate each time. This is mainly to save CPU time if necessary

};
#endif
