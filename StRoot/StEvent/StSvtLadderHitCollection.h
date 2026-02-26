/*!
 * \class StSvtLadderHitCollection 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StSvtLadderHitCollection.h,v 2.4 2002/02/22 22:56:51 jeromel Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSvtLadderHitCollection.h,v $
 * Revision 2.4  2002/02/22 22:56:51  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:43  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2000/02/17 18:13:19  ullrich
 * Changed the SVT hit storage model. Hits are now stored according
 * to barrel/ladder/wafer not by layer/ladder/wafer.
 *
 * Revision 2.1  1999/10/13 19:43:46  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StSvtLadderHitCollection_hh
#define StSvtLadderHitCollection_hh

/// @file StSvtLadderHitCollection.h
/// @brief Defines the StSvtLadderHitCollection class grouping SVT hits by ladder within a barrel.

#include "StObject.h"
#include "StSvtWaferHitCollection.h"

/// @brief Collection of SVT hits belonging to a single ladder, organized by wafer.
class StSvtLadderHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StSvtLadderHitCollection();
    /// @brief Destructor.
    ~StSvtLadderHitCollection();
    // StSvtLadderHitCollection(const StSvtLadderHitCollection&); use default
    // const StSvtLadderHitCollection&
    // operator=(const StSvtLadderHitCollection&);                use default
    
    /// @brief Returns the total number of hits across all wafers on this ladder.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of wafer sub-collections on this ladder.
    unsigned int  numberOfWafers() const;
    
    /// @brief Returns a pointer to the hit collection for the given wafer index.
    StSvtWaferHitCollection*       wafer(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given wafer index.
    const StSvtWaferHitCollection* wafer(unsigned int) const;

    /// @brief Sets the parent barrel index (used to configure wafer capacities).
    void setBarrelNumber(int);
    
private:
    enum { mMaxNumberOfWafers = 7 };
    Int_t                    mBarrelNumber;                    ///<  Parent barrel index [1-3].
    StSvtWaferHitCollection  mWafers[mMaxNumberOfWafers];      ///<  Array of wafer hit collections.
    
    ClassDef(StSvtLadderHitCollection,1)
};
#endif
