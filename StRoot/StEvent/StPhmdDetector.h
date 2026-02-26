/*!
 * \class StPhmdDetector
 * \author Subhasis Chattopadhyay
 */
/********************************************************************
 *
 * $Id: StPhmdDetector.h,v 2.2 2003/04/22 00:08:13 ullrich Exp $
 *
 * Author: Subhasis Chattopadhyay, Dec 2002
 ********************************************************************
 *
 * Description: Base class for PMD detector
 *
 ********************************************************************
 *
 * $Log: StPhmdDetector.h,v $
 * Revision 2.2  2003/04/22 00:08:13  ullrich
 * Removed //! comments
 *
 * Revision 2.1  2002/12/20 22:33:00  ullrich
 * Initial Revision.
 *
 ********************************************************************/
#ifndef StPhmdDetector_hh
#define StPhmdDetector_hh

/// @file StPhmdDetector.h
/// @brief Representation of a single PHMD detector plane (PMD or CPV) with its supermodules and clusters.

#include "StObject.h"
#include "StEnumerations.h"

class StPhmdHit;
class StPhmdModule;
class StPhmdClusterCollection;

/// @brief Represents one PHMD detector plane (PMD or CPV), aggregating supermodules and cluster data.
class StPhmdDetector : public StObject {
public:
    StPhmdDetector();
    /// @brief Constructs a detector plane with the given detector identifier.
    StPhmdDetector(StDetectorId);
    ~StPhmdDetector();

    /// @brief Returns the detector identifier (kPmdId or kCpvId).
    StDetectorId  id() const; 
    /// @brief Returns the maximum number of supermodules in this detector plane.
    unsigned int  numberOfModules() const;
    
    /// @brief Adds a hit to the appropriate supermodule; returns true on success.
    bool          addHit(StPhmdHit*);
    /// @brief Returns the total number of hits across all supermodules.
    unsigned int  numberOfHits() const;
    
    /// @brief Returns a pointer to the supermodule at the given index [0, mMaxModules).
    StPhmdModule*                   module(unsigned int);
    /// @brief Returns a const pointer to the supermodule at the given index.
    const StPhmdModule*             module(unsigned int) const;
    /// @brief Returns the number of hits recorded in the supermodule at the given index.
    int                             moduleHits(unsigned int);

    /// @brief Returns a pointer to the cluster collection for this detector plane.
    StPhmdClusterCollection*        cluster();
    /// @brief Returns a const pointer to the cluster collection for this detector plane.
    const StPhmdClusterCollection*  cluster() const;

    /// @brief Sets the cluster collection for this detector plane.
    void setCluster(StPhmdClusterCollection*);
    /// @brief Sets the supermodule pointer at the given index.
    void setModule(StPhmdModule*, unsigned int);

        
private:
    enum {mMaxModules = 12};
    StDetectorId             mDetectorId;                   ///< Detector identifier (PMD or CPV).
    
    Int_t                    mModulesNHit[mMaxModules];     ///< Hit count per supermodule.
    StPhmdModule*            mModules[mMaxModules];         ///< Pointers to the supermodule objects.
    StPhmdClusterCollection* mClusters;                     ///< Pointer to the cluster collection.
    
    ClassDef(StPhmdDetector,1)
};

inline StDetectorId StPhmdDetector::id() const
{return mDetectorId;}

#endif
