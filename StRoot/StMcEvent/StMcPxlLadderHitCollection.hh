/***************************************************************************
 *
 * $Id: StMcPxlLadderHitCollection.hh,v 2.1 2013/03/25 23:50:36 perev Exp $
 * $Log: StMcPxlLadderHitCollection.hh,v $
 * Revision 2.1  2013/03/25 23:50:36  perev
 * Mustafa.Pxl add
 *
 *
 **************************************************************************/
/// @file StMcPxlLadderHitCollection.hh
/// @brief Monte Carlo hit collection for one PXL ladder.

#ifndef StMcPxlLadderHitCollection_hh
#define StMcPxlLadderHitCollection_hh

#include "StObject.h"
#include "StMcPxlSensorHitCollection.hh"

/// @brief Collection of Monte Carlo PXL hits for a single ladder.
///
/// Holds kNumberOfSensors (10) StMcPxlSensorHitCollection objects,
/// one per sensor along the ladder (sensors numbered 1-10 from East to West).
class StMcPxlLadderHitCollection : public StObject
{
public:
    /// @brief Default constructor.
    StMcPxlLadderHitCollection();
    /// @brief Destructor.
    virtual ~StMcPxlLadderHitCollection();
    
    /// @brief Returns the total number of hits across all sensors in this ladder.
    unsigned int numberOfHits() const;
    /// @brief Returns the number of sensors in this ladder.
    unsigned int numberOfSensors() const;

    /// @brief Returns a pointer to the sensor sub-collection at the given index.
    StMcPxlSensorHitCollection*     sensor(unsigned int);
    /// @brief Returns a const pointer to the sensor sub-collection at the given index.
    const StMcPxlSensorHitCollection* sensor(unsigned int) const;

protected:
    enum { kNumberOfSensors = 10 }; ///< Number of sensors per PXL ladder.
    StMcPxlSensorHitCollection mSensors[kNumberOfSensors]; ///< Array of sensor sub-collections.

    ClassDef(StMcPxlLadderHitCollection,1)
};

inline unsigned int StMcPxlLadderHitCollection::numberOfSensors() const {return kNumberOfSensors;}
#endif
