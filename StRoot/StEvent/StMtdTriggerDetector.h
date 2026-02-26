/***************************************************************************
 *
 * $Id: StMtdTriggerDetector.h,v 2.1 2007/07/02 20:21:55 ullrich Exp $
 *
 * Author: Akio Agawa, July 2007
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StMtdTriggerDetector.h,v $
 * Revision 2.1  2007/07/02 20:21:55  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StMtdTriggerDetector_hh
#define StMtdTriggerDetector_hh

/// @file StMtdTriggerDetector.h
/// @brief Trigger detector data from the STAR Muon Telescope Detector (MTD) ADC/TDC channels.

#include "StObject.h"
#include "StEnumerations.h"

class StTriggerData;

/// @brief Trigger detector data (ADC/TDC) from the STAR Muon Telescope Detector (MTD).
class StMtdTriggerDetector : public StObject {
public:
    /// @brief Default constructor.
    StMtdTriggerDetector();
    /// @brief Constructs from raw trigger data.
    StMtdTriggerDetector(const StTriggerData&);
    /// @brief Destructor.
    virtual ~StMtdTriggerDetector();
    
    /// @brief Returns the total number of MTD trigger counters.
    unsigned int   numberOfMtdCounters() const;
    /// @brief Returns the ADC value for the specified direction and counter.
    unsigned short adc(StBeamDirection eastwest, unsigned int pmt) const;
    /// @brief Returns the TDC value for the specified direction and counter.
    unsigned short tdc(StBeamDirection eastwest, unsigned int pmt) const;

    /// @brief Sets the ADC value for the specified direction and counter.
    void setAdc(StBeamDirection eastwest, unsigned int pmt, unsigned short v);
    /// @brief Sets the TDC value for the specified direction and counter.
    void setTdc(StBeamDirection eastwest, unsigned int pmt, unsigned short v);

protected:
    enum {mMaxMtdCounter=8};
    unsigned short mADC[2][mMaxMtdCounter];  ///< ADC values per direction (east=0, west=1) and counter
    unsigned short mTDC[2][mMaxMtdCounter];  ///< TDC values per direction (east=0, west=1) and counter
    
    ClassDef(StMtdTriggerDetector,1)
};
#endif
