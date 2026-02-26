/*!
 * \class StFtpcHitCollection 
 * \author Thomas Ullrich, Aug 1999
 */
/***************************************************************************
 *
 * $Id: StFtpcHitCollection.h,v 2.3 2002/02/22 22:56:48 jeromel Exp $
 *
 * Author: Thomas Ullrich, Aug 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StFtpcHitCollection.h,v $
 * Revision 2.3  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:36  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  1999/10/13 19:43:06  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StFtpcHitCollection_hh
#define StFtpcHitCollection_hh

/// @file StFtpcHitCollection.h
/// @brief Defines the StFtpcHitCollection class, the top-level FTPC hit container.

#include "StObject.h"
#include "StFtpcPlaneHitCollection.h"
class StFtpcHit;

/// @brief Top-level collection of all reconstructed FTPC hits, organized by readout plane.
class StFtpcHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StFtpcHitCollection();
    // StFtpcHitCollection(const StFtpcHitCollection&);            use default
    // StFtpcHitCollection& operator=(const StFtpcHitCollection&); use default
    /// @brief Destructor.
    ~StFtpcHitCollection();
    
    /// @brief Adds a hit to the appropriate plane/sector sub-collection.
    bool          addHit(StFtpcHit*);
    /// @brief Returns the total number of hits across all planes.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of readout plane sub-collections.
    unsigned int  numberOfPlanes() const;
    
    /// @brief Returns a pointer to the hit collection for the given plane index.
    StFtpcPlaneHitCollection*       plane(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given plane index.
    const StFtpcPlaneHitCollection* plane(unsigned int) const;

private:
    enum { mNumberOfPlanes = 20 };
    StFtpcPlaneHitCollection mPlanes[mNumberOfPlanes];   ///<  Array of plane hit collections [0-19].
    
    ClassDef(StFtpcHitCollection,1)
};
#endif
