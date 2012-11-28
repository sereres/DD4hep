// $Id:$
//====================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------
//
//  Author     : M.Frank
//
//====================================================================
// Framework include files
#include "DDG4/Geant4SensitiveDetector_inline.h"
#include "DDG4/Factories.h"

/*
 *   DD4hep::Simulation namespace declaration
 */
namespace DD4hep {  namespace Simulation {
  /// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  ///               Geant4GenericSD<Tracker>
  /// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  struct Tracker {};
  /// Method for generating hit(s) using the information of G4Step object.
  template <> bool Geant4GenericSD<Tracker>::buildHits(G4Step* step,G4TouchableHistory* ) {
    StepHandler h(step);
    Position prePos    = h.prePos();
    Position postPos   = h.postPos();
    Position direction = (postPos - prePos);
    Position position  = mean_direction(prePos,postPos);
    double   hit_len   = direction.length();
    if (hit_len > 0) {
      direction.setLength(mean_length(h.preMom(),h.postMom()));
    }
    Geant4TrackerHit* hit = 
      new Geant4TrackerHit(h.track->GetTrackID(),
			   h.track->GetDefinition()->GetPDGEncoding(),
			   step->GetTotalEnergyDeposit(),
			   h.track->GetGlobalTime());
    hit->position = position;
    hit->momentum = direction;
    hit->length   = hit_len;
    collection(0)->insert(hit);
    return hit != 0;
  }
  typedef Geant4GenericSD<Tracker> Geant4Tracker;
}}    // End namespace DD4hep::Simulation

DECLARE_GEANT4SENSITIVEDETECTOR(Geant4Tracker);