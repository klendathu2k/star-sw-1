/*!
 * \class StSsdHitCollection 
 * \author Lilian Martin, Thomas Ullrich, Dec 1999
 */
/***************************************************************************
 *
 * $Id: StSsdHitCollection.h,v 2.3 2002/02/22 22:56:50 jeromel Exp $
 *
 * Author: Lilian Martin, Thomas Ullrich, Dec 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSsdHitCollection.h,v $
 * Revision 2.3  2002/02/22 22:56:50  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:42  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  2000/01/05 16:00:07  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StSsdHitCollection_hh
#define StSsdHitCollection_hh

/// @file StSsdHitCollection.h
/// @brief Defines the StSsdHitCollection class, the top-level SSD hit container.

#include "StObject.h"
#include "StSsdLadderHitCollection.h"
class StSsdHit;

/// @brief Top-level collection of all reconstructed SSD hits, organized by ladder.
class StSsdHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StSsdHitCollection();
    /// @brief Destructor.
    ~StSsdHitCollection();
    // StSsdHitCollection(const StSsdHitCollection&);            use default
    // StSsdHitCollection& operator=(const StSsdHitCollection&); use default
    
    /// @brief Adds a hit to the appropriate ladder/wafer sub-collection.
    bool          addHit(StSsdHit*);
    /// @brief Returns the total number of hits across all ladders.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of ladder sub-collections.
    unsigned int  numberOfLadders() const;
    
    /// @brief Returns a pointer to the hit collection for the given ladder index.
    StSsdLadderHitCollection*       ladder(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given ladder index.
    const StSsdLadderHitCollection* ladder(unsigned int) const;

private:
    enum { mNumberOfLadders = 20 };
    StSsdLadderHitCollection mLadders[mNumberOfLadders];   ///<  Array of ladder hit collections [0-19].
    
    ClassDef(StSsdHitCollection,1)
};
#endif
