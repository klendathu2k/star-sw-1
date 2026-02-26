/***************************************************************************
*
* $Id: StIstLadderHitCollection.h,v 2.1 2014/04/10 16:20:09 jeromel Exp $
*
* Author: Yaping Wang, August 2013
****************************************************************************
* Description:
* Data collection for IST hits, and one instance corresponds to one ladder.
***************************************************************************/

#ifndef StIstLadderHitCollection_hh
#define StIstLadderHitCollection_hh
/// @file StIstLadderHitCollection.h
/// @brief IST hit collection for a single ladder of the STAR Intermediate Silicon Tracker.

#include "StObject.h"
#include "StIstSensorHitCollection.h"
#include "StEvent/StEnumerations.h"

using namespace StIstConsts;

/// @brief Collection of IST hits for a single ladder of the Intermediate Silicon Tracker.
class StIstLadderHitCollection : public StObject
{
public:
   /// @brief Default constructor.
   StIstLadderHitCollection();
   /// @brief Destructor.
   ~StIstLadderHitCollection();

   /// @brief Returns the total number of hits across all sensors in this ladder.
   unsigned int  numberOfHits() const;

   /// @brief Returns a pointer to the hit collection for the given sensor (0-indexed).
   StIstSensorHitCollection       *sensor(unsigned int);
   /// @brief Returns a const pointer to the hit collection for the given sensor (0-indexed).
   const StIstSensorHitCollection *sensor(unsigned int) const;

private:
   StIstSensorHitCollection  mSensors[kIstNumSensorsPerLadder]; ///< Per-sensor hit collections.

   ClassDef(StIstLadderHitCollection, 1)
};
#endif


/***************************************************************************
*
* $Log: StIstLadderHitCollection.h,v $
* Revision 2.1  2014/04/10 16:20:09  jeromel
* Ist struct (Thomas OK-ed)
*
* Revision 1.5  2014/03/17 20:27:57  ypwang
* remove numOfLadder() and numOfSensor() from StIstHitCollection.h and StIstLadderHitCollection.h, respectively
*
* Revision 1.4  2014/03/13 22:10:12  smirnovd
* Move some constants from StIstUtil/StIstConsts.h to StEvent/StEnumerations.h to avoid external dependance of StEvent on StIstUtil
*
* Revision 1.3  2014/02/26 21:18:08  smirnovd
* Style corrected with astyle -s3 -p -H -A3 -k3 -O -o -y -Y -f
*
* Revision 1.2  2014/01/29 18:25:00  ypwang
* updating scripts
*
*
****************************************************************************
* StIstLadderHitCollection.h,v 1.0
* Revision 1.0 2013/11/04 15:25:30 Yaping
* Initial version
****************************************************************************/
