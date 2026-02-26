/*!
 * \class StSvtHitCollection 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StSvtHitCollection.h,v 2.4 2002/02/22 22:56:51 jeromel Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSvtHitCollection.h,v $
 * Revision 2.4  2002/02/22 22:56:51  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:43  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2000/02/17 18:13:14  ullrich
 * Changed the SVT hit storage model. Hits are now stored according
 * to barrel/ladder/wafer not by layer/ladder/wafer.
 *
 * Revision 2.1  1999/10/13 19:43:44  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StSvtHitCollection_hh
#define StSvtHitCollection_hh

/// @file StSvtHitCollection.h
/// @brief Defines the StSvtHitCollection class, the top-level SVT hit container.

#include "StObject.h"
#include "StSvtBarrelHitCollection.h"
class StSvtHit;

/// @brief Top-level collection of all reconstructed SVT hits, organized by barrel.
class StSvtHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StSvtHitCollection();
    /// @brief Destructor.
    ~StSvtHitCollection();
    // StSvtHitCollection(const StSvtHitCollection&);            use default
    // StSvtHitCollection& operator=(const StSvtHitCollection&); use default
    
    /// @brief Adds a hit to the appropriate barrel/ladder/wafer sub-collection.
    bool          addHit(StSvtHit*);
    /// @brief Returns the total number of hits across all barrels.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of barrel sub-collections.
    unsigned int  numberOfBarrels() const;
    
    /// @brief Returns a pointer to the hit collection for the given barrel index.
    StSvtBarrelHitCollection*       barrel(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given barrel index.
    const StSvtBarrelHitCollection* barrel(unsigned int) const;

private:
    enum { mNumberOfBarrels = 3 };
    StSvtBarrelHitCollection mBarrels[mNumberOfBarrels]; ///<  Array of barrel hit collections [0-2].
    
    ClassDef(StSvtHitCollection,1)
};
#endif
