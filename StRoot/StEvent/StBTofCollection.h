/*!
 * \class StTofCollection 
 * \author Xin Dong, Nov 2008
 */
/***************************************************************************
 *
 * $Id: StBTofCollection.h,v 2.1 2008/12/22 20:30:55 ullrich Exp $
 *
 * Author: Xin Dong, Nov 2008
 ***************************************************************************
 *
 * Description:
 *
 * Persistent data which is written into StEvent
 * directly from the reco chain. All Barrel ToF stuff goes here
 * except the StBTofPidTraits.
 *
 ***************************************************************************
 *
 * $Log: StBTofCollection.h,v $
 * Revision 2.1  2008/12/22 20:30:55  ullrich
 * Initial Revision.
 *
 *
 **************************************************************************/
#ifndef StBTofCollection_hh
#define StBTofCollection_hh

/// @file StBTofCollection.h
/// @brief Event-level collection of Barrel Time of Flight (BTOF) detector data.

#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"
#include "StBTofHeader.h"
#include "StBTofHit.h"
#include "StBTofRawHit.h"

/// @brief Event-level container for all STAR Barrel TOF hits, raw hits, and event header.
class StBTofCollection : public StObject {
public: 
    /// @brief Default constructor.
    StBTofCollection();
    /// @brief Destructor.
    ~StBTofCollection();

    /// @brief Returns a const pointer to the BTOF event header.
    const StBTofHeader*         tofHeader() const;
    /// @brief Returns a pointer to the BTOF event header.
    StBTofHeader*               tofHeader();

    /// @brief Returns a const reference to the BTOF hit collection.
    const StSPtrVecBTofHit&     tofHits() const;
    /// @brief Returns a reference to the BTOF hit collection.
    StSPtrVecBTofHit&           tofHits();

    /// @brief Returns a const reference to the BTOF raw hit collection.
    const StSPtrVecBTofRawHit&  tofRawHits() const;
    /// @brief Returns a reference to the BTOF raw hit collection.
    StSPtrVecBTofRawHit&        tofRawHits();

    /// @brief Sets the BTOF event header.
    void setHeader(StBTofHeader*);

    /// @brief Adds a reconstructed BTOF hit to the collection.
    void addHit(const StBTofHit*);
    /// @brief Adds a raw BTOF hit to the collection.
    void addRawHit(const StBTofRawHit*);

    /// @brief Returns true if the hit collection is non-empty.
    bool hitsPresent()     const;
    /// @brief Returns true if the raw hit collection is non-empty.
    bool rawHitsPresent()  const;
    
private:
    StBTofHeader*               mBTofHeader;  ///< Pointer to the BTOF event header

    StSPtrVecBTofHit            mBTofHits;     ///< Collection of reconstructed BTOF hits
    StSPtrVecBTofRawHit         mBTofRawHits;  ///< Collection of raw BTOF hits
  
    ClassDef(StBTofCollection, 1)
};

#endif
