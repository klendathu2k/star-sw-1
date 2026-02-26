/*!
 * \class StEtrHitCollection 
 * \author Ming Shao, Jan 5, 2012
 */
/***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEtrHitCollection.h,v $
 * Revision 2.1  2012/01/24 03:06:13  perev
 * Add Etr
 *
 *
 * Revision 1.0  2012/01/05  Ming
 * Initial Revision
 *
 **************************************************************************/
#ifndef StEtrHitCollection_hh
#define StEtrHitCollection_hh

/// @file StEtrHitCollection.h
/// @brief Collection class for ETR (Endcap Tracking prototype) hits.
   
#include "StObject.h"
#include "StContainers.h"

class StEtrHit;

/// @brief Collection of reconstructed ETR (Endcap Tracking prototype) hits.
class StEtrHitCollection : public StObject {
 public:
    /// @brief Default constructor.
    StEtrHitCollection();
    /// @brief Destructor.
    ~StEtrHitCollection();
    
    /// @brief Add a hit to the collection; returns true on success.
    bool          addHit(StEtrHit*);
    /// @brief Returns the total number of hits in the collection.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of ETR layers.
    unsigned int  numberOfLayers() const;
    /// @brief Returns the number of ETR azimuthal sectors.
    unsigned int  numberOfSectors() const;
    
    /// @brief Returns a mutable reference to the hit pointer vector.
    StSPtrVecEtrHit&       hits();
    /// @brief Returns a const reference to the hit pointer vector.
    const StSPtrVecEtrHit& hits() const;

private:
    enum { mNumberOfLayers  = 3  };  ///< Number of radial ETR layers.
    enum { mNumberOfSectors = 12 };  ///< Number of azimuthal ETR sectors.

    StSPtrVecEtrHit mHits;  ///< Container of ETR hit pointers.
    
    ClassDef(StEtrHitCollection,1)
};
#endif
