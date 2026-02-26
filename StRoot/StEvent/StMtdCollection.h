/*!
 * \class StMtdCollection 
 */
/***************************************************************************
 *
 * $Id: StMtdCollection.h,v 2.2 2012/02/28 01:25:32 perev Exp $
 *
 * Author: Frank Geurts, April 25, 2011
 ***************************************************************************
 *
 * Description:
 *
 * Persistent data which is written into StEvent
 * directly from the reco chain.
 *
 ***************************************************************************
 *
 * $Log: StMtdCollection.h,v $
 * Revision 2.2  2012/02/28 01:25:32  perev
 * Browse(...) added
 *
 * Revision 2.1  2011/04/25 21:24:02  ullrich
 * Initial Revision.
 *
 *
 **************************************************************************/
#ifndef StMtdCollection_hh
#define StMtdCollection_hh

/// @file StMtdCollection.h
/// @brief Event-level collection of Muon Telescope Detector (MTD) data.

#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"
#include "StMtdHeader.h"
#include "StMtdHit.h"
#include "StMtdRawHit.h"
class TBrowser;

/// @brief Event-level container for all STAR Muon Telescope Detector (MTD) hits and raw hits.
class StMtdCollection : public StObject {
public: 
    /// @brief Default constructor.
    StMtdCollection();
    /// @brief Destructor.
    ~StMtdCollection();

    /// @brief Returns true so the collection appears as a folder in TBrowser.
    bool IsFolder() const { return true;};
    /// @brief Browses the collection contents in a TBrowser.
    void Browse(TBrowser *b); 

    /// @brief Returns a const pointer to the MTD event header.
    const StMtdHeader*         mtdHeader() const;
    /// @brief Returns a pointer to the MTD event header.
    StMtdHeader*               mtdHeader();

    /// @brief Returns a const reference to the MTD hit collection.
    const StSPtrVecMtdHit&     mtdHits() const;
    /// @brief Returns a reference to the MTD hit collection.
    StSPtrVecMtdHit&           mtdHits();

    /// @brief Returns a const reference to the MTD raw hit collection.
    const StSPtrVecMtdRawHit&  mtdRawHits() const;
    /// @brief Returns a reference to the MTD raw hit collection.
    StSPtrVecMtdRawHit&        mtdRawHits();

    /// @brief Sets the MTD event header.
    void setHeader(StMtdHeader*);

    /// @brief Adds a reconstructed MTD hit to the collection.
    void addHit(const StMtdHit*);
    /// @brief Adds a raw MTD hit to the collection.
    void addRawHit(const StMtdRawHit*);

    /// @brief Returns true if the hit collection is non-empty.
    bool hitsPresent()     const;
    /// @brief Returns true if the raw hit collection is non-empty.
    bool rawHitsPresent()  const;
    
private:
    StMtdHeader*               mMtdHeader;  ///< Pointer to the MTD event header

    StSPtrVecMtdHit            mMtdHits;     ///< Collection of reconstructed MTD hits
    StSPtrVecMtdRawHit         mMtdRawHits;  ///< Collection of raw MTD hits
  
    ClassDef(StMtdCollection, 1)
};

#endif
