#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class DetectorConstruction;
class RunAction;
class SteppingAction : public G4UserSteppingAction
{
	public:
		SteppingAction();
		virtual ~SteppingAction();
		virtual void UserSteppingAction( const G4Step *g4Step );
};

#endif
