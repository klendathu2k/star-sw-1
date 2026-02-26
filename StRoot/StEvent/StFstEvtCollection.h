/***************************************************************************
*
* $Id: StFstEvtCollection.h $
*
* Author: Te-Chuan Huang, Aug. 2022
****************************************************************************
* Description:
* Data collection for FST raw hits, and is saved to StEvent.
***************************************************************************/

#ifndef StFstEvtCollection_hh
#define StFstEvtCollection_hh

/// @file StFstEvtCollection.h
/// @brief Event-level collection of FST raw hits stored in StEvent.

#include "Stiostream.h"
#include "StObject.h"
#include "StContainers.h"
#include "StEvent/StEnumerations.h"
#include "StEvent/StFstConsts.h"

class StFstRawHit;

/// @brief Top-level container for all Forward Silicon Tracker (FST) raw hit data in one event.
class StFstEvtCollection : public StObject
{
public:
   StFstEvtCollection();
   ~StFstEvtCollection() {}

   /// @brief Add a raw hit to the collection (takes ownership).
   void          addRawHit(StFstRawHit *);
   /// @brief Return the total number of raw hits in this event.
   unsigned int  numberOfRawHits() const;

   /// @brief Return a reference to the vector of raw hits.
   StSPtrVecFstRawHit       &rawHits();
   /// @brief Return a const reference to the vector of raw hits.
   const StSPtrVecFstRawHit &rawHits() const;

   /// @brief Print a summary of the collection contents.
   void print(int option=1);

private:
   StSPtrVecFstRawHit mRawHits; ///< Inherits from StStrArray which takes care of deleting the objects
                                ///< pointed by the pointers in this array. This is different from the std::vector

   ClassDef(StFstEvtCollection, 1)
};
#endif

/***************************************************************************
* StFstEvtCollection.h,v 1.0
* Revision 1.0 2022/08/25 Te-Chuan Huang
* Initial version
****************************************************************************/
