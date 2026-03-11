/// @file StiCATpcSeedFinder.h
/// @brief Interface to the Cellular Automaton TPC seed finder for STI track initialisation.
///
/// @ingroup StiTrackFinding
#ifndef __StiCATpcSeedFinder_h__
#define __StiCATpcSeedFinder_h__
#include "Sti/StiHitContainer.h"
#include "Sti/StiTrackFinder.h"

class StiTrack;
class StiCATpcTrackerInterface;

/// @class SeedHit_t
/// @brief Hit record used by the CA TPC seed finder, holding pad-row, status, and spatial coordinates.
/// @ingroup StiTrackFinding
struct SeedHit_t {
#if 0
  Int_t mMinPad, mMaxPad, mMinTmbk, mMaxTmbk;
#endif
  Int_t padrow, status, taken, track_key ; //"m" for modified
  Double_t x,y,z;
  StiHit   *hit;
};
/// @class Seed_t
/// @brief Container for a CA-found seed: an ordered list of SeedHit_t pointers and initial/final node parameters.
/// @ingroup StiTrackFinding
class Seed_t {
 public:
  vector<SeedHit_t *> vhit;
  Int_t total_hits;
  StiNodePars firstNodePars;
  StiNodePars lastNodePars;
  StiNodeErrs firstNodeErrs;
  StiNodeErrs lastNodeErrs;
  virtual void Print(Option_t *option="") const {
    firstNodePars.print();
    firstNodeErrs.print();
    lastNodePars.print();
    lastNodeErrs.print();
  }
};
/// @class StiCATpcSeedFinder
/// @brief Interface to the Cellular Automaton TPC seed finder; converts CA seeds into initial StiKalmanTrackNode chains.
///
/// @ingroup StiTrackFinding
class StiCATpcSeedFinder: public StiTrackFinder 
{
 public:
  StiCATpcSeedFinder(){mSeeds=0;mEnded=0;setName("CASeedFinder");}
  virtual void startEvent(){mEnded=0;}; 
  virtual StiTrack *findTrack(double rMin=0); 
  static Bool_t   SeedsCompareStatus(const Seed_t a, const Seed_t b);
//??  static void     findTpcTracks(StiCATpcTrackerInterface &caTrackerInt);
  protected:
  int mEnded;
  std::vector<Seed_t> *mSeeds;
};

#endif
