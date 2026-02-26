/*!
 * \class StVpdTriggerDetector 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StVpdTriggerDetector.h,v 2.5 2013/10/30 15:47:16 ullrich Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StVpdTriggerDetector.h,v $
 * Revision 2.5  2013/10/30 15:47:16  ullrich
 * Added ADCmxq(), TDCmxq() and referring data member (WJL).
 *
 * Revision 2.4  2007/04/03 20:11:41  ullrich
 * Modified for actual VPD used in 2007.
 *
 * Revision 2.3  2002/02/22 22:56:53  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:47  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  1999/10/13 19:44:20  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StVpdTriggerDetector_hh
#define StVpdTriggerDetector_hh

/// @file StVpdTriggerDetector.h
/// @brief Trigger detector data from the STAR Vertex Position Detector (VPD).

#include "StObject.h"
#include "StEnumerations.h"
 
class dst_TrgDet_st;
class StTriggerData;

/// @brief Trigger detector data (ADC/TDC) from the STAR Vertex Position Detector (VPD).
class StVpdTriggerDetector : public StObject {
public:
    /// @brief Default constructor.
    StVpdTriggerDetector();
    /// @brief Constructs from legacy dst_TrgDet_st trigger data structure.
    StVpdTriggerDetector(const dst_TrgDet_st&);
    /// @brief Constructs from StTriggerData.
    StVpdTriggerDetector(const StTriggerData&);
    // StVpdTriggerDetector& operator=(const StVpdTriggerDetector&); use default
    /// @brief Destructor.
    virtual ~StVpdTriggerDetector();
    
    /// @brief Returns the total number of VPD counters.
    unsigned int   numberOfVpdCounters() const;
    /// @brief Returns the ADC value for the specified direction and tube.
    unsigned short ADC(StBeamDirection eastwest, unsigned int pmt) const;
    /// @brief Returns the TDC value for the specified direction and tube.
    unsigned short TDC(StBeamDirection eastwest, unsigned int pmt) const;
    /// @brief Returns the ADC multiple-hit qualifier value for the specified direction and tube.
    unsigned short ADCmxq(StBeamDirection eastwest, unsigned int pmt) const;		//WJL
    /// @brief Returns the TDC multiple-hit qualifier value for the specified direction and tube.
    unsigned short TDCmxq(StBeamDirection eastwest, unsigned int pmt) const;		//WJL
    /// @brief Returns the earliest TDC value for the specified direction.
    unsigned short EarliestTDC(StBeamDirection eastwest) const;
    /// @brief Returns the VPD east–west time difference [TDC counts].
    unsigned short TimeDifference() const {return mTimeDifference;};

    /// @brief Sets the ADC value for the specified direction and tube.
    void setADC(StBeamDirection eastwest, unsigned int pmt, unsigned short v);
    /// @brief Sets the TDC value for the specified direction and tube.
    void setTDC(StBeamDirection eastwest, unsigned int pmt, unsigned short v);
    /// @brief Sets the ADC multiple-hit qualifier value for the specified direction and tube.
    void setADCmxq(StBeamDirection eastwest, unsigned int pmt, unsigned short v);		//WJL
    /// @brief Sets the TDC multiple-hit qualifier value for the specified direction and tube.
    void setTDCmxq(StBeamDirection eastwest, unsigned int pmt, unsigned short v);		//WJL
    /// @brief Sets the earliest TDC value for the specified direction.
    void setEarliestTDC(StBeamDirection eastwest, unsigned short v) {mEarliestTDC[eastwest]=v;}
    /// @brief Sets the VPD east–west time difference [TDC counts].
    void setTimeDifference(unsigned short v) {mTimeDifference = v;}   

protected:
    enum {mMaxVpdCounter = 16};
    unsigned short mADC[2][mMaxVpdCounter];      ///< ADC values per direction and tube
    unsigned short mTDC[2][mMaxVpdCounter];      ///< TDC values per direction and tube
    unsigned short mADCmxq[2][mMaxVpdCounter];   ///< ADC multiple-hit qualifier per direction and tube //WJL
    unsigned short mTDCmxq[2][mMaxVpdCounter];   ///< TDC multiple-hit qualifier per direction and tube //WJL
    unsigned short mEarliestTDC[2];              ///< Earliest TDC value per direction (east=0, west=1)
    unsigned short mTimeDifference;              ///< VPD east–west time difference [TDC counts]
    unsigned short mYear;                        ///< Run year for data format selection

    ClassDef(StVpdTriggerDetector,3)
};
#endif
