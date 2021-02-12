#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"

#include "globals.hh"

class RunAction;
class SteppingAction;
class EventAction : public G4UserEventAction
{
	public:

		EventAction();
		~EventAction() = default;

		virtual void BeginOfEventAction(const G4Event *);
		virtual void EndOfEventAction(const G4Event *);
};

#endif
