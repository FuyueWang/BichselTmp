#include "MyBichselTool.hh"
#include <fstream>
#include <cmath>
#include <vector>
#include "TROOT.h"
#include "TObject.h"
#include "Rtypes.h"
#include "TMath.h"
#include "TRandom.h"
#include <iostream>
MyBichselTool::MyBichselTool(){}

MyBichselTool::~MyBichselTool(){;}

void MyBichselTool::Initilize(){
  m_nCols=10;
  for(Int_t ParticleType=6;ParticleType<7;ParticleType++){
    m_BichselData.push_back(BichselDataTransform(ParticleType));
  }
}
 

std::vector<std::pair<Double_t,Double_t> > MyBichselTool::BichselSim(Double_t BetaGamma, Int_t ParticleType, Double_t TotalLength, Double_t InciEnergy){ 
  TotalBichselEnergy=0;
  BichselData iData = m_BichselData[0];//[ParticleType-1];
  std::vector<std::pair<Double_t,Double_t> > rawHitRecord;  
  Double_t accumLength = 0.;
  Double_t BetaGammaLog10 = TMath::Log10(BetaGamma);
  std::pair<int,int> indices_BetaGammaLog10 = GetBetaGammaIndices(BetaGammaLog10, iData);

  // upper bound
  Double_t IntXUpperBound = GetUpperBound(indices_BetaGammaLog10, BetaGammaLog10, iData);
  if(IntXUpperBound <= 0.){
  }
  Double_t lambda = (1./IntXUpperBound) * 1.E4;   //um, unit of IntX is cm-1. It needs to be converted to micrometer-1
  // std::cout<<lambda<<std::endl;
  if(std::isnan(lambda)){std::cout<<"lambda is nan"<<std::endl;}  // check nan lambda
  int LoopLimit=5000;
  // int LoopLimit = 1.5*TotalLength/lambda;//1000;  // limit assuming 1 collision per sampling，这个是一共的steps数目
  // std::cout<<"Looplimit="<<LoopLimit<<std::endl;
  if(fabs(1.0*TotalLength/lambda) > LoopLimit){  }
  // begin simulation
  int count = 0;
  while(true){
    // infinite loop protection
    if(count >= (1.0*LoopLimit/m_nCols)){
      std::cout<<111<<std::endl; break;
    }
    double HitPosition = 0.;
    for(int iHit = 0; iHit < m_nCols; iHit++) HitPosition += gRandom->Exp(lambda); 
    if(accumLength + HitPosition >= TotalLength) break;
    // std::cout<<"length="<<accumLength + HitPosition<<std::endl;
    // sample single collision
    double TossEnergyLoss = -1.;
    while(TossEnergyLoss <= 0.){ // we have to do this because sometimes TossEnergyLoss will be negative due to too small TossIntX
      double TossIntX = gRandom->Uniform( 0., IntXUpperBound);
      TossEnergyLoss = GetColE(indices_BetaGammaLog10, TMath::Log10(TossIntX), iData);
      // TossIntX_record = TossIntX;
    }
    // std::cout<<"TossEnergyLoss="<<TossEnergyLoss<<std::endl;
    //deltaray
   // if(TossEnergyLoss > (m_DeltaRayCut*1000.)){
   //  TossEnergyLoss = 0.;
   //  }

    bool fLastStep = false;
    // in case energy loss so far is larger than incident energy -- basically not possible ...
    // This becomes important after taking delta-ray into account!
    if( ((TotalBichselEnergy + TossEnergyLoss)/1.E+6) > InciEnergy ){
      TossEnergyLoss = InciEnergy*1.E+6 -TotalBichselEnergy;
      fLastStep = true;
    }
    // update
    accumLength += HitPosition;
    TotalBichselEnergy += TossEnergyLoss;
 
    // record this hit
    std::pair<double,double> oneHit;
    if(m_nCols == 1)  oneHit.first = accumLength; 
    else              oneHit.first = (accumLength - 1.0*HitPosition/2);     // as long as m_nCols is small enough (making sure lambda*m_nCols is withint resolution of a pixel), then taking middle point might still be reasonable
    oneHit.second = TossEnergyLoss;
    rawHitRecord.push_back(oneHit);

    count++;

    if(fLastStep)
      break;
  }
   // std::cout<<"accumLength="<<accumLength<<" BichselEnergy="<<BichselEnergy<<std::endl;
   return rawHitRecord;
}

// assume vec is already sorted from small to large
std::pair<int,int> MyBichselTool::FastSearch(std::vector<double> vec, double item) {
  std::pair<int,int> output;

  int index_low = 0;
  int index_up = vec.size()-1;

  if((item < vec[index_low]) || (item > vec[index_up])){
    output.first = -1; output.second = -1;
    return output;
  }
  else if(item == vec[index_low]){
    output.first = index_low; output.second = index_low;
    return output;
  }
  else if(item == vec[index_up]){
    output.first = index_up; output.second = index_up;
    return output;
  }

  while( (index_up - index_low) != 1 ){
    int index_middle = int(1.0*(index_up + index_low)/2.);
    if(item < vec[index_middle])//二分法
      index_up = index_middle;
    else if(item > vec[index_middle])
      index_low = index_middle;
    else{ // accurate hit. Though this is nearly impossible ...
      output.first = index_middle; output.second = index_middle;
      return output;
    }
  }

  output.first = index_low; output.second = index_up;
  return output;
}

