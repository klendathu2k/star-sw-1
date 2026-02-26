/*!
 * \class StTofCollection 
 * \author Thomas Ullrich, Dec 2000
 */
/***************************************************************************
 *
 * $Id: StTofCollection.h,v 2.10 2008/06/03 17:41:05 ullrich Exp $
 *
 * Author: Thomas Ullrich, Dec 2000
 ***************************************************************************
 *
 * Description:
 *
 * Persistent data which is written into StEvent
 * directly from the reco chain. All ToF stuff goes here
 * except the StTofPidTraits and the StTofSoftwareMonitor.
 *
 ***************************************************************************
 *
 * $Log: StTofCollection.h,v $
 * Revision 2.10  2008/06/03 17:41:05  ullrich
 * Add new member vzVpd and related access methods.
 *
 * Revision 2.9  2007/04/03 18:16:48  ullrich
 * Add new data members and methods in preperation for new ToF.
 *
 * Revision 2.8  2005/04/11 22:35:25  calderon
 * Tof Classes for Run 5.  Modifications and additions from Xin to
 * take care of new TOF daq and electronics.  Added StTofRawData and
 * modified containers and includes.
 *
 * Revision 2.7  2003/05/23 20:06:12  ullrich
 * Restore plural for data members.
 *
 * Revision 2.6  2003/05/23 16:10:59  ullrich
 * Changed name of access functions.
 *
 * Revision 2.5  2003/05/21 18:22:46  ullrich
 * Major Revision of ToF classes (F. Geurts)
 *
 * Revision 2.4  2002/02/22 22:56:51  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/10/01 19:40:58  ullrich
 * Added methods and members for StTofData.
 *
 * Revision 2.2  2001/04/24 18:20:13  ullrich
 * Added hits and slats to collection.
 *
 * Revision 2.1  2000/12/08 03:52:43  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StTofCollection_hh
#define StTofCollection_hh

/// @file StTofCollection.h
/// @brief Event-level collection of legacy STAR Time of Flight (TOF) detector data.

#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"
#include "StTofHit.h"
#include "StTofCell.h"
#include "StTofSlat.h"
#include "StTofData.h"
#include "StTofRawData.h"

/// @brief Event-level container for all legacy STAR TOF cells, slats, hits, and raw data.
class StTofCollection : public StObject {
public: 
    /// @brief Default constructor.
    StTofCollection();
    /// @brief Destructor.
    ~StTofCollection();

    /// @brief Returns the number of fired VPD east tubes.
    unsigned int    numberOfVpdEast() const;
    /// @brief Returns the number of fired VPD west tubes.
    unsigned int    numberOfVpdWest() const;
    /// @brief Returns the VPD east hit-pattern bitmask.
    unsigned int    vpdEast() const;
    /// @brief Returns the VPD west hit-pattern bitmask.
    unsigned int    vpdWest() const;
    /// @brief Returns the event start time from the VPD [ns].
    float           tstart() const;
    /// @brief Returns the VPD east–west time difference [ns].
    float           tdiff() const;
    /// @brief Returns the primary vertex z from the VPD [cm].
    float           vzVpd() const;

    /// @brief Sets the VPD east hit-pattern bitmask.
    void setVpdEast(unsigned int);
    /// @brief Sets the VPD west hit-pattern bitmask.
    void setVpdWest(unsigned int);
    /// @brief Sets the event start time from the VPD [ns].
    void setTstart(float);
    /// @brief Sets the VPD east–west time difference [ns].
    void setTdiff(float);
    /// @brief Sets the primary vertex z from the VPD [cm].
    void setVzVpd(float);

    /// @brief Returns a const reference to the TOF cell collection.
    const StSPtrVecTofCell&    tofCells() const;
    /// @brief Returns a reference to the TOF cell collection.
    StSPtrVecTofCell&          tofCells();

    /// @brief Returns a const reference to the TOF slat collection.
    const StSPtrVecTofSlat&    tofSlats() const;
    /// @brief Returns a reference to the TOF slat collection.
    StSPtrVecTofSlat&          tofSlats();
    
    /// @brief Returns a const reference to the TOF hit collection.
    const StSPtrVecTofHit&     tofHits() const;
    /// @brief Returns a reference to the TOF hit collection.
    StSPtrVecTofHit&           tofHits();

    /// @brief Returns a const reference to the TOF data collection.
    const StSPtrVecTofData&    tofData() const;
    /// @brief Returns a reference to the TOF data collection.
    StSPtrVecTofData&          tofData();

    /// @brief Returns a const reference to the TOF raw data collection.
    const StSPtrVecTofRawData&    tofRawData() const;
    /// @brief Returns a reference to the TOF raw data collection.
    StSPtrVecTofRawData&          tofRawData();

    /// @brief Adds a TOF slat to the collection.
    void addSlat(const StTofSlat*);
    /// @brief Adds a TOF cell to the collection.
    void addCell(const StTofCell*);
    /// @brief Adds a TOF hit to the collection.
    void addHit(const StTofHit*);
    /// @brief Adds a TOF data object to the collection.
    void addData(const StTofData*); 
    /// @brief Adds a TOF raw data object to the collection.
    void addRawData(const StTofRawData*);

    /// @brief Returns true if the cell collection is non-empty.
    bool cellsPresent()    const;
    /// @brief Returns true if the slat collection is non-empty.
    bool slatsPresent()    const;
    /// @brief Returns true if the hit collection is non-empty.
    bool hitsPresent()     const;
    /// @brief Returns true if the data collection is non-empty.
    bool dataPresent()     const;
    /// @brief Returns true if the raw data collection is non-empty.
    bool rawdataPresent()  const;
    
private:
    UInt_t   mVpdEast;  ///< VPD east hit-pattern bitmask
    UInt_t   mVpdWest;  ///< VPD west hit-pattern bitmask
    Float_t  mTstart;   ///< Event start time from VPD [ns]
    Float_t  mTdiff;    ///< VPD east–west time difference [ns]
    Float_t  mVzVpd;    ///< Primary vertex z position from VPD [cm]

    StSPtrVecTofSlat           mTofSlats;    ///< Collection of TOF slat objects
    StSPtrVecTofCell           mTofCells;    ///< Collection of TOF cell objects
    StSPtrVecTofHit            mTofHits;     ///< Collection of reconstructed TOF hits
    StSPtrVecTofData           mTofData;     ///< Collection of TOFp raw data objects
    StSPtrVecTofRawData        mTofRawData;  ///< Collection of TOF raw TDC data objects
  
    ClassDef(StTofCollection, 6)
};

inline unsigned int StTofCollection::vpdEast() const { return mVpdEast; }

inline unsigned int StTofCollection::vpdWest() const { return mVpdWest; }

inline void StTofCollection::setVpdEast(unsigned int i) { mVpdEast = i; }

inline void StTofCollection::setVpdWest(unsigned int i) { mVpdWest = i; }

inline float StTofCollection::tstart() const { return mTstart; }

inline float StTofCollection::tdiff() const { return mTdiff; }

inline float StTofCollection::vzVpd() const { return mVzVpd; }

inline void StTofCollection::setTstart(float t) { mTstart = t; }

inline void StTofCollection::setTdiff(float t)  { mTdiff = t;}

inline void StTofCollection::setVzVpd(float vz) { mVzVpd = vz; }

#endif
