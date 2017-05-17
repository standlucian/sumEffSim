//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1DetectorConstruction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4Material.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"
#include "G4PhysicalConstants.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 20*cm;
  G4double world_sizeZ  = 80*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
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
  // Shape 1
  //  
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_Ge");
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Al");
  G4Material* shape4_mat = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* shape3_mat = nist->FindOrBuildMaterial("G4_MYLAR");
        
  // Ortec detector
 G4VSolid* ABS_cylinder = new G4Tubs ( "ABS_Cylinder", 0.*mm, (65.9-1.4)/2.*mm, 64.8/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* ABS_cylinder2 = new G4Tubs ( "ABS_Cylinder2", 0.*mm, 8.8/2.*mm, 56.825131583/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* ABS_cylinder3 = new G4Tubs ( "ABS_Cylinder3", 0.*mm, 49.9/2.*mm, 7.3/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* ABS_sphere = new G4Sphere( "ABS_Sphere", 0.*mm, 5.*mm, 0.*deg, 360.*deg, 0.*deg, 61.642363424*deg );
          
 G4VSolid* ABS_torus = new  G4Torus( "ABS_Torus", 0.*mm, (8.-0.7)*mm, 49.9/2*mm, 0.*deg, 360.*deg);

 G4VSolid* ABS_cf = new G4UnionSolid ("ABS_cf", ABS_cylinder2, ABS_sphere, 0, G4ThreeVector(0.,0.,25.787434209*mm));

 G4VSolid* ABS_add1 = new G4UnionSolid ("ABS_add1", ABS_cylinder, ABS_cylinder3, 0, G4ThreeVector(0.,0.,36.05*mm));

 G4VSolid* ABS_add2 = new G4UnionSolid ("ABS_add2", ABS_add1, ABS_torus, 0, G4ThreeVector(0.,0.,32.41*mm));

 G4VSolid* ABS_subst1 = new G4SubtractionSolid ("ABS_subst1", ABS_add2, ABS_cf, 0, G4ThreeVector(0.,0.,-3.75*mm));

 G4LogicalVolume* logicGe1  = new G4LogicalVolume (ABS_subst1, shape1_mat, "logicGe1");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicGe1, "logicGe1", logicWorld, false, 0, checkOverlaps);

   // Ortec cold finger

 /*G4LogicalVolume* logicCf  = new G4LogicalVolume (ABS_cf, shape4_mat, "logicCf");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,-3.75), logicCf, "logicCf", logicWorld, false, 0, checkOverlaps);*/

   // Ortec outer dead layer

 G4VSolid* ABS_cylinder4 = new G4Tubs ( "ABS_Cylinder4", 64.5/2*mm, 65.9/2.*mm, 72.2/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* ABS_cylinder5 = new G4Tubs ( "ABS_Cylinder5", 4.4*mm, 65.9/2.*mm, 0.7/2.*mm, 0.*deg, 360.*deg );
 
 G4VSolid* ABS_cylinder6 = new G4Tubs ( "ABS_Cylinder6", 0*mm, 65.9/2.*mm, 0.7/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* ABS_add3 = new G4UnionSolid ("ABS_add3", ABS_cylinder4, ABS_cylinder5, 0, G4ThreeVector(0.,0.,-36.45*mm));

 G4VSolid* ABS_add4 = new G4UnionSolid ("ABS_add4", ABS_add3, ABS_cylinder6, 0, G4ThreeVector(0.,0.,36.45*mm));
 
 G4LogicalVolume* logicGe2  = new G4LogicalVolume (ABS_add4, shape1_mat, "logicGe2"); 

 new G4PVPlacement(0, G4ThreeVector(0.,0.,3.7), logicGe2, "logicGe2", logicWorld, false, 0, checkOverlaps); 

  // Ortec entrance windows

 G4VSolid* Al_cylinder1 = new G4Tubs ( "Al_cylinder1", 0.*mm, 68.9/2.*mm, 0.03/2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicAl1  = new G4LogicalVolume (Al_cylinder1, shape2_mat, "logicAl1");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,40.915*mm), logicAl1, "logicAl1", logicWorld, false, 0, checkOverlaps);  

  G4VSolid* My_cylinder = new G4Tubs ( "My_cylinder", 0.*mm, 68.9/2.*mm, 0.03/2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicMy  = new G4LogicalVolume (My_cylinder, shape3_mat, "logicMy");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,40.945*mm), logicMy, "logicMy", logicWorld, false, 0, checkOverlaps);

   // Ortec holder

 G4VSolid* Al_cylinder2 = new G4Tubs ( "Al_cylinder2", 68.1/2.*mm, 68.9/2.*mm, 88.5/2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicAl2  = new G4LogicalVolume (Al_cylinder2, shape2_mat, "logicAl2");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,-3.85*mm), logicAl2, "logicAl2", logicWorld, false, 0, checkOverlaps);  

 G4VSolid* Al_cylinder3 = new G4Tubs ( "Al_cylinder3", 8.8/2.*mm, 68.9/2.*mm, 3/2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicAl3  = new G4LogicalVolume (Al_cylinder3, shape2_mat, "logicAl3");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,-49.6*mm), logicAl3, "logicAl3", logicWorld, false, 0, checkOverlaps); 

 // Ortec endcap

 G4VSolid* Al_cylinder4 = new G4Tubs ( "Al_cylinder4", 0.*mm, 81.5/2.*mm, 1./2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicAl4  = new G4LogicalVolume (Al_cylinder4, shape2_mat, "logicAl4");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,44.96*mm), logicAl4, "logicAl4", logicWorld, false, 0, checkOverlaps); 

 G4VSolid* Al_cylinder5 = new G4Tubs ( "Al_cylinder5", 80.5/2*mm, 81.5/2.*mm, 100./2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicAl5  = new G4LogicalVolume (Al_cylinder5, shape2_mat, "logicAl5");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,-5.54*mm), logicAl5, "logicAl5", logicWorld, false, 0, checkOverlaps); 

   // Canberra detector

 G4VSolid* Ca_cylinder = new G4Tubs ( "Ca_cylinder", 0*mm, 63.4/2.*mm, 63.4/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* Ca_cylinder2 = new G4Tubs ( "Ca_cylinder2", 0*mm, 9.5/2.*mm, 36.2/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* Ca_subst1 = new G4SubtractionSolid ("Ca_subst1", Ca_cylinder, Ca_cylinder2, 0, G4ThreeVector(0.,0.,13.6*mm));

 G4LogicalVolume* logicGe3  = new G4LogicalVolume (Ca_subst1, shape1_mat, "logicGe3");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,93.71*mm), logicGe3, "logicGe3", logicWorld, false, 0, checkOverlaps);

 // Canberra dead layer

 G4VSolid* Ca_cylinder3 = new G4Tubs ( "Ca_cylinder3", 63.4/2*mm, 65./2.*mm, 63.4/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* Ca_cylinder4 = new G4Tubs ( "Ca_cylinder4", 0*mm, 65./2.*mm, 0.8/2.*mm, 0.*deg, 360.*deg );
 
 G4VSolid* Ca_cylinder5 = new G4Tubs ( "Ca_cylinder5", 9.5/2*mm, 65./2.*mm, 0.8/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* Ca_add1 = new G4UnionSolid ("Ca_add1", Ca_cylinder3, Ca_cylinder4, 0, G4ThreeVector(0.,0.,-32.1*mm));

 G4VSolid* Ca_add2 = new G4UnionSolid ("Ca_add2", Ca_add1, Ca_cylinder5, 0, G4ThreeVector(0.,0.,32.1*mm));

 G4LogicalVolume* logicDL  = new G4LogicalVolume (Ca_add2, shape1_mat, "logicDL");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,93.71*mm), logicDL, "logicDL", logicWorld, false, 0, checkOverlaps); 

 // Canberra cold finger

 /*G4VSolid* Ca_cf = new G4Tubs ( "Ca_cf", 0.*mm, 9.5/2.*mm, 37./2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicCF2  = new G4LogicalVolume (Ca_cf, shape4_mat, "logicCF2");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,107.71*mm), logicCF2, "logicCF2", logicWorld, false, 0, checkOverlaps); */

 // Canberra endcap

 G4VSolid* Ca_endcap = new G4Tubs ( "Ca_endcap", 0.*mm, 73./2.*mm, 1.5/2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicEC  = new G4LogicalVolume (Ca_endcap, shape2_mat, "logicCF2");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,55.46*mm), logicEC, "logicEC", logicWorld, false, 0, checkOverlaps); 

 G4VSolid* Ca_cover = new G4Tubs ( "Ca_cover", 71.5/2.*mm, 73./2.*mm, 100/2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicCV  = new G4LogicalVolume (Ca_cover, shape2_mat, "logicCF2");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,106.21*mm), logicCV, "logicCV", logicWorld, false, 0, checkOverlaps);

 // Canberra holder

 G4VSolid* Ch_cylinder = new G4Tubs ( "Ch_cylinder", 65./2*mm, 65.8/2.*mm, 73.5/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* Ch_cylinder1 = new G4Tubs ( "Ch_cylinder1", 11./2*mm, 65.8/2.*mm, 3./2.*mm, 0.*deg, 360.*deg );

 G4VSolid* Ch_add1 = new G4UnionSolid ("Ch_add1", Ch_cylinder, Ch_cylinder1, 0, G4ThreeVector(0.,0.,38.25*mm));

 G4VSolid* Ch_cylinder2 = new G4Tubs ( "Ch_cylinder2", 11./2*mm, 15./2.*mm, 10./2.*mm, 0.*deg, 360.*deg );

 G4VSolid* Ch_add2 = new G4UnionSolid ("Ch_add2", Ch_add1, Ch_cylinder2, 0, G4ThreeVector(0.,0.,44.75*mm));

 G4VSolid* Ch_cylinder3 = new G4Tubs ( "Ch_cylinder3", 0*mm, 15./2.*mm, 7.5/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* Ch_add3 = new G4UnionSolid ("Ch_add3", Ch_add2, Ch_cylinder3, 0, G4ThreeVector(0.,0.,53.5*mm));

 G4VSolid* Ch_cylinder4 = new G4Tubs ( "Ch_cylinder4", 65.8/2*mm, 67.3/2.*mm, 1.3/2.*mm, 0.*deg, 360.*deg );

 G4VSolid* Ch_add4 = new G4UnionSolid ("Ch_add4", Ch_add3, Ch_cylinder4, 0, G4ThreeVector(0.,0.,-34.1*mm));

 G4LogicalVolume* logicCh1  = new G4LogicalVolume (Ch_add4, shape2_mat, "logicCh1");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,100.26*mm), logicCh1, "logicCh1", logicWorld, false, 0, checkOverlaps); 

 G4VSolid* Ch_cylinder5 = new G4Tubs ( "Ch_cylinder5", 65.8/2.*mm, 67.3/2.*mm, 8.6/2.*mm, 0.*deg, 360.*deg );

 G4LogicalVolume* logicCh2  = new G4LogicalVolume (Ch_cylinder5, shape2_mat, "logicCh2");

 new G4PVPlacement(0, G4ThreeVector(0.,0.,77.51*mm), logicCh2, "logicCh2", logicWorld, false, 0, checkOverlaps);

 new G4PVPlacement(0, G4ThreeVector(0.,0.,105.51*mm), logicCh2, "logicCh2", logicWorld, false, 0, checkOverlaps);


fScoringVolume = logicGe1;

fScoringVolume = logicGe3;
          
                                 
  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  // 
  // Scorers
  //

  // declare Absorber as a MultiFunctionalDetector scorer
  //  
  G4MultiFunctionalDetector* absDetector 
    = new G4MultiFunctionalDetector("Absorber");

  G4VPrimitiveScorer* primitive;
  primitive = new G4PSEnergyDeposit("Edep");
  absDetector->RegisterPrimitive(primitive);


  SetSensitiveDetector("logicGe1",absDetector);

G4MultiFunctionalDetector* absDetector2 
    = new G4MultiFunctionalDetector("Absorber2");

  primitive = new G4PSEnergyDeposit("Edep");
  absDetector2->RegisterPrimitive(primitive);


  SetSensitiveDetector("logicGe3",absDetector2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