double MyBichselTool::GetColE(std::pair<int,int> indices_BetaGammaLog10, double IntXLog10, BichselData& iData) {
  // std::pair<int,int> indices_BetaGammaLog10;
  // if(BetaGammaLog10 > iData.Array_BetaGammaLog10.back()){ // last one is used because when beta-gamma is very large, energy deposition behavior is very similar
  //   indices_BetaGammaLog10.first = iData.Array_BetaGammaLog10.size()-1;
  //   indices_BetaGammaLog10.second = iData.Array_BetaGammaLog10.size()-1;
  // }
  // else{
  //   indices_BetaGammaLog10 = FastSearch(iData.Array_BetaGammaLog10, BetaGammaLog10);
  // }

  if( (indices_BetaGammaLog10.first==-1) && (indices_BetaGammaLog10.second==-1) )
    return -1.;

  // BetaGammaLog10_2 then
  std::pair<int,int> indices_IntXLog10_x2 = FastSearch(iData.Array_BetaGammaLog10_IntXLog10[indices_BetaGammaLog10.second], IntXLog10);
  if (indices_IntXLog10_x2.first<0)  { return -1; }
  if (indices_IntXLog10_x2.second<0) { return -1; }
  double y21 = iData.Array_BetaGammaLog10_IntXLog10[indices_BetaGammaLog10.second][indices_IntXLog10_x2.first];
  double y22 = iData.Array_BetaGammaLog10_IntXLog10[indices_BetaGammaLog10.second][indices_IntXLog10_x2.second];
  double Est_x2 = ((y22 - IntXLog10)*iData.Array_BetaGammaLog10_ColELog10[indices_BetaGammaLog10.second][indices_IntXLog10_x2.first] + (IntXLog10 - y21)*iData.Array_BetaGammaLog10_ColELog10[indices_BetaGammaLog10.second][indices_IntXLog10_x2.second])/(y22-y21);

  // final estimation
  //double Est = ((BetaGammaLog10_2 - BetaGammaLog10)*Est_x1 + (BetaGammaLog10 - BetaGammaLog10_1)*Est_x2)/(BetaGammaLog10_2 - BetaGammaLog10_1);
  double Est = Est_x2;

  return TMath::Power(10., Est);
}



double MyBichselTool::GetColE(double BetaGammaLog10, double IntXLog10, BichselData& iData) {
  std::pair<int,int> indices_BetaGammaLog10 = GetBetaGammaIndices(BetaGammaLog10, iData);
  return GetColE(indices_BetaGammaLog10, IntXLog10, iData);
}

// IMPORTANT!! For this one, one should use interpolation, instead of fixed beta-gamma.
// Otherwise, dE/dx shape will get distorted again.
double MyBichselTool::GetUpperBound(std::pair<int,int> indices_BetaGammaLog10, double BetaGammaLog10, BichselData& iData) {
  // std::pair<int,int> indices_BetaGammaLog10;
  // if(BetaGammaLog10 > iData.Array_BetaGammaLog10.back()){
  //   indices_BetaGammaLog10.first = iData.Array_BetaGammaLog10.size()-1;
  //   indices_BetaGammaLog10.second = iData.Array_BetaGammaLog10.size()-1;
  // }
  // else{
  //   indices_BetaGammaLog10 = FastSearch(iData.Array_BetaGammaLog10, BetaGammaLog10);
  // }

  if (indices_BetaGammaLog10.first<0)  { return -1; }
  if (indices_BetaGammaLog10.second<0) { return -1; }
  double BetaGammaLog10_1 = iData.Array_BetaGammaLog10[indices_BetaGammaLog10.first];
  double BetaGammaLog10_2 = iData.Array_BetaGammaLog10[indices_BetaGammaLog10.second];

  // obtain estimation
  double Est_1 = iData.Array_BetaGammaLog10_UpperBoundIntXLog10[indices_BetaGammaLog10.first];
  double Est_2 = iData.Array_BetaGammaLog10_UpperBoundIntXLog10[indices_BetaGammaLog10.second];

  // final estimation
  double Est = ((BetaGammaLog10_2 - BetaGammaLog10)*Est_1 + (BetaGammaLog10 - BetaGammaLog10_1)*Est_2)/(BetaGammaLog10_2 - BetaGammaLog10_1);

  return TMath::Power(10., Est);
}


Double_t MyBichselTool::GetUpperBound(Double_t BetaGammaLog10, BichselData& iData) {
  std::pair<int,int> indices_BetaGammaLog10 = GetBetaGammaIndices(BetaGammaLog10, iData);
  return GetUpperBound(indices_BetaGammaLog10, BetaGammaLog10, iData);
}

