#include "RunManager.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "SteppingVerbose.hh"
#include "DataManager.hh"

#include <G4UImanager.hh>
#include <Randomize.hh>
#include <G4UIExecutive.hh>
#include <G4VisExecutive.hh>

int main(int argc, char** argv)
{
	// detect interactive mode (if no arguments) and define UI session
	G4UIExecutive* ui = nullptr;
	std::string macroFileName  = "../../simulation/macro/init_vis.mac";
	std::string outputFileName = "run.root";
	if (argc == 1)
	{
		ui = new G4UIExecutive(argc,argv);
	}
	else if( argc == 2)
	{
		macroFileName = "../../simulation/macro/run.mac";
		outputFileName = argv[1];
	}
	else if( argc == 3)
	{
		// Example ./ProtonSimulation run.mac output.root
		outputFileName = argv[1];
		macroFileName  = argv[2];
	}

	// choose the Random engine
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());

	// my Verbose output class
	G4VSteppingVerbose::SetInstance(new SteppingVerbose());

	// Construct the default run manager
	RunManager* runManager = new RunManager();

	// set mandatory initialization classes
	DetectorConstruction* detectorConstruction = new DetectorConstruction();
	PhysicsList*          physicsList          = new PhysicsList();

	runManager->SetUserInitialization(detectorConstruction);
	runManager->SetUserInitialization(physicsList);

	//set user action classes
	PrimaryGeneratorAction* generatorAction = new PrimaryGeneratorAction();
	RunAction*              runAction       = new RunAction();
	TrackingAction*         trackingAction  = new TrackingAction();
	SteppingAction*         steppingAction  = new SteppingAction();
	EventAction*            eventAction     = new EventAction();

	runManager->SetUserAction(eventAction);
	runManager->SetUserAction(generatorAction);
	runManager->SetUserAction(runAction);
	runManager->SetUserAction(trackingAction);
	runManager->SetUserAction(steppingAction);

	// initialize visualization
	G4VisManager* visManager = nullptr;

	// get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Data manager
	DataManager::getInstance()->setOutputFileName(outputFileName);

	G4String command = "/control/execute ";
	if (ui)
	{
		// interactive mode
		visManager = new G4VisExecutive;
		visManager->Initialize();
		UImanager->ApplyCommand(command+macroFileName);
		ui->SessionStart();
		delete ui;
	}
	else
	{
		// batch mode
		UImanager->ApplyCommand(command+macroFileName);
	}
	DataManager::getInstance()->finish();
	// job termination
	delete visManager;
	delete runManager;
}
