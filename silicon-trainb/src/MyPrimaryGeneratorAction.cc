#include "MyPrimaryGeneratorAction.hh"
#include "MyPrimaryGeneratorMessenger.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

//ROOT classes
#include "Rtypes.h"
#include "TFile.h"
#include "Randomize.hh"
#include "TROOT.h"
#include "TBranch.h"
#include "TFile.h"
#include "TF2.h"
#include "TMath.h"
#include "TRandom.h"
MyPrimaryGeneratorAction::MyPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="e-"));
  particleGun->SetParticleEnergy(10.0*GeV);

  m_primarygenMessenger= new MyPrimaryGeneratorMessenger(this);  
  
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
  delete m_primarygenMessenger;
  delete particleGun; 
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{  
  gRandom->SetSeed();
  // G4cout<<"eta="<<eta<<" fai="<<fai<<" pixelx="<<pixelx<<" pixely="<<pixely<<" pixelz="<<pixelz<<G4endl;
  Double_t pi=3.1415926,height=39,smear=0.001,fai_rad,theta_rad,faicentral_rad,thetacentral_rad;//39mm,1mmrad
  //px,py,pz是粒子源所在的位置,ax,ay,az为随机产生的粒子打在探测器上的点的位置,dx,dy,dz是PA方向
  Double_t sourcepos[3],dir[3];
  faicentral_rad=fai/180.*pi;
  thetacentral_rad=atan(2.*exp(-eta)/(1.-exp(-2.*eta)));
  sourcepos[0]=pixelx/2-height*tan(faicentral_rad);
  sourcepos[1]=pixely/2-height/tan(thetacentral_rad);
  sourcepos[2]=-height;

  fai_rad=gRandom->Uniform(faicentral_rad-smear/2.,faicentral_rad+smear/2.);
  theta_rad=gRandom->Uniform(thetacentral_rad-smear/2.,thetacentral_rad+smear/2.);
  // G4cout<<"theta="<<thetacentral_rad<<" theta_rad="<<theta_rad<<" fai="<<fai_rad/pi*180<<G4endl;
  dir[0]=height*tan(fai_rad);
  dir[1]=height/tan(theta_rad);
  dir[2]=height;
  // G4cout<<"Gene primary="<<G4ThreeVector(sourcepos[0]+dir[0],sourcepos[1]+dir[1],sourcepos[2]+dir[2])<<G4endl;
  particleGun->SetParticlePosition(G4ThreeVector(sourcepos[0]*mm,sourcepos[1]*mm,sourcepos[2]*mm));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(dir[0],dir[1],dir[2]));
  particleGun->GeneratePrimaryVertex(anEvent);
}
 
void MyPrimaryGeneratorAction::SetPixelsize(G4ThreeVector pixelsize){
  pixelx=pixelsize.x();
  pixely=pixelsize.y();
  pixelz=pixelsize.z();
}
