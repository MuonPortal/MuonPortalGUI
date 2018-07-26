

#include <G4DetectorGeometry.h>
#include <ConfigParser.h>


#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4NistManager.hh"

#include <TMath.h>

#include <vector>
#include <cstddef>
#include <iostream>
#include <sstream>

using namespace std;


G4DetectorGeometry::G4DetectorGeometry(){

	SetConfig();

}//close constructor

G4DetectorGeometry::~G4DetectorGeometry(){



}//close constructor


void G4DetectorGeometry::SetConfig(){

	fContainerSizeX= ConfigParser::fContainerSizeX*CLHEP::cm;
	fContainerSizeY= ConfigParser::fContainerSizeY*CLHEP::cm;
	fContainerSizeZ= ConfigParser::fContainerSizeZ*CLHEP::cm;
	fVoxelSizeX= ConfigParser::fVoxelSizeX*CLHEP::cm;
	fVoxelSizeY= ConfigParser::fVoxelSizeY*CLHEP::cm;
	fVoxelSizeZ= ConfigParser::fVoxelSizeZ*CLHEP::cm;

	if( fmod(ConfigParser::fContainerSizeX,ConfigParser::fVoxelSizeX)!=0 ||
	    fmod(ConfigParser::fContainerSizeY,ConfigParser::fVoxelSizeY)!=0 ||
			fmod(ConfigParser::fContainerSizeZ,ConfigParser::fVoxelSizeZ)!=0
	){
		cerr<<"G4DetectorGeometry::SetConfig(): ERROR: Container size ("<<ConfigParser::fContainerSizeX<<","<<ConfigParser::fContainerSizeY<<","<<ConfigParser::fContainerSizeZ<<") is not an exact multiple of voxel sizes ("<<ConfigParser::fVoxelSizeX<<","<<ConfigParser::fVoxelSizeY<<","<<ConfigParser::fVoxelSizeZ<<")...exit!"<<endl;
		
		exit(1);
	}
	fNVoxelX = (int)(ConfigParser::fContainerSizeX/ConfigParser::fVoxelSizeX);
	fNVoxelY = (int)(ConfigParser::fContainerSizeY/ConfigParser::fVoxelSizeY);
	fNVoxelZ = (int)(ConfigParser::fContainerSizeZ/ConfigParser::fVoxelSizeZ);
	
	//## Find proper size of world box
	double WorldFactorEnhancement= 2;
	fExpHallSizeX=  WorldFactorEnhancement*ConfigParser::fContainerSizeX*CLHEP::cm;	
	fExpHallSizeY = WorldFactorEnhancement*ConfigParser::fContainerSizeY*CLHEP::cm;	
  fExpHallSizeZ = WorldFactorEnhancement*ConfigParser::fContainerSizeZ*CLHEP::cm;	

}//close G4DetectorGeometry::SetConfig()


