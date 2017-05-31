/**
 *  Author:
 *    
 *
 *  allpix Authors:
 *   John Idarraga <idarraga@cern.ch>
 *   Mathieu Benoit <benoit@lal.in2p3.fr>
 */

#include "MyDigitizer.hh"
#include "MyTrackerHit.hh"
#include "G4DigiManager.hh"
#include "G4VDigitizerModule.hh"
#include "DetectorConstruction.hh"
#include "MyBichselTool.hh"

#include "TMath.h"
#include "TF1.h"
#include "TROOT.h"
#include "TRandom.h"
#include <vector>

MyDigitizer::MyDigitizer(const G4String& modName,const G4String& digsCollectionName) 
  : G4VDigitizerModule (modName) {

  collectionName.push_back(digsCollectionName);
  digitizerName=modName;
  //Geometory parameter
  DetectorConstruction* det=new DetectorConstruction;
  m_hitsColName=det->GetHitCollectionName();

  // Registration of digits collection name
  //constants
  elec=3.6;//eV
  noise=0*elec;//100
  diff=0.007;//for 0.3 mm
  Nbofbit=4;

  //digitize过程标志
  doDiff = true;
  deltaray = false;
  //pixel

  //Bichsel
  doBichsel=false;
  if(doBichsel==true) Bichsel.Initilize();

}

MyDigitizer::~MyDigitizer(){

}

void MyDigitizer::Digitize(){
 
  // create the digits collection
  digitsCollection = new MyDigitsCollection(digitizerName, collectionName[0] );
  // get the digiManager
  G4DigiManager * digiMan = G4DigiManager::GetDMpointer();

  // BoxSD_0_HitsCollection
  G4int hcID = digiMan->GetHitsCollectionID(m_hitsColName);

  MyTrackerHitsCollection* hitsCollection = (MyTrackerHitsCollection*)(digiMan->GetHitsCollection(hcID));

  threshold=(*hitsCollection)[0]->GetDigitizerThreshold()*elec;
  pixelx=(*hitsCollection)[0]->GetPixelsize().x();//pixelsize.x();  pixely=pixelsize.y();  pixelz=pixelsize.z();
  pixely=(*hitsCollection)[0]->GetPixelsize().y();
  pixelz=(*hitsCollection)[0]->GetPixelsize().z();
  // G4cout<<"Digitizer x="<<(*hitsCollection)[0]->GetPixelsize().x()<<" y="<<(*hitsCollection)[0]->GetPixelsize().y()<<" z="<<(*hitsCollection)[0]->GetPixelsize().z()<<" thre="<<threshold/elec<<G4endl; 
  G4int nEntries = hitsCollection->entries();
  G4int deltaEntries=0;
  G4double energy=0.;
  for(G4int itr  = 0 ; itr < nEntries ; itr++) {
    if((*hitsCollection)[itr]->GetTrackID()>1&&!deltaray) continue;
    energy+=(*hitsCollection)[itr]->GetEdep();
  }
  // G4cout<<"Digitizer pixel="<<(*hitsCollection)[0]->GetPixelsize()<<G4endl;

  G4ThreeVector* dir=new G4ThreeVector((*hitsCollection)[0]->GetPrePos().x()-PrimaryVertex->GetX0(),(*hitsCollection)[0]->GetPrePos().y()-PrimaryVertex->GetY0(),0-PrimaryVertex->GetZ0());
  Double_t tanthe=dir->z()/dir->y();
  Double_t eta=-log((sqrt(1+tanthe*tanthe)-1)/tanthe);
  Double_t fai=atan(dir->x()/dir->z())/3.1415926*180;
  // G4cout<<"Digitizer vertex="<<PrimaryVertex->GetX0()<<" very="<<PrimaryVertex->GetY0()<<" verz="<<PrimaryVertex->GetZ0()<<" prepos="<<(*hitsCollection)[0]->GetPrePos()<<" eta="<<eta<<" fai="<<fai<<G4endl;
  centerpixelshiftx=0;centerpixelshifty=0;
  if(eta<0.5){
    pixelsizex=5;
    pixelsizey=5;
  }
  else if(eta>0.5&&eta<1.5){
    pixelsizex=7;
    pixelsizey=11;
    centerpixelshifty=2;
  }
  else{
    pixelsizex=7;
    pixelsizey=19;
    centerpixelshifty=5;
  }


  std::vector<G4double> Pixel;
  G4int clusterx,clustery,clustersize;
  G4int isGeant4onecluster=0;
  isGeant4onecluster=ChargetoToT("Geant4",&Pixel,&clusterx,&clustery,&clustersize);//,threshold,pixelsize
 
  MyDigi * digit = new MyDigi();
  // if(isBichselonecluster==1) digit->SetBichselEff(1);
  // else if(isBichselonecluster==2) digit->SetBichselEff(0);
  // else digit->SetBichselEff(99);
  // if(isGeant4onecluster==1) digit->SetGeant4Eff(1);
  // else if(isGeant4onecluster==2) digit->SetGeant4Eff(0);
  // else digit->SetGeant4Eff(99);
  if(isGeant4onecluster==1){//Fill Digit
    digit->SetPixelToT(Pixel);
    digit->SetClusterx(clusterx);
    digit->SetClustery(clustery);
    digit->SetClustersize(clustersize);
    digit->SetPixelEnergyDep(energy);
    digit->SetTruthPosX((*hitsCollection)[0]->GetPosWithRespectToPixel().x());
    digit->SetTruthPosY((*hitsCollection)[0]->GetPosWithRespectToPixel().y());
  }
  else{
    digit->SetPixelToT(Pixel);
    digit->SetClusterx(0);
    digit->SetClustery(0);
    digit->SetClustersize(0);
    digit->SetTruthPosX(0);
    digit->SetTruthPosY(0);
    digit->SetPixelEnergyDep(0);
  }
  digitsCollection->insert(digit);
  // G4int dc_entries = digitsCollection->entries();
  // if(dc_entries > 0){
  //   G4cout << "--------> Digits Collection : " << collectionName[0]
  //   << "(" << m_hitsColName << ")"
  //   << " contains " << dc_entries
  //   << " digits" << G4endl;
  
  StoreDigiCollection(digitsCollection);
}



