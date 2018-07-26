

#ifndef G4DetectorGeometry_h
#define G4DetectorGeometry_h 1

#include <globals.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4Element.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PhantomParameterisation.hh>
#include <G4PVParameterised.hh>

#include <G4Tubs.hh>
#include <G4Box.hh>


class G4DetectorGeometry : public G4VUserDetectorConstruction
{
  public:
		
		G4DetectorGeometry();
   ~G4DetectorGeometry();

		
		void SetConfig();

	public:
	
		G4VPhysicalVolume* Construct();
    
		void UpdateGeometry();
		bool GetUpdated(){return updated;}
		void SetUpdated(bool choice){updated= choice;}

	private:

		void DefineMaterials();
		G4VPhysicalVolume* BuildDetectorGeometry();

	private: 

		//Elements
		G4Element* H;
    G4Element* B;
		G4Element* C;
		G4Element* N;
		G4Element* O;
		G4Element* Na;
		G4Element* Fe;
		G4Element* Mg;
		G4Element* Mn;
		G4Element* Ca;
		G4Element* K;
		G4Element* P;
		G4Element* Cl;
		G4Element* Lu;
		G4Element* Si;  
		G4Element* Y;
		G4Element* Ce;

		//radioactive
		G4Element* Tc;
		G4Element* Pm;
		G4Element* Po;
		G4Element* At;
		G4Element* Rn;
		G4Element* Fr;
		G4Element* Ra;
		G4Element* Ac;
		G4Element* Th;
		G4Element* Pa;
		G4Element* U;
		G4Element* Np;
		G4Element* Pu;
		G4Element* Am;
		G4Element* Cm;
		G4Element* Bk;
		G4Element* Cf;
		
		//radioactive materials
		G4Material* Technetium;
		G4Material* Promethium;
		G4Material* Polonium;
		G4Material* Astatine;
		G4Material* Radon;
		G4Material* Francium;
		G4Material* Radium;
		G4Material* Actinium;
		G4Material* Thorium;
		G4Material* Protactinium;
		G4Material* Uranium;
		G4Material* Neptunium;
		G4Material* Plutonium;
		G4Material* Americium;
		G4Material* Curium;
		G4Material* Berkelium;
		G4Material* Californium;

		//common materials
		G4Material* Iron;
		G4Material* Air;
		G4Material* Water;
  	G4Material* Vacuum;
  	G4Material* Aluminium;
		G4Material* Silicon;
		G4Material* Copper;
		G4Material* Nickel;
		G4Material* Zinc;
		G4Material* Lead;
		G4Material* Tungsten;
		G4Material* Silver;
		G4Material* Platinum;
		G4Material* Gold;
		G4Material* Concrete;
		G4Material* Tin;
		G4Material* PVC;

		//Shape
		G4Box* fExpHall_box;
		G4Box* fContainer_box;
		G4Box* fVoxelEnsembleContainer_box;
		G4Box* fVoxel_box;

		//logical
		G4LogicalVolume* fExpHall_log;
		G4LogicalVolume* fContainer_log;
		G4LogicalVolume* fVoxelEnsembleContainer_log;
		G4LogicalVolume* fVoxel_log;

		//phys
		G4VPhysicalVolume* fExpHall_phys;
		G4PhantomParameterisation* fPhantomParameterisation;
		G4VPhysicalVolume* fContainer_phys;
		G4VPhysicalVolume* fVoxelEnsembleContainer_phys;
		G4PVParameterised* fVoxelEnsemble_phys;

		//vis attr.
		G4VisAttributes* fExpHall_va;


		//world
    double fExpHallSizeX;
    double fExpHallSizeY;
    double fExpHallSizeZ;
		
		//container	
		double fContainerSizeX;
		double fContainerSizeY;
		double fContainerSizeZ;
		double fVoxelSizeX;
		double fVoxelSizeY;
		double fVoxelSizeZ;
		int fNVoxelX;
		int fNVoxelY;
		int fNVoxelZ;

		bool updated;

};

#endif
