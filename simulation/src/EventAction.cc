#include "EventAction.hh"
#include "DataManager.hh"

EventAction::EventAction() : G4UserEventAction() {}

void EventAction::BeginOfEventAction(const G4Event*)
{
	DataManager::getInstance()->clear();
}

void EventAction::EndOfEventAction(const G4Event*)
{
	DataManager::getInstance()->save();
}
