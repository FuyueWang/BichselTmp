/**
 *  Author:
 *    
 *
 *  allpix Authors:
 *   John Idarraga <idarraga@cern.ch>
 *   Mathieu Benoit <benoit@lal.in2p3.fr>
 */

#ifndef MyDigit_h
#define MyDigit_h 1

#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include "G4VDigi.hh"
/**
 *  Digit My class.
 */
class MyDigi : public G4VDigi {

public:
  MyDigi();
  ~MyDigi();

  MyDigi(const MyDigi&);
  const MyDigi& operator=(const MyDigi&);
  int operator==(const MyDigi&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw();
  void Print();

private:
  
  G4int pixelIDX;    
  G4int pixelIDY,clusterx,clustery,clustersize,Bichselclusterx,Bichselclustery,Bichselclustersize;    
  G4int pixelCounts,Geanteff,Bichseleff;
  G4double pixelEnergyDep,BichselpixelEnergyDep; // MC // Corrected MC charge (detector effects included, at Digi step)
  G4ThreeVector primaryVertex;
  G4double posx,posy,posz,truthposx,truthposy;
  std::vector<G4double> pixeltot,Bichselpixeltot;
public:
  
  inline void SetPixelIDX(G4int pidX)   {pixelIDX = pidX;};
  inline void SetPixelIDY(G4int pidY)   {pixelIDY = pidY;};
  inline void SetPixelCounts(G4int pcs)  {pixelCounts = pcs;};
  inline void SetPixelEnergyDep(G4double ed)  {pixelEnergyDep = ed;}; // MC // Corrected MC charge (detector effects included, at Digi step)
  inline void SetBichselPixelEnergyDep(G4double ed)  {BichselpixelEnergyDep = ed;}; // MC // Corrected MC charg
  inline void SetPrimaryVertex(G4ThreeVector pv)  {primaryVertex = pv;}; // MC vertex //
  inline void Set_posX_WithRespectoToPixel(G4double pos) { posx=pos;}
  inline void Set_posY_WithRespectoToPixel(G4double pos) { posy=pos;}
  inline void Set_posZ_WithRespectoToPixel(G4double pos) { posz=pos;}
  inline void SetTruthPosX(G4double pos){ truthposx=pos;}
  inline void SetTruthPosY(G4double pos){ truthposy=pos;}
  inline void SetPixelToT(std::vector<G4double> pixel){pixeltot=pixel;}
  inline void SetClusterx(G4int x){clusterx=x;}
  inline void SetClustery(G4int y){clustery=y;}
  inline void SetClustersize(G4int size){clustersize=size;}

  inline void SetBichselPixelToT(std::vector<G4double> pixel){Bichselpixeltot=pixel;}
  inline void SetBichselClusterx(G4int x){Bichselclusterx=x;}
  inline void SetBichselClustery(G4int y){Bichselclustery=y;}
  inline void SetBichselClustersize(G4int size){Bichselclustersize=size;}
  inline void SetBichselEff(G4int eff){Bichseleff=eff;}
  inline void SetGeant4Eff(G4int eff){Geanteff=eff;}


  inline void IncreasePixelCounts()     {pixelCounts++;};

  inline G4int GetPixelIDX()   {return pixelIDX;};
  inline G4int GetPixelIDY()   {return pixelIDY;};
  inline G4int GetPixelCounts()  {return pixelCounts;};
  inline G4double GetPixelEnergyDep()  {return pixelEnergyDep;}; // MC //
  inline G4double GetBichselPixelEnergyDep()  {return BichselpixelEnergyDep;}; // MC //

  inline G4ThreeVector GetPrimaryVertex()  {return primaryVertex;}; // MC //
  inline G4double Get_posX_WithRespectoToPixel() {return posx;}
  inline G4double Get_posY_WithRespectoToPixel() {return posy;}
  inline G4double Get_posZ_WithRespectoToPixel() {return posz;}
  inline G4double GetTruthPosX(){return truthposx;}
  inline G4double GetTruthPosY(){return truthposy;}
  inline std::vector<G4double> GetPixelToT(){return pixeltot;}
  inline G4int GetClusterx(){return clusterx;}
  inline G4int GetClustery(){return clustery;}
  inline G4int GetClustersize(){return clustersize;}

  inline std::vector<G4double> GetBichselPixelToT(){return Bichselpixeltot;}
  inline G4int GetBichselClusterx(){return Bichselclusterx;}
  inline G4int GetBichselClustery(){return Bichselclustery;}
  inline G4int GetBichselClustersize(){return Bichselclustersize;}
  inline G4int GetBichselEff(){return Bichseleff;}
  inline G4int GetGeant4Eff(){return Geanteff;}


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

typedef G4TDigiCollection<MyDigi> MyDigitsCollection;

extern G4ThreadLocal G4Allocator<MyDigi> *MyDigiAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void* MyDigi::operator new(size_t)
{
  // void* aDigi;
  // aDigi = (void*) MyDigitAllocator.MallocSingle();
  // return aDigi;
  if (!MyDigiAllocator)
    MyDigiAllocator = new G4Allocator<MyDigi>;
  return (void*) MyDigiAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void MyDigi::operator delete(void* aDigi)
{
  MyDigiAllocator->FreeSingle((MyDigi*) aDigi);
}

#endif

