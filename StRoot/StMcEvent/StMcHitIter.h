/***************************************************************************
 *
 * $Id: StMcHitIter.h,v 2.2 2018/03/15 22:00:35 smirnovd Exp $
 * $Log: StMcHitIter.h,v $
 * Revision 2.2  2018/03/15 22:00:35  smirnovd
 * Fix linker error by removing declared but undefined functions
 *
 * Revision 2.1  2012/03/22 01:08:05  perev
 * McHitIter added
 *
 *
 * 
 **************************************************************************/
/// @file StMcHitIter.h
/// @brief Iterator class for traversing all MC hits in an StMcEvent across all detectors.
#ifndef StMcHitIter_h
#define StMcHitIter_h
#include "StEvent/StEnumerations.h" 		//StDetectorId
#include "TObject.h" 		
class StMcEvent;
class StMcHit;

/// @brief Iterator that steps through all StMcHit objects in an StMcEvent, detector by detector.
class StMcHitIter : public TObject {
public:
  /// @brief Construct and initialize the iterator for the given StMcEvent.
  StMcHitIter(const StMcEvent *mcev);
  /// @brief Reset the iterator to the start of the given StMcEvent.
  void  Reset(const StMcEvent *mcev);
  /// @brief Return the current iterator path indices and fill lev with the depth.
  const int *Path(int &lev) const 	{lev = mLev; return mPath;}
  /// @brief Return the array of maximum index values at each path level.
  const int *MaxN() const 		{return mMaxN;}

protected:

int     mDets[kMaxDetectorId+1];   ///< Enabled detector IDs to iterate over
char 	mBeg[1];                   //!
const StMcEvent	*mMcEv;            ///< The StMcEvent being iterated
const StMcHit 	*mMcHit;           ///< Current hit pointer
int 	mLev;                      ///< Current depth level in the path hierarchy
int 	mPath[10];                 ///< Current indices at each path level
int 	mMaxN[10];                 ///< Maximum index at each path level
char 	mEnd[1];                   //!

  ClassDef(StMcHitIter,0)
};

#endif

