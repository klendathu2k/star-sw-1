/*!
 * \class StRnDHitCollection 
 * \author Thomas Ullrich, Jan 2006
 */
/***************************************************************************
 *
 * $Id: StRnDHitCollection.h,v 2.1 2006/01/19 21:42:06 ullrich Exp $
 *
 * Author: Thomas Ullrich, Jan 2006
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StRnDHitCollection.h,v $
 * Revision 2.1  2006/01/19 21:42:06  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StRnDHitCollection_hh
#define StRnDHitCollection_hh

/// @file StRnDHitCollection.h
/// @brief Collection class for R&D (Research and Development) generic detector hits.

#include "StObject.h"
#include "StContainers.h"

class StRnDHit;

/// @brief Collection of R&D (Research and Development) generic detector hits.
class StRnDHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StRnDHitCollection();
    /// @brief Destructor.
    ~StRnDHitCollection();
    // StRnDHitCollection(const StRnDHitCollection&); use default
    // const StRnDHitCollection&
    // operator=(const StRnDHitCollection&);           use default
    
    /// @brief Returns a mutable reference to the hit pointer vector.
    StSPtrVecRnDHit&       hits();
    /// @brief Returns a const reference to the hit pointer vector.
    const StSPtrVecRnDHit& hits() const;

    /// @brief Add a hit to the collection; returns true on success.
    bool          addHit(StRnDHit*);
    /// @brief Returns the total number of hits in the collection.
    unsigned int  numberOfHits() const;
    
private:
    StSPtrVecRnDHit mHits;  ///< Container of R&D hit pointers.
    
    ClassDef(StRnDHitCollection,1)
};

inline const StSPtrVecRnDHit& StRnDHitCollection::hits() const { return mHits; }
inline StSPtrVecRnDHit& StRnDHitCollection::hits() { return mHits; }
inline unsigned int StRnDHitCollection::numberOfHits() const {return mHits.size();}

#endif
