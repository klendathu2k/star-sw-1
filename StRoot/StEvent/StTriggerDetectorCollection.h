/*!
 * \class StTriggerDetectorCollection
 * \author Thomas Ullrich, Sep 1999
 * \brief Aggregates trigger data from all STAR sub-detector trigger systems for one event.
 */
/***************************************************************************
 *
 * $Id: StTriggerDetectorCollection.h,v 2.9 2007/07/02 20:23:09 ullrich Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *`
 * $Log: StTriggerDetectorCollection.h,v $
 * Revision 2.9  2007/07/02 20:23:09  ullrich
 * Added FMS and MTD.
 *
 * Revision 2.8  2006/09/14 00:02:16  ullrich
 * Removed argument (run) in constructor. Not needed anymore.
 *
 * Revision 2.7  2006/08/21 19:43:35  ullrich
 * Run number becomes constructor argument. Needed for ZDC. (Akio)
 *
 * Revision 2.6  2005/10/10 22:27:42  ullrich
 * Added setXXX functions.
 *
 * Revision 2.5  2004/08/03 17:21:15  ullrich
 * Fpd as trigger detector added.
 *
 * Revision 2.4  2004/02/11 01:42:09  ullrich
 * Added new constructor to load data from StTriggerData.
 *
 * Revision 2.3  2002/02/22 22:56:53  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2002/02/20 03:12:15  ullrich
 * Added EMC trigger.
 *
 * Revision 2.1  2002/01/03 20:59:34  ullrich
 * Added BBC and FPD.
 *
 * Revision 2.0  1999/10/12 18:43:20  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StTriggerDetectorCollection_hh
#define StTriggerDetectorCollection_hh

/// @file StTriggerDetectorCollection.h
/// @brief Aggregates trigger data from all STAR sub-detector trigger systems for one event.

#include "StObject.h"
#include "StBbcTriggerDetector.h"
#include "StCtbTriggerDetector.h"
#include "StMwcTriggerDetector.h"
#include "StVpdTriggerDetector.h"
#include "StZdcTriggerDetector.h"
#include "StEmcTriggerDetector.h"
#include "StFpdTriggerDetector.h"
#include "StFmsTriggerDetector.h"
#include "StMtdTriggerDetector.h"

class dst_TrgDet_st;
class StTriggerData;

/// @brief Collection of trigger detector objects for all STAR triggering sub-systems in one event.
class StTriggerDetectorCollection : public StObject {
public:
    StTriggerDetectorCollection();
    /// @brief Construct from legacy DST trigger detector table.
    StTriggerDetectorCollection(const dst_TrgDet_st&);
    /// @brief Construct by unpacking all detector trigger data from @p StTriggerData.
    StTriggerDetectorCollection(const StTriggerData&);
    // StTriggerDetectorCollection(const StTriggerDetectorCollection&);            use default
    // StTriggerDetectorCollection& operator=(const StTriggerDetectorCollection&); use default
    virtual ~StTriggerDetectorCollection();
    
    /// @brief Access BBC (Beam-Beam Counter) trigger detector data.
    StBbcTriggerDetector&       bbc();
    const StBbcTriggerDetector& bbc() const;
    /// @brief Access CTB (Central Trigger Barrel) trigger detector data.
     StCtbTriggerDetector&      ctb();
    const StCtbTriggerDetector& ctb() const;
    /// @brief Access MWC (Multi-Wire Chamber) trigger detector data.
    StMwcTriggerDetector&       mwc();
    const StMwcTriggerDetector& mwc() const;
    /// @brief Access VPD (Vertex Position Detector) trigger detector data.
    StVpdTriggerDetector&       vpd();
    const StVpdTriggerDetector& vpd() const;
    /// @brief Access ZDC (Zero Degree Calorimeter) trigger detector data.
    StZdcTriggerDetector&       zdc();
    const StZdcTriggerDetector& zdc() const;
    /// @brief Access EMC (Electromagnetic Calorimeter) trigger detector data.
    StEmcTriggerDetector&       emc();
    const StEmcTriggerDetector& emc() const;
    /// @brief Access FPD (Forward Pion Detector) trigger detector data.
    StFpdTriggerDetector&       fpd();
    const StFpdTriggerDetector& fpd() const;
    /// @brief Access FMS (Forward Meson Spectrometer) trigger detector data.
    StFmsTriggerDetector&       fms();
    const StFmsTriggerDetector& fms() const;
    /// @brief Access MTD (Muon Telescope Detector) trigger detector data.
    StMtdTriggerDetector&       mtd();
    const StMtdTriggerDetector& mtd() const;

    /// @brief Replace the CTB trigger detector object.
    void setCtbTriggerDetector(const StCtbTriggerDetector&);
    /// @brief Replace the MWC trigger detector object.
    void setMwcTriggerDetector(const StMwcTriggerDetector&);
    /// @brief Replace the VPD trigger detector object.
    void setVpdTriggerDetector(const StVpdTriggerDetector&);
    /// @brief Replace the ZDC trigger detector object.
    void setZdcTriggerDetector(const StZdcTriggerDetector&);
    /// @brief Replace the BBC trigger detector object.
    void setBbcTriggerDetector(const StBbcTriggerDetector&);
    /// @brief Replace the EMC trigger detector object.
    void setEmcTriggerDetector(const StEmcTriggerDetector&);
    /// @brief Replace the FPD trigger detector object.
    void setFpdTriggerDetector(const StFpdTriggerDetector&);
    /// @brief Replace the FMS trigger detector object.
    void setFmsTriggerDetector(const StFmsTriggerDetector&);
    /// @brief Replace the MTD trigger detector object.
    void setMtdTriggerDetector(const StMtdTriggerDetector&);
    
protected:
    StCtbTriggerDetector mCtb; ///< CTB (Central Trigger Barrel) trigger data.
    StMwcTriggerDetector mMwc; ///< MWC (Multi-Wire Chamber) trigger data.
    StVpdTriggerDetector mVpd; ///< VPD (Vertex Position Detector) trigger data.
    StZdcTriggerDetector mZdc; ///< ZDC (Zero Degree Calorimeter) trigger data.
    StBbcTriggerDetector mBbc; ///< BBC (Beam-Beam Counter) trigger data.
    StEmcTriggerDetector mEmc; ///< EMC (Electromagnetic Calorimeter) trigger data.
    StFpdTriggerDetector mFpd; ///< FPD (Forward Pion Detector) trigger data.
    StFmsTriggerDetector mFms; ///< FMS (Forward Meson Spectrometer) trigger data.
    StMtdTriggerDetector mMtd; ///< MTD (Muon Telescope Detector) trigger data.
    
    ClassDef(StTriggerDetectorCollection,6)
};

inline void StTriggerDetectorCollection::setCtbTriggerDetector(const StCtbTriggerDetector& val) {mCtb = val;}
inline void StTriggerDetectorCollection::setMwcTriggerDetector(const StMwcTriggerDetector& val) {mMwc = val;}
inline void StTriggerDetectorCollection::setVpdTriggerDetector(const StVpdTriggerDetector& val) {mVpd = val;}
inline void StTriggerDetectorCollection::setZdcTriggerDetector(const StZdcTriggerDetector& val) {mZdc = val;}
inline void StTriggerDetectorCollection::setBbcTriggerDetector(const StBbcTriggerDetector& val) {mBbc = val;}
inline void StTriggerDetectorCollection::setEmcTriggerDetector(const StEmcTriggerDetector& val) {mEmc = val;}
inline void StTriggerDetectorCollection::setFpdTriggerDetector(const StFpdTriggerDetector& val) {mFpd = val;}
inline void StTriggerDetectorCollection::setFmsTriggerDetector(const StFmsTriggerDetector& val) {mFms = val;}
inline void StTriggerDetectorCollection::setMtdTriggerDetector(const StMtdTriggerDetector& val) {mMtd = val;}

#endif
