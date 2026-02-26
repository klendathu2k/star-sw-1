/*!
 * \class StTpcHitCollection 
 * \author Thomas Ullrich, July 1999
 */
/***************************************************************************
 *
 * $Id: StTpcHitCollection.h,v 2.7 2019/04/02 15:32:49 smirnovd Exp $
 *
 * Author: Thomas Ullrich, July 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTpcHitCollection.h,v $
 * Revision 2.7  2019/04/02 15:32:49  smirnovd
 * Add iterator to loop over StTpcHits in StTpcHitContainer
 *
 * Revision 2.6  2019/04/02 15:32:42  smirnovd
 * Add accessors to StTpcHitContainer
 *
 * Revision 2.5  2019/04/02 15:32:33  smirnovd
 * Remove commented code
 *
 * Revision 2.4  2009/11/23 16:34:07  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.3  2002/02/22 22:56:52  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:44  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  1999/10/13 19:44:00  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StTpcHitCollection_hh
#define StTpcHitCollection_hh

/// @file StTpcHitCollection.h
/// @brief Top-level collection of all TPC hits, organised by sector and padrow.

#include <iterator>

#include "StObject.h"
#include "StTpcSectorHitCollection.h"

class StTpcHit;

/// @brief Top-level TPC hit collection containing hits from all 24 sectors.
class StTpcHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StTpcHitCollection();
    /// @brief Destructor.
    ~StTpcHitCollection();

    /// @brief Add a hit to the appropriate sector and padrow sub-collection.
    bool          addHit(StTpcHit*);
    /// @brief Returns the total number of TPC hits across all sectors and padrows.
    unsigned int  numberOfHits() const;
    /// @brief Returns the fixed number of TPC sectors (24).
    unsigned int  numberOfSectors() const {return mNumberOfSectors; }
    /// @brief Returns the number of padrows in the given sector (1-based index).
    unsigned int  numberOfPadrows(int sectorId) const { return sector(sectorId) ? sector(sectorId)->numberOfPadrows() : 0; }

    /// @brief Returns a mutable pointer to the hit collection for the given sector (1-based).
    StTpcSectorHitCollection*       sector(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given sector (1-based).
    const StTpcSectorHitCollection* sector(unsigned int) const;

    /// @brief Returns a const pointer to the hit vector for the given sector and padrow (1-based).
    const StSPtrVecTpcHit* hits(int sectorId, int padrowId) const;

    /// An iterator over StTpcHits in a StTpcHitCollection
    class StTpcHitIter : public std::iterator<std::input_iterator_tag, StTpcHit*>
    {
    public:
      /// @brief Returns an iterator pointing to the first hit in the collection.
      static StTpcHitIter begin(StTpcHitCollection& c);
      /// @brief Returns a past-the-end sentinel iterator for the collection.
      static StTpcHitIter end(StTpcHitCollection& c);

      /// @brief Advances the iterator to the next hit, skipping empty padrows/sectors.
      StTpcHitIter& operator++();
      /// @brief Returns true when two iterators refer to the same position.
      bool operator==(const StTpcHitIter &other) const;
      /// @brief Returns true when two iterators refer to different positions.
      bool operator!=(const StTpcHitIter &other) const;
      /// @brief Dereferences the iterator to obtain a const pointer to the current hit.
      const StTpcHit* operator*() const;

    private:
      /// @brief Constructs an iterator at a given sector/padrow/hit index within the collection.
      StTpcHitIter(StTpcHitCollection& c, int l1=0, int l2=0) : coll(c), iSector(l1), iPadrow(l2), iHit(0) {}

      StTpcHitCollection& coll;    ///< Reference to the parent collection being iterated.
      std::size_t iSector = 0;     ///< Current sector index (0-based).
      std::size_t iPadrow = 0;     ///< Current padrow index within the current sector (0-based).
      std::size_t iHit = 0;        ///< Current hit index within the current padrow (0-based).
    };

    /// @brief Returns a forward iterator to the first TPC hit in the collection.
    StTpcHitIter begin() { return StTpcHitIter::begin(*this); }
    /// @brief Returns a past-the-end iterator for the collection.
    StTpcHitIter end()   { return StTpcHitIter::end(*this); }

private:
    enum { mNumberOfSectors = 24 };
    StTpcSectorHitCollection mSectors[mNumberOfSectors]; ///< Per-sector hit collections (24 sectors).
    
    ClassDef(StTpcHitCollection,1)
};
#endif