std::pair<int,int> MyBichselTool::GetBetaGammaIndices(Double_t BetaGammaLog10, BichselData& iData) {
  std::pair<int,int> indices_BetaGammaLog10;
  if(BetaGammaLog10 > iData.Array_BetaGammaLog10.back()){ // last one is used because when beta-gamma is very large, energy deposition behavior is very similar
    indices_BetaGammaLog10.first = iData.Array_BetaGammaLog10.size()-1;
    indices_BetaGammaLog10.second = iData.Array_BetaGammaLog10.size()-1;
  }
  else{
    indices_BetaGammaLog10 = FastSearch(iData.Array_BetaGammaLog10, BetaGammaLog10);//找到iData.Array_BetaGammaLog10数组中等于BetaGammaLog10的那个的indices，iData.Array_BetaGammaLog10是一个每个元素都按从小到大排列的数组，
  }
  return indices_BetaGammaLog10;
}

//read from the Bichsel Model Table, store the data in the BichselData.
BichselData MyBichselTool::BichselDataTransform(Int_t iParticleType){
  std::ifstream inputFile;
  TString inputFileName = TString::Format("/home/fuyue/fromcern/may23/data/Bichsel_%d%s.dat", iParticleType, m_nCols == 1 ? "" : TString::Format("_%dsteps", m_nCols).Data());
  TString FullFileName=inputFileName;
  // std::cout<<"filename="<<FullFileName<<std::endl;
  inputFile.open(FullFileName);
  BichselData iData;    
  Double_t BetaGammaLog10 = 0; inputFile >> BetaGammaLog10;
  Double_t ColELog10 = 0;      inputFile >> ColELog10;
  Double_t IntXLog10 = 0;      inputFile >> IntXLog10;

  while(!inputFile.eof()){
    // check if this BetaGamma has already been stored
    if((iData.Array_BetaGammaLog10.size()==0)||(iData.Array_BetaGammaLog10.back()!=BetaGammaLog10)){ // a new BetaGamma
      if(iData.Array_BetaGammaLog10.size() != 0){//is not first betagamma
	iData.Array_BetaGammaLog10_UpperBoundIntXLog10.push_back(iData.Array_BetaGammaLog10_IntXLog10.back().back());
      } 
      iData.Array_BetaGammaLog10.push_back(BetaGammaLog10);
      std::vector<Double_t> new_ColELog10;  
      iData.Array_BetaGammaLog10_ColELog10.push_back(new_ColELog10);
      std::vector<Double_t> new_IntXLog10;  
      iData.Array_BetaGammaLog10_IntXLog10.push_back(new_IntXLog10);
    }
    iData.Array_BetaGammaLog10_ColELog10.back().push_back(ColELog10);
    iData.Array_BetaGammaLog10_IntXLog10.back().push_back(IntXLog10);

    inputFile >> BetaGammaLog10;
    inputFile >> ColELog10;
    inputFile >> IntXLog10;
  }
  iData.Array_BetaGammaLog10_UpperBoundIntXLog10.push_back(iData.Array_BetaGammaLog10_IntXLog10.back().back());
  return iData;
}



Int_t MyBichselTool::trfPDG(int pdgId) {
  if(std::fabs(pdgId) == 2212) return 1;   // proton
  if(std::fabs(pdgId) == 211)  return 2;   // pion
  // alpha is skipped -- 3
  if(std::fabs(pdgId) == 11)   return 4;   // electron
  if(std::fabs(pdgId) == 321)  return 5;   // kaon
  if(std::fabs(pdgId) == 13)   return 6;   // muon

  return -1;   // unsupported particle
}


//cluster thousand hits into ~20 groups
std::vector<std::pair<double,double> > MyBichselTool::ClusterHits(std::vector<std::pair<double,double> >& rawHitRecord, int n_pieces){
 
  std::vector<std::pair<double,double> > trfHitRecord;

  if((int)(rawHitRecord.size()) < n_pieces){ // each single collision is the most fundamental unit
    n_pieces = rawHitRecord.size();
  }

  int unitlength = int(1.0*rawHitRecord.size()/n_pieces);//每个group中有多少hits
  int index_start = 0;
  int index_end = unitlength-1;   // [index_start, index_end] are included
  while(true){
    // calculate weighted center of each slice
    double position = 0.;
    double energyloss = 0.;

    for(int index = index_start; index <= index_end; index++){
      position += (rawHitRecord[index].first * rawHitRecord[index].second);
      energyloss += rawHitRecord[index].second;
    }
    position = (energyloss == 0. ? 0. : position/energyloss);

    // store
    std::pair<double,double> oneHit;
    oneHit.first = position; oneHit.second = energyloss;
    trfHitRecord.push_back(oneHit);

    // procede to next slice
    index_start = index_end + 1;
    index_end = index_start + unitlength - 1;

    if(index_start > (int)(rawHitRecord.size()-1)){
      break;
    }

    if(index_end > (int)(rawHitRecord.size()-1)){
      index_end = rawHitRecord.size()-1;
    }
  }
  return trfHitRecord;
}
