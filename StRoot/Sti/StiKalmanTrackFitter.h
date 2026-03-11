/// @file StiKalmanTrackFitter.h
/// @brief Kalman smoother and track-fit quality classes.
///
/// @ingroup StiTrackFitting

#ifndef StiKalmanTrackFitter_H
#define StiKalmanTrackFitter_H
#include "StiTrackFitter.h"
#include "StDetectorDbMaker/StiKalmanTrackFitterParameters.h"
class StiTrack;
class EditableParameters;

/// @class StiKalmanTrackFitter
/// @brief Full Kalman smoother refit of a found track.
///
/// `fit(track, dir)` runs a Kalman smoother pass in the given direction
/// (kInsideOut or kOutsideIn) over all nodes in the track.  At each node
/// the propagation is re-done from scratch, MCS errors from `StiMcsErrs`
/// are added, and — if the node has an associated StiHit — the measurement
/// update is applied via `StiTrackNodeHelper`.
///
/// **Fitting workflow (per call to fit()):**
/// 1. Walk nodes in the requested direction.
/// 2. For each node, propagate the state from the previous node.
/// 3. Add multiple-Coulomb-scattering (MCS) process noise via `StiMcsErrs`.
/// 4. If a hit exists: compute chi² increment; if acceptable, apply update.
/// 5. Accumulate fit-quality statistics into `QaFit`.
///
/// Return value is one of the kStatus enum values; kNoErrors on success.
/// @ingroup StiTrackFitting
///Class implements a kalman track fitter 
///Based on the abstract interface StiTrackFitter
///Uses the fitting parameters carried by StiKalmanTrackFitterParameters
class StiKalmanTrackFitter : public StiTrackFitter, public Named, public Described
{
 public:
  
  StiKalmanTrackFitter() {}
  virtual ~StiKalmanTrackFitter() {}
  virtual Int_t fit(StiTrack * track, Int_t direction);
  static  void setDebug(Int_t m = 0) {_debug = m;}
  static  Int_t  debug() {return _debug;}

  typedef enum{ // type of return value for the fit() procedure
    kNoErrors = 0,
    kShortTrackBeforeFit,
    kShortTrackAfterFit,
    kManyErrors
  } TFitStatus;
  
 protected:
  static  Int_t _debug;
};

#endif
