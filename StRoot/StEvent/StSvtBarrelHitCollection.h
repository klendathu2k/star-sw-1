/*!
 * \class StSvtBarrelHitCollection 
 * \author Thomas Ullrich, Feb 2000
 */
/***************************************************************************
 *
 * $Id: StSvtBarrelHitCollection.h,v 2.3 2002/02/22 22:56:50 jeromel Exp $
 *
 * Author: Thomas Ullrich, Feb 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSvtBarrelHitCollection.h,v $
 * Revision 2.3  2002/02/22 22:56:50  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:42  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  2000/02/17 18:15:11  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StSvtBarrelHitCollection_hh
#define StSvtBarrelHitCollection_hh

/// @file StSvtBarrelHitCollection.h
/// @brief Defines the StSvtBarrelHitCollection class grouping SVT hits by barrel.

#include "StObject.h"
#include "StSvtLadderHitCollection.h"

/// @brief Collection of SVT hits belonging to a single barrel, organized by ladder.
class StSvtBarrelHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StSvtBarrelHitCollection();
    /// @brief Destructor.
    ~StSvtBarrelHitCollection();
    // StSvtBarrelHitCollection(const StSvtBarrelHitCollection&); use default
    // const StSvtBarrelHitCollection&
    // operator=(const StSvtBarrelHitCollection&);                use default
    
    /// @brief Returns the total number of hits across all ladders in this barrel.
    unsigned int numberOfHits() const;
    /// @brief Returns the number of ladder sub-collections in this barrel.
    unsigned int  numberOfLadders() const;
    
    /// @brief Returns a pointer to the hit collection for the given ladder index.
    StSvtLadderHitCollection*       ladder(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given ladder index.
    const StSvtLadderHitCollection* ladder(unsigned int) const;

    /// @brief Sets the barrel index for this collection.
    void setBarrelNumber(int);
    
private:
    enum { mMaxNumberOfLadders = 16 };
    Int_t                    mBarrelNumber;                        ///<  Barrel index [1-3].
    StSvtLadderHitCollection mLadders[mMaxNumberOfLadders];        ///<  Array of ladder hit collections.
    
    ClassDef(StSvtBarrelHitCollection,1)
};
#endif
