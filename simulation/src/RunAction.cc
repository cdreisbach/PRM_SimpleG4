#include <DataManager.hh>
#include <DataVisualisation.hh>

#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "StepMax.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

RunAction::RunAction() : G4UserRunAction() {}

void RunAction::BeginOfRunAction( const G4Run *aRun )
{
	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
	DataManager::getInstance()->prepare();
}

void RunAction::EndOfRunAction( const G4Run *aRun )
{
	G4int nbofEvents = aRun->GetNumberOfEvent();
	if ( nbofEvents == 0 )
	{
		return;
	}
}
