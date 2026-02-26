/***************************************************************************
*
* $Id: StIstHitCollection.h,v 2.1 2014/04/10 16:17:23 jeromel Exp $
*
* Author: Yaping Wang, August 2013
****************************************************************************
* Description:
* Data collection for IST hits, and is saved to StEvent.
***************************************************************************/

#ifndef StIstHitCollection_hh
#define StIstHitCollection_hh
/// @file StIstHitCollection.h
/// @brief Top-level hit collection for the STAR Intermediate Silicon Tracker (IST).

#include "StObject.h"
#include "StIstLadderHitCollection.h"
#include "StEvent/StEnumerations.h"

using namespace StIstConsts;

class StIstHit;

/// @brief Top-level collection of IST hits for the full Intermediate Silicon Tracker.
class StIstHitCollection : public StObject
{
public:
   /// @brief Default constructor.
   StIstHitCollection();
   /// @brief Destructor.
   ~StIstHitCollection();

   /// @brief Adds an IST hit to the appropriate ladder/sensor sub-collection.
   bool          addHit(StIstHit *);
   /// @brief Returns the total number of hits across all ladders.
   unsigned int  numberOfHits() const;

   /// @brief Returns the clustering algorithm type used for these hits.
   unsigned char getClusteringType() const;
   /// @brief Sets the clustering algorithm type.
   void          setClusteringType(unsigned char clusteringType);

   /// @brief Returns a pointer to the hit collection for the given ladder (0-indexed).
   StIstLadderHitCollection       *ladder(unsigned int);
   /// @brief Returns a const pointer to the hit collection for the given ladder (0-indexed).
   const StIstLadderHitCollection *ladder(unsigned int) const;

private:
   StIstLadderHitCollection mLadders[kIstNumLadders]; ///< Per-ladder hit collections.
   UChar_t mClusteringType;  ///< Clustering algorithm type identifier.

   ClassDef(StIstHitCollection, 1)
};
#endif


/***************************************************************************
*
* $Log: StIstHitCollection.h,v $
* Revision 2.1  2014/04/10 16:17:23  jeromel
* Add hit def for Ist (Thomas OK-ed)
*
* Revision 1.8  2014/03/17 20:27:57  ypwang
* remove numOfLadder() and numOfSensor() from StIstHitCollection.h and StIstLadderHitCollection.h, respectively
*
* Revision 1.7  2014/03/13 22:17:09  smirnovd
* Minor whitespace alignment fixes
*
* Revision 1.6  2014/03/13 22:10:30  smirnovd
* Expand tabs and trim trailing whitespace
*
* Revision 1.5  2014/03/13 22:10:12  smirnovd
* Move some constants from StIstUtil/StIstConsts.h to StEvent/StEnumerations.h to avoid external dependance of StEvent on StIstUtil
*
* Revision 1.4  2014/02/26 21:18:08  smirnovd
* Style corrected with astyle -s3 -p -H -A3 -k3 -O -o -y -Y -f
*
* Revision 1.3  2014/02/25 17:08:49  ypwang
* add mClusteringType and its accessory/modifier functions
*
*
****************************************************************************
* StIstHitCollection.h,v 1.0
* Revision 1.0 2013/11/04 15:25:30 Yaping
* Initial version
****************************************************************************/
