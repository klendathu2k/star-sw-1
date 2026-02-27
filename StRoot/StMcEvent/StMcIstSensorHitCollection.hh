/***************************************************************************
 *
 *
 * Author: Amilkar Quintero, Feb 2015
 ***************************************************************************
 *
 * Description: Monte Carlo Ist Sensor Hit Collection class
 *
 ***************************************************************************/
/// @file StMcIstSensorHitCollection.hh
/// @brief Monte Carlo hit collection for one IST sensor.

#ifndef StMcIstSensorHitCollection_hh
#define StMcIstSensorHitCollection_hh
#include "StMcContainers.hh"
#include "StObject.h"

class StMcIstHit;

/// @brief Collection of Monte Carlo IST hits for a single sensor.
///
/// Stores a vector of StMcIstHit pointers for one IST sensor.
/// Represents the leaf level of the IST hit hierarchy.
class StMcIstSensorHitCollection : public StObject
{
public:
    /// @brief Default constructor.
    StMcIstSensorHitCollection();
    // StMcIstSensorHitCollection(const StMcIstSensorHitCollection&); use default
    // const StMcIstSensorHitCollection& operator=(const StMcIstSensorHitCollection&); use default
    /// @brief Destructor.
    virtual ~StMcIstSensorHitCollection();
    /// @brief Clears the hit vector.
    void Clear(const char* opt="");
    /// @brief Returns true to allow ROOT browser folding.
    bool IsFolder() const { return true;};
  //virtual void Browse(TBrowser *b); 
    
    /// @brief Returns a reference to the vector of hits.
    StSPtrVecMcIstHit&       hits();
    /// @brief Returns a const reference to the vector of hits.
    const StSPtrVecMcIstHit& hits() const;

protected:
    StSPtrVecMcIstHit mHits; ///< Vector of Monte Carlo IST hits.
    ClassDef(StMcIstSensorHitCollection,1)
};
#endif
