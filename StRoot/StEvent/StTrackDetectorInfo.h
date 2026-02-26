/*!
 * \class StTrackDetectorInfo 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StTrackDetectorInfo.h,v 2.16 2015/05/19 20:09:40 perev Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 * Note the following: with the arrival of ITTF it is now possible to
 * store the numberOfPoints for every detector individually. Before
 * that and because of the way the tables were defined TPC and FTPC were
 * one and the same. This caused confusion. However, since we have to
 * stay backwards compatible the "old way" is still working.
 * If
 * a) mNumberOfPoints == 0 the new encoding is the one to use, i.e.,
 *    mNumberOfPointsTpc
 *    mNumberOfPointsFtpcWest 
 *    mNumberOfPointsFtpcEast 
 *    mNumberOfPointsSvt 
 *    mNumberOfPointsSsd 
 *    are the ones that count
 * b) mNumberOfPoints != 0 then we still loaded the info from
 *    the tables and all is as it was before, i.e., we do not distinguish
 *    between FTPC and TPC.
 *
 ***************************************************************************
 *
 * $Log: StTrackDetectorInfo.h,v $
 * Revision 2.16  2015/05/19 20:09:40  perev
 * added Ist & Pxl
 *
 * Revision 2.15  2012/08/03 23:42:24  perev
 * ClassDef vers++
 *
 * Revision 2.14  2012/07/21 03:33:54  perev
 * Add Other hits
 *
 * Revision 2.13  2009/11/23 16:34:07  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.12  2004/10/13 16:11:59  ullrich
 * Added optional arg to addHit() to allow NOT to increase ref counter.
 *
 * Revision 2.11  2004/08/05 22:23:32  ullrich
 * Fixed bug in first argument type of setNumberOfPoints().
 *
 * Revision 2.10  2004/08/05 19:25:03  ullrich
 * Changes to the handling of numberOfPoints() to allow ITTF more flexibility.
 *
 * Revision 2.9  2004/07/15 16:36:26  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.8  2003/10/30 20:07:32  perev
 * Check of quality added
 *
 * Revision 2.7  2002/02/22 22:56:52  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.6  2001/04/05 04:00:45  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.5  2001/03/24 03:35:00  perev
 * clone() -> clone() const
 *
 * Revision 2.4  2000/04/20 13:30:02  ullrich
 * Added new methods and removed inconsistencies in numberOfPoints().
 *
 * Revision 2.3  1999/11/01 12:45:12  ullrich
 * Modified unpacking of point counter
 *
 * Revision 2.2  1999/10/28 22:27:30  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:44:11  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StTrackDetectorInfo_hh
#define StTrackDetectorInfo_hh
/// @file StTrackDetectorInfo.h
/// @brief Stores hit references and per-detector hit-count summaries for a reconstructed track.

#include "StContainers.h"
#include "StObject.h"
#include "StThreeVectorF.hh"
#include "StEnumerations.h"

class StHitFilter;
class StHit;

/// @brief Stores hit references and per-detector hit-count summaries for a reconstructed track.
///
/// StTrackDetectorInfo is shared between a StGlobalTrack and its sibling
/// StPrimaryTrack objects (both reference the same instance through the
/// StTrackNode).  It records the first/last hit positions, per-detector
/// hit counts, and owning pointers to the actual StHit objects.
class StTrackDetectorInfo : public StObject {
public:
    /// @brief Default constructor; initialises all counts to zero.
    StTrackDetectorInfo();
    // StTrackDetectorInfo(const StTrackDetectorInfo&);             use default
    // StTrackDetectorInfo & operator=(const StTrackDetectorInfo&); use default
    virtual ~StTrackDetectorInfo();

    /// @brief Global position of the innermost hit on the track (cm).
    const StThreeVectorF& firstPoint() const;
    /// @brief Global position of the outermost hit on the track (cm).
    const StThreeVectorF& lastPoint()  const;

    /// @brief Total number of measured hits on the track (all detectors combined).
    unsigned short        numberOfPoints() const;
    /// @brief Number of hits in the specified detector.
    unsigned short        numberOfPoints(StDetectorId) const;
    			 
    /// @brief Number of hits explicitly linked via hit pointers (all detectors).
    unsigned short        numberOfReferencedPoints() const;
    /// @brief Number of linked hits in the specified detector.
    unsigned short        numberOfReferencedPoints(StDetectorId) const;
    
    /// @brief All hits from the specified detector associated with this track.
    StPtrVecHit           hits(StDetectorId) const;
    /// @brief All hits passing the supplied filter.
    StPtrVecHit           hits(StHitFilter&) const;
    /// @brief Non-const reference to the full hit pointer vector.
    StPtrVecHit&          hits();
    /// @brief Const reference to the full hit pointer vector.
    const StPtrVecHit&    hits() const;

    /// @brief Set the position of the innermost hit (cm).
    void setFirstPoint(const StThreeVectorF&);
    /// @brief Set the position of the outermost hit (cm).
    void setLastPoint(const StThreeVectorF&);
    /// @brief Set hit count for the specified detector.
    void setNumberOfPoints(unsigned char, StDetectorId);
    /// @brief Add a hit to this detector info; optionally increment reference counter.
    void addHit(StHit*, bool = true);
    /// @brief Remove a hit from this detector info.
    void removeHit(StHit*&);
    /// @brief Returns non-zero if the detector info is invalid.
    int  bad() const;
    
protected:
    void setNumberOfPoints(unsigned short);  // obsolete since ITTF
    
private:
    StThreeVectorF mFirstPoint;             ///< Global position of the innermost hit (cm).
    StThreeVectorF mLastPoint;              ///< Global position of the outermost hit (cm).
    UShort_t       mNumberOfPoints;         ///< Total hit count (obsolete since ITTF; use per-detector members).
    UChar_t        mNumberOfPointsTpc;      ///< Number of TPC hits.
    UChar_t        mNumberOfPointsFtpcWest; ///< Number of FTPC west hits.
    UChar_t        mNumberOfPointsFtpcEast; ///< Number of FTPC east hits.
    UChar_t        mNumberOfPointsSvt;      ///< Number of SVT hits.
    UChar_t        mNumberOfPointsSsd;      ///< Number of SSD hits.
    UChar_t        mNumberOfPointsOth;      ///< Number of hits in other detectors.
    UChar_t        mNumberOfPointsIst;      ///< Number of IST hits.
    UChar_t        mNumberOfPointsPxl;      ///< Number of PXL hits.

    StPtrVecHit    mHits;                   ///< Non-owning pointers to all hits associated with this track.

    ClassDef(StTrackDetectorInfo,4)
};

#endif