G4int MyDigitizer::ChargetoToT(G4String whichmodel,std::vector<G4double>* Pixel,Int_t* clusterx,Int_t *clustery,Int_t *clustersize){
  G4int nEntries;
  G4double posx,posy,posz,eHitTotal;
  // G4cout<<"digitiz "<<whichmodel<<G4endl;
  G4DigiManager * digiMan = G4DigiManager::GetDMpointer();
  G4int hcID = digiMan->GetHitsCollectionID(m_hitsColName);
  MyTrackerHitsCollection * hitsCollection =(MyTrackerHitsCollection*)(digiMan->GetHitsCollection(hcID));

  if(whichmodel=="Bichsel") nEntries=BichselhitsCollection.size();
  else nEntries = hitsCollection->entries();
  // G4cout<<"Hits collection="<<nEntries<<G4endl;
  //temporary data structure
  map<pair<G4int, G4int>, G4double > pixelsContent;
  pair<G4int, G4int> extraPixel;
  G4int centerpixelx,centerpixely,precision=1;
  centerpixelx=(*hitsCollection)[0]->GetPixelNbX()+centerpixelshiftx;
  centerpixely=(*hitsCollection)[0]->GetPixelNbY()+centerpixelshifty;
  for(G4int itr  = 0 ; itr < nEntries ; itr++) {
    if(whichmodel=="Geant4"&&(*hitsCollection)[itr]->GetTrackID()>1&&deltaray==false) continue;
    if(whichmodel=="Bichsel"){
      eHitTotal=BichselhitsCollection[itr].energy*1e6;//eV
      posx=BichselhitsCollection[itr].pos.x();
      posy=BichselhitsCollection[itr].pos.y();
      posz=BichselhitsCollection[itr].pos.z();
      extraPixel.first  = BichselhitsCollection[itr].PixelNb.first;
      extraPixel.second = BichselhitsCollection[itr].PixelNb.second;
    }
    else{ 
      eHitTotal=(*hitsCollection)[itr]->GetEdep()*1e6;//eV
      posx=(*hitsCollection)[itr]->GetPosWithRespectToPixel().x();
      posy=(*hitsCollection)[itr]->GetPosWithRespectToPixel().y();
      posz=(*hitsCollection)[itr]->GetPosWithRespectToPixel().z();
      extraPixel.first  = (*hitsCollection)[itr]->GetPixelNbX();
      extraPixel.second = (*hitsCollection)[itr]->GetPixelNbY();
    }
    // G4cout<<whichmodel<<" "<<itr<<" z="<<posz<<" energy="<<eHitTotal<<G4endl;
    
    // if(posz<0.001||eHitTotal<0.01){ posz=0.006; eHitTotal=200;}
    for(G4int nQ=0;nQ<precision;nQ++){
      G4double eHit=eHitTotal/(2.*precision);
      for(G4int eholes=0;eholes<2;eholes++){
  	bool isHole=false;
  	if(eholes==1) isHole=true;
  	G4double zposD = posz;
  	if (isHole) zposD = pixelz - posz;
  	G4double rdif=diff*sqrt(zposD/0.3);
  	if (!doDiff) rdif = 0.;
  	gRandom->SetSeed();
  	G4double xposD=posx+rdif*gRandom->Gaus(0,1); 
  	G4double yposD=posy+rdif*gRandom->Gaus(0,1);
  	while (TMath::Abs(xposD) > pixelx/2){
  	  G4double sign = xposD/(TMath::Abs(xposD)); 
  	  extraPixel.first = extraPixel.first + 1*sign;
  	  xposD = xposD - (pixelx*sign); 
  	}
  	while (TMath::Abs(yposD) > pixely/2){
  	  G4double sign = yposD/(TMath::Abs(yposD));
  	  extraPixel.second = extraPixel.second + 1*sign;
  	  yposD = yposD - (pixely*sign); 
  	}
  	pixelsContent[extraPixel] += eHit;// eV
      }
    }  
  }
  // Now create digits, one per pixel
  // Second entry in the map is the energy deposit in the pixel
  map<pair<G4int, G4int>, G4double >::iterator pCItr = pixelsContent.begin();

  std::vector<std::pair<G4int,G4int>> pixelover0,cluster;
  std::pair<G4int,G4int> tempposition,position;
  G4int isonecluster=0;

  G4int maxbit,pixelrangex,pixelrangey;
  G4double tuning,ToT,totsum=0;

  pixelrangex=(G4int)(pixelsizex/2);
  pixelrangey=(G4int)(pixelsizey/2);

  // NOTE that there is a nice interface which provides useful info for hits.
  // For instance, the following member gives you the position of a hit with respect
  //  to the center of the pixel.
  // G4ThreeVector vec = (*hitsCollection)[itr]->GetPosWithRespectToPixel();
  // See class MyTrackerHit !

  // Also, you have full access to the detector geometry from this scope
  // MyGeoDsc * GetDetectorGeoDscPtr()
  // provides you with a pointer to the geometry description.
  // See class MyGeoDsc !
  for (G4int j=-pixelrangey; j<=pixelrangey; j++){
    extraPixel.second =centerpixely+j;
    for (G4int i=-pixelrangex; i<=pixelrangex; i++){
      extraPixel.first =centerpixelx+i;
      pixelsContent[extraPixel]+=noise*gRandom->Gaus(0,1);
      if (pixelsContent[extraPixel] < threshold) ToT=0;
      else{//Pixel[j]>0
  	ToT =1;
  	pixelover0.push_back(extraPixel);
      }
      // G4cout<<whichmodel<<" x="<<extraPixel.first<<" y="<<extraPixel.second<<" charge="<<pixelsContent[extraPixel]<<G4endl;
      Pixel->push_back(ToT);//Pixel[(j+pixelrangey)*pixelsizex+i+pixelrangex]=ToT;
      totsum +=ToT;
    }
  }

  if(totsum!=0){
    cluster.push_back(pixelover0[0]);
    for(G4int k=0;k<cluster.size();k++){
      tempposition=cluster[k];
      for (G4int i=-1; i<=1; i++){
  	position.first = tempposition.first+i;
  	for (G4int j=-1; j<=1; j++){
  	  if(i==0&&j==0) continue;
  	  position.second = tempposition.second+j;
  	  if (pixelsContent[position]>threshold&& IsInVector(cluster,position)==0)
  	    cluster.push_back(position);
  	}
      }
    }
    if(cluster.size()==pixelover0.size()) isonecluster=1;
    else isonecluster=0;
  }  
  else isonecluster=2;//not efficient to detect this event;
  // G4cout<<"clustersize="<<cluster.size()<<" over0="<<pixelover0.size()<<G4endl;
  *clusterx=GetCluster_x(pixelover0);
  *clustery=GetCluster_y(pixelover0);
  *clustersize=cluster.size();
  return isonecluster;
}
      



G4int MyDigitizer::GetCluster_x (std::vector<std::pair<G4int,G4int>> cluster){
  G4int lowest = 10000;
  G4int highest = -1;
  for (G4int i=0; i<cluster.size(); i++){
    if (cluster[i].first > highest) highest = cluster[i].first;
    if (cluster[i].first < lowest) lowest = cluster[i].first;
  }
  return highest - lowest + 1;
}

G4int MyDigitizer::GetCluster_y (std::vector<std::pair<G4int,G4int>> cluster){
  G4int lowest = 10000;
  G4int highest = -1;
  for (G4int i=0; i<cluster.size(); i++){
    if (cluster[i].second > highest) highest = cluster[i].second;
    if (cluster[i].second < lowest) lowest = cluster[i].second;
  }
  return highest - lowest + 1; 
}

G4int MyDigitizer::IsInVector(std::vector<pair<G4int,G4int>> a,std::pair<G4int,G4int> b){
  G4int flag=0;
  for(G4int i=0;i<a.size();i++)
    if(b==a[i]) flag=1;
  return flag;

}
