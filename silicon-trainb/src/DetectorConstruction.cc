#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVDivision.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "MyTrackerSD.hh"
#include "G4UserEventAction.hh"
#include "MyEventAction.hh"
#include "MyDigitizer.hh"
#include "G4DigiManager.hh"
DetectorConstruction::DetectorConstruction():pixelz(0.)
{
  fStepLimit = new G4UserLimits();
  m_detectorMessenger = new DetectorMessenger(this);
  HitCollectionName="TrackerHitsCollection";
  NbofPixelx=20; NbofPixely=100; NbofPixelz=1 ;
  pixelx=50*um;  pixely=50*um; pixelz=50.*um;
  
}

DetectorConstruction::~DetectorConstruction()
{
  delete m_detectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
 
  G4cout<<"pixelx="<<pixelx<<" pixely="<<pixely<<" pixelz="<<pixelz<<G4endl;
  siplatex=(G4double)pixelx*NbofPixelx; 
  siplatey=(G4double)pixely*NbofPixely;
  siplatez=(G4double)pixelz*NbofPixelz;
  G4cout<<"siplatex="<<siplatex<<" siplatey="<<siplatey<<" siplatez="<<siplatez<<G4endl;
  G4cout<<"threshold="<<threshold<<G4endl;
//------------------------------------------------------ elements
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4String name,symbol;

  G4Element* H=new G4Element(name="Hydrogen",symbol="H",z=1.,a=1.01*g/mole);
  G4Element* C=new G4Element(name="Carbon",symbol="C",z=6.,a=12.01*g/mole);
  G4Element* S= new G4Element(name="Sulful" ,symbol="S" , z= 16., a=   32.06*g/mole);
  G4Element* F= new G4Element(name="fluorine" ,symbol="F" , z= 9., a=  18.01*g/mole);
  a = 28.09*g/mole;
  G4Element* Silicon = new G4Element(name="Silicon", symbol="Si", z=14., a);
  
//------------------------------------------------------ materials
  G4double density;
  G4int ncomponents,natoms;
  G4double fractionmass;

 G4NistManager * nistman = G4NistManager::Instance();

  //Vaccum for world
  G4Material* vacuum=new G4Material("Galactic",z=1.,a=1.01*g/mole,density=universe_mean_density,kStateGas,2.73*kelvin,3.e-18*pascal);  

  // density = 2.333*g/cm3;  
  // G4Material* Si = new G4Material(name="Silicon", density, ncomponents=1);
  // Si->AddElement(Silicon, fractionmass=1.);
//geometry parameters
  G4Material * Si = nistman->FindOrBuildMaterial("G4_Si");


  G4double worldx=siplatex*1.2,worldy=siplatey*1.2, worldz=siplatez*25;//0.12*mm
  G4double  ynum=0, xnum=0, number=0;
//------------------------------------------------------ volumes
//experimental hall (world volume)

  G4Box* world=new G4Box("world",worldx/2.,worldy/2.,worldz/2.);
  G4LogicalVolume* world_log=new G4LogicalVolume(world,vacuum,"world");
  G4VPhysicalVolume* world_phys=new G4PVPlacement(0,G4ThreeVector(),world_log,"world",0,false,0);

  world_log->SetVisAttributes(G4VisAttributes::Invisible);

// //a Ar-C2H6 chamber  
//   G4Box* siplate=new G4Box("siplate",siplatex/2.,siplatey/2.,siplatez/2.);
//   G4LogicalVolume* siplate_log=new G4LogicalVolume(siplate,vacuum,"siplate");
//   G4VPhysicalVolume* siplate_phys=new G4PVPlacement(0,G4ThreeVector(0,0,siplatez/2),siplate_log,"siplate",world_log,false,0);

//   G4Box* pixel=new G4Box("pixel",pixelx/2.,pixely/2.,pixelz/2.);
//   G4LogicalVolume* pixel_log=new G4LogicalVolume(pixel,Si,"pixel");
 
 
//   G4double x=0.,y=0.;

//   for(ynum=0;ynum<NbofPixely;ynum++){
//     for(xnum=0;xnum<NbofPixelx;xnum++){
//       number=ynum*NbofPixelx+xnum;//from 0
//       x=-siplatex/2.+pixelx/2.+(G4double)xnum*pixelx;
//       y=siplatey/2.-pixely/2-(G4double)ynum*pixely;
//       new G4PVPlacement(0,
// 			G4ThreeVector(x,y,0),
// 			pixel_log,
// 			"pixel",		
// 		        siplate_log,
// 			false,	
// 			number);
//     }	  
//   }

 
  G4Box * Box_box = new G4Box("Box_box",siplatex/2.,siplatey/2.,siplatez/2.);
  G4LogicalVolume* box_log = new G4LogicalVolume(Box_box,Si,"Box_log");


  G4VPhysicalVolume* box_phys = new G4PVPlacement(
                    0,
                    G4ThreeVector(0,0,siplatez/2.0),
                    box_log,
		    "Box_phys",
		    world_log, // mother log
		    false,
		    0, // copy number
		    true); // check overlap

  G4Box* pixel_box=new G4Box("Pixel_box",pixelx/2.,pixely/2.,pixelz/2.);
  G4LogicalVolume* pixel_log=new G4LogicalVolume(pixel_box,Si,"Pixel_log");

  G4Box* siplate_box=new G4Box("Siplate_box",pixelx/2.,siplatey/2.,siplatez/2.);
  G4LogicalVolume* siplate_log=new G4LogicalVolume(siplate_box,vacuum,"Siplate_log");
 
  new G4PVDivision(//divide Box into slices
		   "Siplate_log",
		   siplate_log,
		   box_log,
		   kXAxis,
		   NbofPixelx,
		   0); // offset
 
  new G4PVDivision(//divide slices into pixel
		   "Pixel_log",
		   pixel_log,
		   siplate_log,
		   kYAxis,
		   NbofPixely,
		   0); 

  G4VisAttributes* pixel_visAttr=new G4VisAttributes(G4Color(1.0,1.0,0.0));
  pixel_visAttr->SetForceSolid(true);
  pixel_log->SetVisAttributes(pixel_visAttr);
  
  pixel_log->SetUserLimits(fStepLimit);

  //Sensitive Detector
  MyTrackerSD* aTrackerSD = new MyTrackerSD("TrackerChamberSD",HitCollectionName,
                                            G4ThreeVector(0,0,0),this);
  SetSensitiveDetector(pixel_log ,  aTrackerSD );

  return world_phys;
}

void DetectorConstruction::SetMaxStepSize(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
  G4cout<<"MaxStep="<<maxStep<<G4endl;
}

void DetectorConstruction::SetDetectorPixelsize(G4ThreeVector pixelsize){
  pixelx=pixelsize.x();
  pixely=pixelsize.y();
  pixelz=pixelsize.z();
}

void DetectorConstruction::SetDigitizerThreshold(G4double thre){
  threshold=thre;
}


void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  // nTPCEventAction * ea = (nTPCEventAction *) G4RunManager::GetRunManager()->GetUserEventAction();
  // ea->SetupDigitizers();
  // MyDigitizer * myDM = new MyDigitizer("MyDigitizer","DigitCollection" );
  // G4DigiManager::GetDMpointer()->AddNewModule(myDM);
  // 			// dmPtr = static_cast<AllPixDigitizerInterface *> (dp);
  // G4cout<<"Update "<<11<<G4endl;
}
