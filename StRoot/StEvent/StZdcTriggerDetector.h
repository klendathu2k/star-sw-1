/*!
 * \class StZdcTriggerDetector
 * \author Thomas Ullrich, Sep 1999
 * \brief ZDC (Zero Degree Calorimeter) trigger detector ADC/TDC data for one event.
 */
/***************************************************************************
 *
 * $Id: StZdcTriggerDetector.h,v 2.11 2006/09/14 00:02:53 ullrich Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StZdcTriggerDetector.h,v $
 * Revision 2.11  2006/09/14 00:02:53  ullrich
 * Removed argument (run) in constructor. Not needed anymore.
 *
 * Revision 2.10  2006/08/21 19:43:35  ullrich
 * Run number becomes constructor argument. Needed for ZDC. (Akio)
 *
 * Revision 2.9  2004/04/06 19:39:44  ullrich
 * Added ZDC SMD support.
 *
 * Revision 2.8  2004/02/11 01:42:09  ullrich
 * Added new constructor to load data from StTriggerData.
 *
 * Revision 2.7  2002/02/22 22:56:53  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.6  2001/07/13 16:25:20  perev
 * last static array fixed
 *
 * Revision 2.5  2001/07/12 22:58:33  ullrich
 * Added variable to store the vertex_z from timing info.
 *
 * Revision 2.4  2001/04/05 04:00:47  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.3  2000/07/13 12:51:13  ullrich
 * Added new method numberOfZdcWords to replace old one with wrong name.
 *
 * Revision 2.2  1999/12/20 12:54:48  ullrich
 * Adapted changed in trigger table dst_TrgDet
 *
 * Revision 2.1  1999/10/13 19:44:22  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StZdcTriggerDetector_hh
#define StZdcTriggerDetector_hh

/// @file StZdcTriggerDetector.h
/// @brief ZDC (Zero Degree Calorimeter) trigger detector ADC/TDC data and SMD strip data.

#include "StObject.h"
#include "StEnumerations.h"

class dst_TrgDet_st;
class StTriggerData;

/// @brief Stores ZDC (Zero Degree Calorimeter) ADC sums, individual PMT ADC/TDC values,
///        Shower Maximum Detector (SMD) strip data, and a vertex-z estimate from ZDC timing.
///        The ZDC detects spectator neutrons in the very forward region (|eta|>6),
///        used for minimum-bias and centrality triggering in heavy-ion collisions.
class StZdcTriggerDetector : public StObject {
public:
    StZdcTriggerDetector();
    /// @brief Construct from legacy DST trigger detector table.
    StZdcTriggerDetector(const dst_TrgDet_st&);
    /// @brief Construct from decoded StTriggerData.
    StZdcTriggerDetector(const StTriggerData&);
    // StZdcTriggerDetector(const StZdcTriggerDetector&);            use default
    // StZdcTriggerDetector& operator=(const StZdcTriggerDetector&); use default
    virtual ~StZdcTriggerDetector();
    
    /// @brief Return the ADC sum for east or west ZDC arm.
    float         adcSum(StBeamDirection) const;
    /// @brief Return the combined (east + west) ZDC ADC sum.
    float         adcSum() const;
    /// @brief Return the number of ZDC ADC/TDC words stored.
    unsigned int  numberOfZdcWords() const;
    /// @brief Return the raw ADC value at hardware index @p i.
    float         adc(unsigned int) const;
    /// @brief Return the raw TDC value at hardware index @p i.
    float         tdc(unsigned int) const;
    /// @brief Return the z-vertex position (cm) estimated from ZDC timing.
    float         vertexZ() const;
    /// @brief Return the ZDC SMD strip ADC for given direction (east/west), orientation (vert=0/horiz=1), and strip @p strip.
    float         zdcSmd(StBeamDirection eastwest, int verthori, int strip) const;

    /// @brief Set the raw ADC value at hardware index @p i.
    void setAdc(unsigned int, float);
    /// @brief Set the raw TDC value at hardware index @p i.
    void setTdc(unsigned int, float);
    /// @brief Set the ADC sum for east or west arm.
    void setAdcSum(StBeamDirection, float);
    /// @brief Set the combined (east + west) ADC sum.
    void setAdcSum(float);
    /// @brief Set the z-vertex position (cm) from ZDC timing.
    void setVertexZ(float);
    /// @brief Set a ZDC SMD strip value for given direction, orientation, and strip.
    void setZdcSmd(StBeamDirection eastwest, int verthori, int strip, float val);

    /// @brief Deprecated: use numberOfZdcWords() instead.
    unsigned int   numberOfZdcCounters() const;  // usage depreciated, to be removed soon
    
protected:
    enum {mMaxZdcWords = 16}; ///< Maximum number of ZDC ADC/TDC words.
    Float_t  mAdc[mMaxZdcWords];        ///< Raw ADC values for each ZDC channel.
    Float_t  mTdc[mMaxZdcWords];        ///< Raw TDC values for each ZDC channel.
    Float_t  mSumAdc[2];                ///< ADC sum for east [0] and west [1] arms.
    Float_t  mSum;                      ///< Combined east+west ZDC ADC sum.
    Float_t  mVertexZ;                  ///< Z-vertex estimate from ZDC timing (cm).

    Float_t  mZdcSmdEast[mMaxZdcWords]; ///< ZDC SMD strip ADC values for the east arm.
    Float_t  mZdcSmdWest[mMaxZdcWords]; ///< ZDC SMD strip ADC values for the west arm.
    
    ClassDef(StZdcTriggerDetector,3)
};
#endif
