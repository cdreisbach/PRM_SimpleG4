#include "TrackingAction.hh"

#include "G4Track.hh"

TrackingAction::TrackingAction() :G4UserTrackingAction() {}

TrackingAction::~TrackingAction() {}

void TrackingAction::PostUserTrackingAction(const G4Track*) {}

