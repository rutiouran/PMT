#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

namespace B1
{

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  fEdep = 0.;
}

void EventAction::EndOfEventAction(const G4Event*)
{
  // accumulate statistics in run action
 // fRunAction->AddEdep(fEdep);
}

}