void G4DetectorGeometry::DefineMaterials(){

	G4NistManager* man = G4NistManager::Instance();

  double a, z, density;
  double fractionmass;
  int nelements;
  
  int polyPMMA = 1;
  int nC_PMMA = 3+2*polyPMMA;
  int nH_PMMA = 6+2*polyPMMA;

  int polyeth = 1;
  int nC_eth = 2*polyeth;
  int nH_eth = 4*polyeth;

	//##########################################
	//###         ELEMENTS
	//##########################################
  //*** Elements	
  H = new G4Element("H", "H", z=1., a=1.01*CLHEP::g/CLHEP::mole);
	B = new G4Element("B", "B", z=5., a= 10.811*CLHEP::g/CLHEP::mole);	
  C = new G4Element("C", "C", z=6., a=12.01*CLHEP::g/CLHEP::mole);
  N = new G4Element("N", "N", z=7., a= 14.01*CLHEP::g/CLHEP::mole);
  O = new G4Element("O", "O", z=8., a= 16.00*CLHEP::g/CLHEP::mole);
	Na = new G4Element("Na","Na", z=11., a= 22.99*CLHEP::g/CLHEP::mole);	
	Fe = new G4Element("Fe","Fe", z=26., a= 55.8457*CLHEP::g/CLHEP::mole);	
	Mn = new G4Element("Mn","Mn", z=25., a= 54.938049*CLHEP::g/CLHEP::mole);	
	Mg = new G4Element("Mg","Mg", z=12., a= 24.3050*CLHEP::g/CLHEP::mole);	
	Ca = new G4Element("Ca","Ca", z=20., a= 40.078*CLHEP::g/CLHEP::mole);	
	K = new G4Element("K","K", z=19., a= 39.0983*CLHEP::g/CLHEP::mole);	
	P = new G4Element("P","P", z=15., a= 30.973761*CLHEP::g/CLHEP::mole);	
	Cl= new G4Element("Cl","Cl", z=17., a= 35.453*CLHEP::g/CLHEP::mole);
	Lu = new G4Element("Lutetium","Lu", z=71, a=175.01*CLHEP::g/CLHEP::mole);
  Si = new G4Element("Silicon", "Si", z=14, a=28.01*CLHEP::g/CLHEP::mole);
  Y  = new G4Element("Yttrium", "Y", z=39, a=88.9*CLHEP::g/CLHEP::mole);
  Ce = new G4Element("Cerium", "Ce", z=58, a=140*CLHEP::g/CLHEP::mole);


	Tc= man->FindOrBuildElement("G4_Tc");
	Pm= man->FindOrBuildElement("G4_Pc");
	Po= man->FindOrBuildElement("G4_Po");
	At= man->FindOrBuildElement("G4_At");
	Rn= man->FindOrBuildElement("G4_Rn");
	Fr= man->FindOrBuildElement("G4_Fr");
  Ra= man->FindOrBuildElement("G4_Ra");
	Ac= man->FindOrBuildElement("G4_Ac");
	Th= man->FindOrBuildElement("G4_Th");
	Pa= man->FindOrBuildElement("G4_Pa");
	U= man->FindOrBuildElement("G4_U");
	Np= man->FindOrBuildElement("G4_Np");
	Pu= man->FindOrBuildElement("G4_Pu");
	Am= man->FindOrBuildElement("G4_Am");
	Cm= man->FindOrBuildElement("G4_Cm");
	Bk= man->FindOrBuildElement("G4_Bk");
	Cf= man->FindOrBuildElement("G4_Cf");

	//*** Radiactive materials
	Technetium= man->FindOrBuildMaterial("G4_Tc");
	Technetium->SetName("Technetium");
	Promethium= man->FindOrBuildMaterial("G4_Pm");
	Promethium->SetName("Promethium");

	Polonium= man->FindOrBuildMaterial("G4_Po");
	Polonium->SetName("Polonium");
	Astatine= man->FindOrBuildMaterial("G4_At");
	Astatine->SetName("Astatine");
	Radon= man->FindOrBuildMaterial("G4_Rn");
	Radon->SetName("Radon");
	Francium= man->FindOrBuildMaterial("G4_Fr");
	Francium->SetName("Francium");
  Radium= man->FindOrBuildMaterial("G4_Ra");
	Radium->SetName("Radium");
	Actinium= man->FindOrBuildMaterial("G4_Ac");
	Actinium->SetName("Actinium");
	
	Thorium= man->FindOrBuildMaterial("G4_Th");
	Thorium->SetName("Thorium");
	Protactinium= man->FindOrBuildMaterial("G4_Pa");
	Protactinium->SetName("Protactinium");
	Uranium= man->FindOrBuildMaterial("G4_U");	
	Uranium->SetName("Uranium");
	Neptunium= man->FindOrBuildMaterial("G4_Np");
	Neptunium->SetName("Neptunium");
	Plutonium= man->FindOrBuildMaterial("G4_Pu");
	Plutonium->SetName("Plutonium");
	Americium= man->FindOrBuildMaterial("G4_Am");
	Americium->SetName("Americium");
	Curium= man->FindOrBuildMaterial("G4_Cm");
	Curium->SetName("Curium");
	Berkelium= man->FindOrBuildMaterial("G4_Bk");
	Berkelium->SetName("Berkelium");
	Californium= man->FindOrBuildMaterial("G4_Cf");
	Californium->SetName("Californium");


	//** standard materials
	Vacuum = new G4Material("Vacuum",z=1.,a=1.01*CLHEP::g/CLHEP::mole,density=CLHEP::universe_mean_density,kStateGas,0.1*CLHEP::kelvin,1.e-19*CLHEP::pascal);   
  Air = man->FindOrBuildMaterial("G4_AIR");
	Air->SetName("Air");

  Water = new G4Material("Water", density= 1.0*CLHEP::g/CLHEP::cm3, nelements=2);
  Water->AddElement(H, 2);
  Water->AddElement(O, 1);
	Water->SetName("Water");

	PVC= man->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
	PVC->SetName("PVC");

	Aluminium= man->FindOrBuildMaterial("G4_Al");
	Aluminium->SetName("Aluminium");
	Iron= man->FindOrBuildMaterial("G4_Fe");
	Iron->SetName("Iron");
	Silicon= man->FindOrBuildMaterial("G4_Si");
	Silicon->SetName("Silicon");
	Copper= man->FindOrBuildMaterial("G4_Cu");
	Copper->SetName("Copper");
	Nickel= man->FindOrBuildMaterial("G4_Ni");
	Nickel->SetName("Nickel");
	Zinc= man->FindOrBuildMaterial("G4_Zn");
	Zinc->SetName("Zinc");
	Lead= man->FindOrBuildMaterial("G4_Pb");
	Lead->SetName("Lead");
	Tungsten= man->FindOrBuildMaterial("G4_W");
	Tungsten->SetName("Tungsten");
	Silver= man->FindOrBuildMaterial("G4_Ag");
	Silver->SetName("Silver");
	Platinum= man->FindOrBuildMaterial("G4_Pt");
	Platinum->SetName("Platinum");
	Gold= man->FindOrBuildMaterial("G4_Au");
	Gold->SetName("Gold");
	Concrete= man->FindOrBuildMaterial("G4_CONCRETE");
	Concrete->SetName("Concrete");
	Tin= man->FindOrBuildMaterial("G4_Sn");
	Tin->SetName("Tin");

	

}//close G4DetectorGeometry::DefineMaterials()


