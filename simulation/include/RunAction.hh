#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "g4root.hh"

class DetectorConstruction;
class PhysicsList;
class PrimaryGeneratorAction;
class G4Run;
class RunAction : public G4UserRunAction
{
	public:

		RunAction();
		virtual ~RunAction() = default;
		virtual void BeginOfRunAction( const G4Run *run );
		virtual void EndOfRunAction( const G4Run *run );
};

#endif
