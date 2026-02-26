/***************************************************************************
*
* $Id: StFstHitCollection.h $
*
* Author: Shenghui Zhang, Oct. 2021
****************************************************************************
* Description:
* Data collection for FST hits, and is saved to StEvent.
***************************************************************************/

#ifndef StFstHitCollection_hh
#define StFstHitCollection_hh

/// @file StFstHitCollection.h
/// @brief Event-level collection of FST reconstructed hits stored in StEvent.

#include "StObject.h"
#include "StFstWedgeHitCollection.h"
#include "StEvent/StEnumerations.h"
#include "StEvent/StFstConsts.h"

class StFstHit;

/// @brief Top-level container for all Forward Silicon Tracker (FST) reconstructed hits,
/// organised hierarchically as wedge → sensor collections.
class StFstHitCollection : public StObject
{
public:
   StFstHitCollection();

   /// @brief Add a hit to the appropriate wedge/sensor sub-collection.
   bool          addHit(StFstHit *);
   /// @brief Return the total number of hits summed over all wedges.
   unsigned int  numberOfHits() const;

   /// @brief Return the clustering algorithm type identifier.
   unsigned char getClusteringType() const;
   /// @brief Set the clustering algorithm type identifier.
   void          setClusteringType(unsigned char clusteringType);

   /// @brief Return a pointer to the hit collection for wedge @p i (0-based).
   StFstWedgeHitCollection       *wedge(unsigned int);
   /// @brief Return a const pointer to the hit collection for wedge @p i (0-based).
   const StFstWedgeHitCollection *wedge(unsigned int) const;

private:
   StFstWedgeHitCollection mWedges[kFstNumWedges]; ///< Hit sub-collections, one per FST wedge.
   UChar_t mClusteringType;  ///< Clustering algorithm type identifier.

   ClassDef(StFstHitCollection, 1)
};
#endif


/***************************************************************************
* StFstHitCollection.h,v 1.0
* Revision 1.0 2021/10/04 Shenghui Zhang
* Initial version
****************************************************************************/
