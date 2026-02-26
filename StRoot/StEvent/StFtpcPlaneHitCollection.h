/*!
 * \class StFtpcPlaneHitCollection 
 * \author Thomas Ullrich, July 1999
 */
/***************************************************************************
 *
 * $Id: StFtpcPlaneHitCollection.h,v 2.3 2002/02/22 22:56:48 jeromel Exp $
 *
 * Author: Thomas Ullrich, July 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StFtpcPlaneHitCollection.h,v $
 * Revision 2.3  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:37  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  1999/10/13 19:43:09  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StFtpcPlaneHitCollection_hh
#define StFtpcPlaneHitCollection_hh

/// @file StFtpcPlaneHitCollection.h
/// @brief Defines the StFtpcPlaneHitCollection class grouping FTPC hits by readout plane.

#include "StObject.h"
#include "StFtpcSectorHitCollection.h"

/// @brief Collection of FTPC hits belonging to a single readout plane, organized by sector.
class StFtpcPlaneHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StFtpcPlaneHitCollection();
    // StFtpcPlaneHitCollection(const StFtpcPlaneHitCollection&);            use default
    // StFtpcPlaneHitCollection& operator=(const StFtpcPlaneHitCollection&); use default
    /// @brief Destructor.
    ~StFtpcPlaneHitCollection();
    
    /// @brief Returns the total number of hits across all sectors in this plane.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of sector sub-collections in this plane.
    unsigned int  numberOfSectors() const;
    
    /// @brief Returns a pointer to the hit collection for the given sector index.
    StFtpcSectorHitCollection*       sector(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given sector index.
    const StFtpcSectorHitCollection* sector(unsigned int) const;

private:
    enum { mNumberOfSectors = 6 };
    StFtpcSectorHitCollection mSectors[mNumberOfSectors];   ///<  Array of sector hit collections [0-5].
    
    ClassDef(StFtpcPlaneHitCollection,1)
};
#endif
