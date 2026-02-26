/*!
 * \class StPxlLadderHitCollection 
 * \author X. Dong, Jan 2013
 */
/***************************************************************************
 *
 * $Id: StPxlLadderHitCollection.h,v 2.1 2013/03/05 14:40:41 ullrich Exp $
 *
 * Author: X. Dong, Jan 2013
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StPxlLadderHitCollection.h,v $
 * Revision 2.1  2013/03/05 14:40:41  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StPxlLadderHitCollection_hh
#define StPxlLadderHitCollection_hh
/// @file StPxlLadderHitCollection.h
/// @brief PXL hit collection for one ladder of the STAR Pixel Detector.

#include "StObject.h"
#include "StPxlSensorHitCollection.h"

/// @brief Collection of PXL hits for a single ladder, containing per-sensor sub-collections.
class StPxlLadderHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StPxlLadderHitCollection();
    /// @brief Destructor.
    ~StPxlLadderHitCollection();

    /// @brief Returns the total number of hits across all sensors in this ladder.
    unsigned int numberOfHits() const;
    /// @brief Returns the number of sensors per ladder (10).
    unsigned int numberOfSensors() const;

    /// @brief Returns a pointer to the hit collection for the given sensor (0-indexed).
    StPxlSensorHitCollection*       sensor(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given sensor (0-indexed).
    const StPxlSensorHitCollection* sensor(unsigned int) const;

private:
    enum { mNumberOfSensors = 10 };
    StPxlSensorHitCollection mSensors[mNumberOfSensors]; ///< Per-sensor hit collections.
    
    ClassDef(StPxlLadderHitCollection,1)
};

inline unsigned int StPxlLadderHitCollection::numberOfSensors() const { return mNumberOfSensors; }

#endif
