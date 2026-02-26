/***************************************************************************
*
* $Id: StIstSensorHitCollection.h,v 2.1 2014/04/10 16:21:04 jeromel Exp $
*
* Author: Yaping Wang, August 2013
****************************************************************************
* Description:
* Data collection for IST hits, and one instance corresponds to one sensor.
***************************************************************************/

#ifndef StIstSensorHitCollection_hh
#define StIstSensorHitCollection_hh
/// @file StIstSensorHitCollection.h
/// @brief IST hit collection for a single sensor of the STAR Intermediate Silicon Tracker.

#include "StObject.h"
#include "StContainers.h"

class StIstHit;

/// @brief Collection of IST hits for a single sensor of the Intermediate Silicon Tracker.
class StIstSensorHitCollection : public StObject
{
public:
   /// @brief Default constructor.
   StIstSensorHitCollection();
   /// @brief Destructor.
   ~StIstSensorHitCollection();

   /// @brief Returns a reference to the vector of hits on this sensor.
   StSPtrVecIstHit       &hits();
   /// @brief Returns a const reference to the vector of hits on this sensor.
   const StSPtrVecIstHit &hits() const;

private:
   StSPtrVecIstHit mHits; ///< Owned hit collection; StStrArray manages object lifetimes.

   ClassDef(StIstSensorHitCollection, 1)
};
#endif


/***************************************************************************
*
* $Log: StIstSensorHitCollection.h,v $
* Revision 2.1  2014/04/10 16:21:04  jeromel
* Ist struct (Thomas OK-ed)
*
* Revision 1.3  2014/02/26 21:18:08  smirnovd
* Style corrected with astyle -s3 -p -H -A3 -k3 -O -o -y -Y -f
*
* Revision 1.2  2014/01/29 18:25:00  ypwang
* updating scripts
*
*
****************************************************************************
* StIstSensorHitCollection.h,v 1.0
* Revision 1.0 2013/11/04 15:25:30 Yaping
* Initial version
****************************************************************************/
