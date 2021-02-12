#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
	: G4UImessenger(),
	  fDetector(Det),
	  fTestemDir(nullptr),
	  fDetDir(nullptr),
	  fPressureCmd(nullptr),
	  fPadPlaneCmd(nullptr)
{
	fTestemDir = new G4UIdirectory("/simulation/");
	fTestemDir->SetGuidance("Simulation control.");

	fDetDir = new G4UIdirectory("/simulation/TPC/");
	fDetDir->SetGuidance("TPC Detector construction commands");

	fPressureCmd = new G4UIcmdWithADouble("/simulation/TPC/setPressure",this);
	fPressureCmd->SetGuidance("Select pressure of the gas.");
	fPressureCmd->SetParameterName("pressure",false);
	fPressureCmd->SetRange("pressure>0.");
	fPressureCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	fPressureCmd->SetToBeBroadcasted(false);

	fPadPlaneCmd = new G4UIcmdWithAnInteger("/simulation/TPC/setPadPlaneId",this);
	fPadPlaneCmd->SetGuidance("Set pad plane id");
	fPadPlaneCmd->SetParameterName("padPlaneId",false);
	fPadPlaneCmd->SetRange("padPlaneId>=0");
	fPadPlaneCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	fPadPlaneCmd->SetToBeBroadcasted(false);

	fPositionZCmd = new G4UIcmdWithADoubleAndUnit("/simulation/TPC/setPositionZ",this);
	fPositionZCmd->SetGuidance("Set position z (mm)");
	fPositionZCmd->SetParameterName("positionZ",false);
	fPositionZCmd->SetUnitCategory("Length");
	fPositionZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	fPositionZCmd->SetToBeBroadcasted(false);
}

DetectorMessenger::~DetectorMessenger()
{
	delete fPadPlaneCmd;
	delete fPressureCmd;
	delete fDetDir;
	delete fTestemDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
	// Set TPC Pressure
	if( command == fPressureCmd )
	{
		fDetector->setPressure(fPressureCmd->GetNewDoubleValue(newValue));
	}

	// Set Pad Plane ID
	if( command == fPadPlaneCmd )
	{
		fDetector->setPadPlaneId(fPadPlaneCmd->GetNewIntValue(newValue));
	}

	// Set Position Z
	if( command == fPositionZCmd )
	{
		fDetector->setPositionZ(fPositionZCmd->GetNewDoubleValue(newValue));
	}

}
