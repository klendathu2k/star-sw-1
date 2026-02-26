/***************************************************************************
 *
 * $Id: StETofCollection.h,v 2.1 2018/07/09 14:53:47 ullrich Exp $
 *
 * Author: Florian Seck, April 2018
 ***************************************************************************
 *
 * Description: This class collects eTOF objects for persistent
 * storage in StEvent. All eTOF stuff goes here expect the PiDTraits:
 *              - StETofDigi
 *              - StETofHit
 *              - StETofHeader
 *
 ***************************************************************************
 *
 * $Log: StETofCollection.h,v $
 * Revision 2.1  2018/07/09 14:53:47  ullrich
 * Initial Revision.
 *
 *
 ***************************************************************************/
#ifndef STETOFCOLLECTION_H
#define STETOFCOLLECTION_H

/// @file StETofCollection.h
/// @brief Event-level collection of Endcap Time of Flight (eTOF) detector data.

#include "StObject.h"
#include "StETofDigi.h"
#include "StETofHit.h"
#include "StETofHeader.h"

#include "StContainers.h"
#include "StEnumerations.h"

/// @brief Event-level container for all STAR Endcap TOF (eTOF) digis, hits, and event header.
class StETofCollection : public StObject {
public: 
    /// @brief Default constructor.
    StETofCollection();
    /// @brief Destructor.
    ~StETofCollection();

    /// @brief Returns a const pointer to the eTOF event header.
    const StETofHeader*         etofHeader() const;
    /// @brief Returns a pointer to the eTOF event header.
    StETofHeader*               etofHeader();

    /// @brief Returns a const reference to the eTOF digi collection.
    const StSPtrVecETofDigi&    etofDigis() const;
    /// @brief Returns a reference to the eTOF digi collection.
    StSPtrVecETofDigi& 	        etofDigis();

    /// @brief Returns a const reference to the eTOF hit collection.
    const StSPtrVecETofHit&     etofHits() const;
    /// @brief Returns a reference to the eTOF hit collection.
    StSPtrVecETofHit&           etofHits();

    /// @brief Sets the eTOF event header.
    void setHeader( StETofHeader* );

    /// @brief Adds a digi to the eTOF digi collection.
    void addDigi( const StETofDigi* );
    /// @brief Adds a hit to the eTOF hit collection.
    void addHit(  const StETofHit*  );

    /// @brief Returns true if the digi collection is non-empty.
    bool digisPresent()	   const;
    /// @brief Returns true if the hit collection is non-empty.
    bool hitsPresent()     const;

private:
    StETofHeader*       mETofHeader;  ///< Pointer to the eTOF event header

    StSPtrVecETofDigi   mETofDigis;   ///< Collection of eTOF digi objects
    StSPtrVecETofHit    mETofHits;    ///< Collection of reconstructed eTOF hits


    ClassDef( StETofCollection, 1 )
};

#endif // STETOFCOLLECTION_H