G4VPhysicalVolume* G4DetectorGeometry::Construct(){

	DefineMaterials();
  return BuildDetectorGeometry();

}//close G4DetectorGeometry::Construct()


G4VPhysicalVolume* G4DetectorGeometry::BuildDetectorGeometry(){

	//## Set parameters from config file
	SetConfig();

	
	//#####################
	//##   World	      ###
	//#####################
  G4Material* worldMaterial= Vacuum;
	//G4Material* worldMaterial= Air;

	fExpHall_box = new G4Box("ExpHall_box",fExpHallSizeX/2,fExpHallSizeY/2,fExpHallSizeZ/2);	
	fExpHall_log= new G4LogicalVolume(fExpHall_box,worldMaterial,"ExpHall_log",0,0,0);
  fExpHall_va = new G4VisAttributes(G4Colour(1,1,1));
  fExpHall_log->SetVisAttributes(G4VisAttributes::Invisible);
  fExpHall_phys= new G4PVPlacement(0,G4ThreeVector(),fExpHall_log,"ExpHall_phys",0,false,0);

	//#####################
	//##   Container	  ###
	//#####################
	std::vector<G4Material*> containerMaterialList;
	//G4Material* containerMaterial= Vacuum;
	G4Material* containerMaterial= Air;
	containerMaterialList.push_back(containerMaterial);

	fContainer_box = new G4Box("Container_box",fContainerSizeX/2.,fContainerSizeY/2.,fContainerSizeZ/2.);
	fContainer_log = new G4LogicalVolume(fContainer_box,containerMaterial,"Container_log",0,0,0);
	fContainer_phys= new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fContainer_log,"Container_phys",fExpHall_log,false,0);
	
	fVoxel_box = new G4Box("Voxel_box",fVoxelSizeX/2,fVoxelSizeY/2,fVoxelSizeZ/2);
  fVoxel_log = new G4LogicalVolume(fVoxel_box,containerMaterial,"Voxel_log",0,0,0);

	fVoxelEnsembleContainer_box = new G4Box("VoxelEnsembleContainer_box",fContainerSizeX/2.,fContainerSizeY/2.,fContainerSizeZ/2.);
	fVoxelEnsembleContainer_log = new G4LogicalVolume(fVoxelEnsembleContainer_box,containerMaterial,"VoxelEnsembleContainer_log",0,0,0);
	fVoxelEnsembleContainer_phys= new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fVoxelEnsembleContainer_log,"VoxelEnsembleContainer_phys",fContainer_log,false,0);

	fPhantomParameterisation = new G4PhantomParameterisation();
	fPhantomParameterisation->SetVoxelDimensions(fVoxelSizeX/2,fVoxelSizeY/2,fVoxelSizeZ/2);
	fPhantomParameterisation->SetNoVoxel(fNVoxelX,fNVoxelY,fNVoxelZ);	
	fPhantomParameterisation->SetMaterials(containerMaterialList);
	fPhantomParameterisation->BuildContainerSolid(fVoxelEnsembleContainer_phys);
	fPhantomParameterisation->CheckVoxelsFillContainer( 
				fVoxelEnsembleContainer_box->GetXHalfLength(),	
				fVoxelEnsembleContainer_box->GetYHalfLength(),
				fVoxelEnsembleContainer_box->GetZHalfLength() 
	);	
	fPhantomParameterisation->SetSkipEqualMaterials(0);
	

	fVoxelEnsemble_phys = new G4PVParameterised("VoxelEnsemble_phys", // name
			fVoxel_log, // logical volume
			fVoxelEnsembleContainer_log, // mother volume
			kXAxis, // optimisation hint
			fNVoxelX*fNVoxelY*fNVoxelZ, // number of voxels
			fPhantomParameterisation
	); // parameterisation
	fVoxelEnsemble_phys->SetRegularStructureId(1);
		
	G4VisAttributes* fContainer_va = new G4VisAttributes(G4Colour(0.75,0.75,0.75,0.3));//silver
  fContainer_log->SetVisAttributes(G4VisAttributes::Invisible);
	//fContainer_log->SetVisAttributes(fContainer_va);	
	
	G4VisAttributes* fVoxelEnsembleContainer_va = new G4VisAttributes(G4Colour(0.75,0.75,0.75,0.3));//silver
  //fVoxelEnsembleContainer_log->SetVisAttributes(G4VisAttributes::Invisible);
	fVoxelEnsembleContainer_log->SetVisAttributes(fVoxelEnsembleContainer_va);	

	G4VisAttributes* fVoxel_va= new G4VisAttributes(G4Colour(0.75,0.75,0.75,0.3));//silver
  //fVoxel_log->SetVisAttributes(G4VisAttributes::Invisible);
	fVoxel_log->SetVisAttributes(fVoxel_va);	
	
	return fExpHall_phys;

}//close G4DetectorGeometry::BuildDetectorGeometry()


void G4DetectorGeometry::UpdateGeometry(){

  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();

  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();


  //define new one
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  updated= false;

}//close G4DetectorGeometry::UpdateGeometry()



