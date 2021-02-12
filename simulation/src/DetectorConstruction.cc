#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include <G4ThreeVector.hh>
#include <G4RunManager.hh>
#include <G4NistManager.hh>
#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>
#include <G4ProductionCuts.hh>
#include <G4VisAttributes.hh>
#include <G4GeometryManager.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SolidStore.hh>

DetectorConstruction::DetectorConstruction()
		: G4VUserDetectorConstruction(),
		  _padPlaneId(1),
		  _radius(0),
		  _length(0),
		  _pressure(1),
		  _worldSizeX(0),
		  _worldSizeY(0),
		  _worldSizeZ(0),
		  _targetSizeX(0),
		  _targetSizeY(0),
		  _targetSizeZ(0),
		  _materialName("G4_H"),
		  _detectorMessenger(nullptr)
{
	// Default parameter values
	this->_positionZ    = 0.0 * CLHEP::mm;
	this->_length       = 4*400 * CLHEP::mm;
	this->_pressure     = 20 * CLHEP::STP_Pressure;
	this->_materialName = "G4_H";

	// WORLD VOLUME
	this->_worldSizeX  = 100 * CLHEP::meter;
	this->_worldSizeY  = this->_worldSizeX;
	this->_worldSizeZ  = this->_worldSizeX;

	this->_detectorMessenger = new DetectorMessenger(this);

	this->_defineMaterials();
	this->_setMaterial();
}

void DetectorConstruction::_defineMaterials()
{
	G4NistManager::Instance()->FindOrBuildMaterial(this->_materialName);
}

void DetectorConstruction::_setMaterial()
{
	G4Material* material = G4Material::GetMaterial(this->_materialName);
	if (material != nullptr)
	{
		this->_material = material;
	}
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}
G4VPhysicalVolume* DetectorConstruction::Construct()
{
	return this->_constructVolumes();
}

G4VPhysicalVolume* DetectorConstruction::_constructVolumes()
{
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	this->_world = new G4Box("World",
	                         this->_worldSizeX/2,
	                         this->_worldSizeY/2,
	                         this->_worldSizeZ/2);

	this->_worldLogicalVolume = new G4LogicalVolume(this->_world,
	                                                G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"),
	                                                "World");

	this->_worldLogicalVolume->SetVisAttributes( G4VisAttributes::GetInvisible() );
	this->_worldPlacement = new G4PVPlacement(nullptr,
	                                          G4ThreeVector(),
	                                          "World",
	                                          this->_worldLogicalVolume,
	                                          nullptr,
	                                          false,
	                                          0);

	// Construct segmented TPC volumina
	G4LogicalVolume* tpcVolumeLogical = this->_constructTPC();

	this->_targetRegion    = new G4Region("Target");
	G4ProductionCuts* cuts = new G4ProductionCuts();

	G4double defCut = 1*CLHEP::micrometer;
	cuts->SetProductionCut(defCut,"gamma");
	cuts->SetProductionCut(defCut,"e-");
	cuts->SetProductionCut(defCut,"e+");
	cuts->SetProductionCut(defCut,"proton");

	this->_targetRegion->SetProductionCuts(cuts);
	this->_targetRegion->AddRootLogicalVolume(tpcVolumeLogical);

	return this->_worldPlacement;
}

void DetectorConstruction::setPressure(double value)
{
	this->_pressure = value * CLHEP::STP_Pressure;
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

void DetectorConstruction::setPadPlaneId(unsigned int id)
{
	this->_padPlaneId = id;
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}
void DetectorConstruction::setPositionZ( double positionZ )
{
	this->_positionZ = positionZ * CLHEP::mm;
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

DetectorConstruction::~DetectorConstruction()
{
	delete this->_detectorMessenger;
}

G4LogicalVolume* DetectorConstruction::_constructTPC()
{
	// Construct TPC volume
	G4Tubs* tpcVolume = new G4Tubs("TPCVolume",
	                               0,
	                               300 * CLHEP::mm,
	                               0.5 * this->_length,
	                               0,
	                               CLHEP::twopi);

	// Define material
	this->_material = G4NistManager::Instance()->ConstructNewGasMaterial( "PressurizedGas",
	                                                                      this->_materialName,
	                                                                      CLHEP::STP_Temperature,
	                                                                      this->_pressure  );
	this->_material->GetIonisation()->SetMeanEnergyPerIonPair( this->_material->GetIonisation()
	                                                                          ->GetMeanExcitationEnergy() );

	G4LogicalVolume* tpcVolumeLogical = new G4LogicalVolume(tpcVolume,
	                                                        this->_material,
	                                                        "TPCVolume");

	G4Colour transparentBlue( 0, 0, 255, 0.5 );
	tpcVolumeLogical->SetVisAttributes( G4VisAttributes( transparentBlue ) );
	new G4PVPlacement(nullptr,
	                  G4ThreeVector(0,0,this->_positionZ),
	                  "TPCVolume",
	                  tpcVolumeLogical,
	                  this->_worldPlacement,
	                  false,
	                  0);

	return tpcVolumeLogical;
}
