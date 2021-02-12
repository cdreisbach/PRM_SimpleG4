#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4Material;
class G4Box;
class G4Tubs;
class G4Region;
class DetectorMessenger;
class G4LogicalVolume;
class G4PVPlacement;
class DetectorConstruction: public G4VUserDetectorConstruction
{
	public:
		DetectorConstruction();
		~DetectorConstruction();

		virtual G4VPhysicalVolume* Construct();

		double       getRadius()       const { return this->_radius;       }
		double       getLength()       const { return this->_length;       }
		double       getPressure()     const { return this->_pressure;     }
		std::string  getMaterialName() const { return this->_materialName; }
		G4Material*  getMaterial()     const { return this->_material;     }
		unsigned int getPadPlaneId()   const { return this->_padPlaneId;   }

		void setPressure(double value);
		void setPadPlaneId(unsigned int id);

		double getWorldSizeX() const { return this->_worldSizeX; }
		double getWorldSizeY() const { return this->_worldSizeY; }
		double getWorldSizeZ() const { return this->_worldSizeZ; }

		double getTargetSizeX() const {return this->_targetSizeX; }
		double getTargetSizeY() const {return this->_targetSizeY; }
		double getTargetSizeZ() const {return this->_targetSizeZ; }

		void setPositionZ( double positionZ );

	private:
		G4LogicalVolume* _constructTPC();
		G4VPhysicalVolume* _constructVolumes();
		void _defineMaterials();
		void _setMaterial();
		unsigned int _padPlaneId;
		double _radius;
		double _length;
		double _pressure;
		double _worldSizeX;
		double _worldSizeY;
		double _worldSizeZ;
		double _targetSizeX;
		double _targetSizeY;
		double _targetSizeZ;
		double _positionZ;

		std::string _materialName;

		DetectorMessenger* _detectorMessenger;
		G4Material*        _material;
		G4Region*          _targetRegion;
		G4LogicalVolume*   _worldLogicalVolume;
		G4PVPlacement*     _worldPlacement;
		G4Box*             _world;
};

#endif

