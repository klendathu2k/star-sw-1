/***************************************************************************
* $Id: StFstSensorHitCollection.h$
*
* Author: Shenghui Zhang, Oct. 2021
****************************************************************************
* Description:
* Data collection for FST hits, and one instance corresponds to one sensor.
***************************************************************************/

#ifndef StFstSensorHitCollection_hh
#define StFstSensorHitCollection_hh

/// @file StFstSensorHitCollection.h
/// @brief Collection of FST reconstructed hits for a single sensor.

#include "StObject.h"
#include "StContainers.h"

class StFstHit;

/// @brief Container of StFstHit objects belonging to one FST sensor.
class StFstSensorHitCollection : public StObject
{
public:
   StFstSensorHitCollection();
   ~StFstSensorHitCollection();

   /// @brief Return the number of hits in this sensor.
   unsigned int numberOfHits() const;

   /// @brief Return a reference to the hit vector for this sensor.
   StSPtrVecFstHit       &hits();
   /// @brief Return a const reference to the hit vector for this sensor.
   const StSPtrVecFstHit &hits() const;

private:
   StSPtrVecFstHit mHits; ///< Hits on this sensor; ownership is managed by StStrArray (auto-deletes elements).

   ClassDef(StFstSensorHitCollection, 1)
};
#endif


/***************************************************************************
* StFstSensorHitCollection.h,v 1.0
* Revision 1.0 2021/10/04 Shenghui Zhang
* Initial version
****************************************************************************/
