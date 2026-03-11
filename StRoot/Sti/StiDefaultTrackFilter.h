/// @file StiDefaultTrackFilter.h
/// @brief Default StiFilter implementation that accepts all tracks (no cuts applied).
///
/// @ingroup StiUtilities
#ifndef StiDefaultTrackFilter_H_INCLUDED
#define StiDefaultTrackFilter_H_INCLUDED 1
#include "StiTrack.h"
#include "Base/EditableFilter.h"
#include "Base/Parameter.h"

/// @class StiDefaultTrackFilter
/// @brief Default StiFilter implementation that accepts all tracks (no cuts applied).
///
/// Can be replaced by experiment-specific track selection.
/// @ingroup StiUtilities
class StiDefaultTrackFilter : public EditableFilter<StiTrack>
{
 public:
  StiDefaultTrackFilter();
  StiDefaultTrackFilter(const string & name, const string & description);
  virtual ~StiDefaultTrackFilter();
  bool accept(const StiTrack *filtered) const;
  virtual void initialize();
  virtual void setDefaults();
};

#endif
