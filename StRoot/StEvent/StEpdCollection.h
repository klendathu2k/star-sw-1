/*!
 * \class StEpdCollection
 * \author Mike Lisa, Jan 2018
 */
/***************************************************************************
 *
 * $Id: StEpdCollection.h,v 2.1 2018/02/08 17:35:02 ullrich Exp $
 *
 * Author: Mike Lisa, Jan 2018
 ***************************************************************************
 *
 * Description:
 *
 * Persistent data which is written into StEvent
 * based on TriggerData object and database information
 * filled by StEpdHitMaker
 *
 * Using StBTofCollection as a template
 *
 ***************************************************************************
 *
 * $Log: StEpdCollection.h,v $
 * Revision 2.1  2018/02/08 17:35:02  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StEpdCollection_hh
#define StEpdCollection_hh

/// @file StEpdCollection.h
/// @brief Event-level collection of hits in the STAR Event Plane Detector (EPD).

#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"
#include "StEpdHit.h"

/// @brief Container for all EPD tile hits recorded in a single event.
class StEpdCollection : public StObject {
public: 
    StEpdCollection();
    ~StEpdCollection();

    /// @brief Returns a const reference to the vector of EPD tile hits.
    const StSPtrVecEpdHit&     epdHits() const;
    /// @brief Returns a reference to the vector of EPD tile hits.
    StSPtrVecEpdHit&           epdHits();

    /// @brief Adds an EPD hit to the collection.
    void addHit(const StEpdHit*);

    /// @brief Returns true if the collection contains at least one hit.
    bool hitsPresent() const;
    
private:

    StSPtrVecEpdHit mEpdHits;   ///< Vector of EPD tile hits owned by this collection.
  
    ClassDef(StEpdCollection, 1)
};

#endif
