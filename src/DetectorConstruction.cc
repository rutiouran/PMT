#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

DetectorConstruction::DetectorConstruction()
: Galactic(0), Air(0), Bialkali(0)
{}

DetectorConstruction::~DetectorConstruction()
{}

void DetectorConstruction::DefineMaterials()
{
	// Material definition
    G4NistManager* nistManager = G4NistManager::Instance();

    //
    // Define Element
    //
    G4double Cs_Z = 55;     G4double Cs_M = 132.90543*g/mole;   // 1.870*g/cm3
    G4double  K_Z = 19;     G4double  K_M = 39.0983  *g/mole;   // 0.863*g/cm3
    G4double Sb_Z = 51;     G4double Sb_M = 121.760  *g/mole;   // 6.697*g/cm3

    // Define atom
    G4Element* Cs = new G4Element("strontium"   , "Cs", Cs_Z, Cs_M);
    G4Element*  K = new G4Element("kalium"      , "K" ,  K_Z,  K_M);
    G4Element* Sb = new G4Element("antimony"    , "Sb", Sb_Z, Sb_M);

    // Define Material
    Galactic    = nistManager->FindOrBuildMaterial("G4_Galactic");
    Air         = nistManager->FindOrBuildMaterial("G4_AIR");

    Bialkali = new G4Material("CsK2Sb", 1.7295*g/cm3, 3);
    Bialkali->AddElement(Cs, 1);
    Bialkali->AddElement( K, 2);
    Bialkali->AddElement(Sb, 1);

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Define Materials
	DefineMaterials();

	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();
	
	// Envelope parameters
	//
	G4double env_sizeXY = 20.0*cm, env_sizeZ = 20.0*cm;
	
	// Option to switch on/off checking of volumes overlaps
	//
	G4bool checkOverlaps = true;
	
	//
	// World
	//
	G4double world_sizeXY = 1.2*env_sizeXY;
	G4double world_sizeZ  = 1.2*env_sizeZ;
	
	G4Box* solidWorld =
	  new G4Box("World",                       //its name
	     0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
	
	G4LogicalVolume* logicWorld =
	  new G4LogicalVolume(solidWorld,          //its solid
	                      Galactic,            //its material
	                      "World");            //its name
	
	G4VPhysicalVolume* physWorld =
	  new G4PVPlacement(0,                     //no rotation
	                    G4ThreeVector(),       //at (0,0,0)
	                    logicWorld,            //its logical volume
	                    "World",               //its name
	                    0,                     //its mother  volume
	                    false,                 //no boolean operation
	                    0,                     //copy number
	                    checkOverlaps);        //overlaps checking
	
	//
	// Envelope
	//
	G4Box* solidEnv =
	  new G4Box("Envelope",                    //its name
	      0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
	
	G4LogicalVolume* logicEnv =
	  new G4LogicalVolume(solidEnv,            //its solid
	                      Galactic,            //its material
	                      "Envelope");         //its name
	
	new G4PVPlacement(0,                       //no rotation
	                  G4ThreeVector(),         //at (0,0,0)
	                  logicEnv,                //its logical volume
	                  "Envelope",              //its name
	                  logicWorld,              //its mother  volume
	                  false,                   //no boolean operation
	                  0,                       //copy number
	                  checkOverlaps);          //overlaps checking

	//
    // PMT
    //

    // Vacuum tube
    G4Tubs* TubeS
        = new G4Tubs("Tube", 0.0*mm, 18.6/2.0*mm, 88.0/2.0*mm, 0.*deg, 360.*deg);

    G4LogicalVolume* TubeLV
        = new G4LogicalVolume(TubeS, Galactic, "Tube");

    G4VPhysicalVolume* TubePV
        = new G4PVPlacement(
                    0,
                    G4ThreeVector(0, 0, 44.0*mm),
                    TubeLV,
                    "Tube",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);

//	// Photocathode
//  
//	G4Tubs* photocathodeS
//	    = new G4Tubs("Photocathode", 0.0*mm, 15.0/2.0*mm, 40.0/2.0*nm, 0.*deg, 360.*deg);
//	
//	G4LogicalVolume* photocathodeLV
//	    = new G4LogicalVolume(photocathodeS, Bialkali, "Photocathode");
//	
//	G4VPhysicalVolume* photocathodePV
//	    = new G4PVPlacement(
//	                0,
//	                G4ThreeVector(0, 0, ),
//	                );

	//
	//always return the physical World
	//
	return physWorld;
}

}
