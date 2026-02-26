/*!
 * \class StSsdLadderHitCollection 
 * \author Lilian Martin, Thomas Ullrich, Dec 1999
 */
/***************************************************************************
 *
 * $Id: StSsdLadderHitCollection.h,v 2.3 2002/02/22 22:56:50 jeromel Exp $
 *
 * Author: Lilian Martin, Thomas Ullrich, Dec 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSsdLadderHitCollection.h,v $
 * Revision 2.3  2002/02/22 22:56:50  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:42  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  2000/01/05 16:00:10  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StSsdLadderHitCollection_hh
#define StSsdLadderHitCollection_hh

/// @file StSsdLadderHitCollection.h
/// @brief Defines the StSsdLadderHitCollection class grouping SSD hits by ladder.

#include "StObject.h"
#include "StSsdWaferHitCollection.h"

/// @brief Collection of SSD hits belonging to a single ladder, organized by wafer.
class StSsdLadderHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StSsdLadderHitCollection();
    /// @brief Destructor.
    ~StSsdLadderHitCollection();
    // StSsdLadderHitCollection(const StSsdLadderHitCollection&); use default
    // const StSsdLadderHitCollection&
    // operator=(const StSsdLadderHitCollection&);                use default
    
    /// @brief Returns the total number of hits across all wafers on this ladder.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of wafer sub-collections on this ladder.
    unsigned int  numberOfWafers() const;
    
    /// @brief Returns a pointer to the hit collection for the given wafer index.
    StSsdWaferHitCollection*       wafer(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given wafer index.
    const StSsdWaferHitCollection* wafer(unsigned int) const;

private:
    enum { mMaxNumberOfWafers = 16 };
    StSsdWaferHitCollection  mWafers[mMaxNumberOfWafers];   ///<  Array of wafer hit collections [0-15].
    
    ClassDef(StSsdLadderHitCollection,1)
};
#endif
