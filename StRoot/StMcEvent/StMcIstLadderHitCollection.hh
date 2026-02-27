/***************************************************************************
 *
 *  StMcIstLadderHitCollection.hh,v 1.0 2015/02/12 13:00:00
 *
 * Author: Amilkar Quintero, Feb 2015
 ***************************************************************************
 *
 * Description: Monte Carlo Ist Ladder Hit Collection class
 *
 ***************************************************************************/
/// @file StMcIstLadderHitCollection.hh
/// @brief Monte Carlo hit collection for one IST ladder.

#ifndef StMcIstLadderHitCollection_hh
#define StMcIstLadderHitCollection_hh

#include "StMcContainers.hh"
#include "StObject.h"

#include "StMcIstSensorHitCollection.hh"

/// @brief Collection of Monte Carlo IST hits for a single ladder.
///
/// Holds up to mMaxNumberOfSensors StMcIstSensorHitCollection objects,
/// one per sensor on the ladder (6 sensors per IST ladder).
class StMcIstLadderHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StMcIstLadderHitCollection();
    /// @brief Destructor.
    virtual ~StMcIstLadderHitCollection();
    /// @brief Clears all sensor sub-collections.
    void Clear(const char* opt="");
  // bool IsFolder() const { return true;};
  /// @brief Returns the total number of hits across all sensors in this ladder.
  unsigned long numberOfHits() const;
  /// @brief Returns the number of sensors in this ladder.
  unsigned int  numberOfSensors() const {return mMaxNumberOfSensors;}
  
  /// @brief Returns a pointer to the sensor sub-collection at the given index.
  StMcIstSensorHitCollection*       sensor(unsigned int);
  /// @brief Returns a const pointer to the sensor sub-collection at the given index.
  const StMcIstSensorHitCollection* sensor(unsigned int) const;

protected:
  enum { mMaxNumberOfSensors = 6 }; ///< Number of sensors per IST ladder.
  StMcIstSensorHitCollection  mSensors[mMaxNumberOfSensors]; ///< Array of sensor sub-collections.
  
  ClassDef(StMcIstLadderHitCollection,1)
};
#endif
