/*!
 * \class StL3Trigger
 * \author Thomas Ullrich, Apr 2000
 * \brief Level-3 online trigger object containing online-reconstructed tracks, vertices, and algorithm results.
 */
/***************************************************************************
 *
 * $Id: StL3Trigger.h,v 2.4 2002/02/22 22:56:49 jeromel Exp $
 *
 * Author: Thomas Ullrich, Apr 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StL3Trigger.h,v $
 * Revision 2.4  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/08/02 01:27:45  ullrich
 * Added event summary and algorithms.
 *
 * Revision 2.2  2001/04/05 04:00:38  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  2000/03/29 16:53:11  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StL3Trigger_hh
#define StL3Trigger_hh

/// @file StL3Trigger.h
/// @brief Level-3 online trigger object with online-reconstructed TPC tracks, vertices, and algorithm summaries.

#include "StContainers.h"
#include "TArrayL.h"

class StTrackNode;
class StTpcHitCollection;
class StTrackDetectorInfo;
class StPrimaryVertex;
class StL3EventSummary;

/// @brief Level-3 (L3) online trigger object.  Stores the online-reconstructed TPC hits,
///        track nodes, primary vertices, and the per-algorithm decision summary produced by
///        the L3 farm during data-taking.
class StL3Trigger : public StObject {
public:
    StL3Trigger();
    ~StL3Trigger();

    /// @brief Return the L3 event summary (algorithm decisions and counters).
    StL3EventSummary*                   l3EventSummary();
    const StL3EventSummary*             l3EventSummary() const;

    /// @brief Return the online TPC hit collection.
    StTpcHitCollection*                 tpcHitCollection();
    const StTpcHitCollection*           tpcHitCollection() const;

    /// @brief Return the collection of online track detector-info objects.
    StSPtrVecTrackDetectorInfo&         trackDetectorInfo();
    const StSPtrVecTrackDetectorInfo&   trackDetectorInfo() const;
    
    /// @brief Return the collection of online track nodes.
    StSPtrVecTrackNode&                 trackNodes();
    const StSPtrVecTrackNode&           trackNodes() const;

    /// @brief Return the number of online primary vertices found.
    unsigned int                        numberOfPrimaryVertices() const;
    /// @brief Return the online primary vertex at index @p i (default 0 = best vertex).
    StPrimaryVertex*                    primaryVertex(unsigned int = 0);
    const StPrimaryVertex*              primaryVertex(unsigned int = 0) const;

    /// @brief Set the L3 event summary object (takes ownership).
    void setL3EventSummary(StL3EventSummary*);
    /// @brief Set the online TPC hit collection (takes ownership).
    void setTpcHitCollection(StTpcHitCollection*);
    /// @brief Add an online primary vertex.
    void addPrimaryVertex(StPrimaryVertex*);
    
protected:
    StL3EventSummary*            mL3EventSummary;       ///< L3 algorithm decision summary.
    StTpcHitCollection*          mL3TpcHits;            ///< Online TPC hit collection.
    StSPtrVecTrackDetectorInfo   mL3TrackDetectorInfo;  ///< Online track detector-info list.
    StSPtrVecTrackNode           mL3TrackNodes;         ///< Online track nodes.
    StSPtrVecPrimaryVertex       mL3PrimaryVertices;    ///< Online primary vertices.
    TArrayL                      mTriggerWords;         ///< L3 trigger words array.

private:
    StL3Trigger(const StL3Trigger&);
    StL3Trigger& operator=(const StL3Trigger&);

    ClassDef(StL3Trigger,1)
};
#endif
