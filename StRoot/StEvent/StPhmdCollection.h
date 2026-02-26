/*!
 * \class StPhmdCollection
 * \author  Subhasis Chattopadhyay
 */
/********************************************************************
 *
 * $Id: StPhmdCollection.h,v 2.1 2002/12/20 22:33:00 ullrich Exp $
 *
 * Author: Subhasis Chattopadhyay, Dec 2002
 ********************************************************************
 *
 * Description: Base class for PMD Collection which
 *              includes both PMD and CPV
 *
 ********************************************************************
 *
 * $Log: StPhmdCollection.h,v $
 * Revision 2.1  2002/12/20 22:33:00  ullrich
 * Initial Revision.
 *
 ********************************************************************/
#ifndef StPhmdCollection_hh
#define StPhmdCollection_hh

/// @file StPhmdCollection.h
/// @brief Event-level collection for the STAR Photon Multiplicity Detector (PMD and CPV planes).

#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"
#include "StPhmdHit.h"

class StPhmdDetector;

/// @brief Top-level container for PHMD detector data, holding both the PMD and CPV sub-detector planes.
class StPhmdCollection : public StObject {
public:
    StPhmdCollection();
    ~StPhmdCollection();
    
    /// @brief Returns a pointer to the detector plane identified by @p id (kPmdId or kCpvId).
    StPhmdDetector*          detector(StDetectorId);
    /// @brief Returns a const pointer to the detector plane identified by @p id.
    const StPhmdDetector*    detector(StDetectorId) const;
    
    /// @brief Registers a detector plane (PMD or CPV) with the collection.
    void setDetector(StPhmdDetector*);
    
private:
    StPhmdDetector*   mDetector;      ///< Pointer to the PMD (photon) detector plane.
    StPhmdDetector*   mCpvDetector;   ///< Pointer to the CPV (charged-particle veto) detector plane.
    ClassDef(StPhmdCollection,1)
};

#endif








