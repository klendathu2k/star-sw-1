/*!
 * \class StBbcTriggerDetector
 * \author Akio Ogawa, Jan 2002
 * \brief BBC (Beam-Beam Counter) trigger detector data for one event.
 */
/***************************************************************************
 *
 * $Id: StBbcTriggerDetector.h,v 2.10 2008/08/15 18:36:18 ullrich Exp $
 *
 * Author: Akio Ogawa, Jan 2002
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StBbcTriggerDetector.h,v $
 * Revision 2.10  2008/08/15 18:36:18  ullrich
 * Minor change, move zVertex().
 *
 * Revision 2.9  2007/07/11 23:06:45  perev
 * Cleanup+fix StXXXTriggerDetector
 *
 * Revision 2.8  2007/04/24 14:52:23  ullrich
 * Fixed bug in BBC unpacking (Akio).
 *
 * Revision 2.7  2004/08/03 17:22:16  ullrich
 * Major update by Akio and Marco.
 *
 * Revision 2.6  2004/02/11 01:42:09  ullrich
 * Added new constructor to load data from StTriggerData.
 *
 * Revision 2.5  2003/01/23 23:23:29  ullrich
 * Modified to cope with changes in how BBC data is loaded for Run3.
 *
 * Revision 2.4  2002/10/17 02:07:49  akio
 * Increase # of PMT from 32 to 48 for 2003 run.
 * Schema evolution should take care for reading old files.
 * Some of access functions (for east west separately, or sums)
 * will give wrong answer if you are using new 2003 mapping.
 * One should access data through raw adc values until improvements.
 * These functions do correct job for 2002 data.
 *
 * Revision 2.3  2002/02/22 22:56:46  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2002/01/28 19:28:10  jeromel
 * Wrong format of an enum list corrected. Solaris picked this up.
 *
 * Revision 2.1  2002/01/03 20:57:37  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StBbcTriggerDetector_hh
#define StBbcTriggerDetector_hh

/// @file StBbcTriggerDetector.h
/// @brief BBC (Beam-Beam Counter) trigger detector ADC/TDC data and derived quantities.

#include "StObject.h"

class dst_TrgDet_st;
class StTriggerData;

/// @brief Stores ADC, TDC, scalar, and pedestal data read out from the BBC
///        (Beam-Beam Counter) trigger detector for one event.
///        The BBC consists of small and large scintillator tiles on each side
///        of the interaction point, used for luminosity monitoring and minimum-bias triggering.
///        Note: some legacy accessor methods are unsupported for Run 3+ data.
class StBbcTriggerDetector : public StObject {
public:
    StBbcTriggerDetector();
    /// @brief Construct from legacy DST trigger detector table.
    StBbcTriggerDetector(const dst_TrgDet_st&);
    /// @brief Construct from decoded StTriggerData.
    StBbcTriggerDetector(const StTriggerData&);
    virtual ~StBbcTriggerDetector();
    // StBbcTriggerDetector(const StBbcTriggerDetector&);            use default
    // StBbcTriggerDetector& operator=(const StBbcTriggerDetector&); use default
    
    /// @brief Return the total number of BBC PMT channels.
    unsigned int   numberOfPMTs() const;
    /// @brief Return the number of hardware register words.
    unsigned int   numberOfRegisters() const;
    /// @brief Return the number of pedestal data words.
    unsigned int   numberOfPedestalData() const;
    /// @brief Return the number of scalar (rate counter) words.
    unsigned int   numberOfScalars() const;
    
    /// @brief Return the ADC value for PMT at index @p i.
    unsigned short adc(unsigned int) const;
    /// @brief Return the TDC value for PMT at index @p i.
    unsigned short tdc(unsigned int) const;
    /// @brief Return the BBC hardware register word at index @p i.
    unsigned short bbcRegister(unsigned int) const;
    /// @brief Return the pedestal data word at index @p i.
    unsigned short pedestalData(unsigned int) const;
    /// @brief Return the scalar (rate counter) value at index @p i.
    unsigned int   scalar(unsigned int) const;
    
    /// @brief Return the pedestal mean for PMT @p id.
    unsigned short pedestal(unsigned int id) const;
    /// @brief Return the pedestal width (sigma) for PMT @p id.
    unsigned short pedestalWidth(unsigned int id) const;
    /// @brief Return the MIP (minimum-ionizing particle) peak position for PMT @p id.
    unsigned short mip(unsigned int id) const;
    /// @brief Return the MIP peak width for PMT @p id.
    unsigned short mipWidth(unsigned int id) const;
    

    /// @brief Return the ADC sum over east small tiles.
    int   adcSumEast(); 
    /// @brief Return the ADC sum over west small tiles.
    int   adcSumWest();
    /// @brief Return the ADC sum over east large tiles.
    int   adcSumEastLarge(); 
    /// @brief Return the ADC sum over west large tiles.
    int   adcSumWestLarge();
    /// @brief Return the ADC sum over all small tiles (east + west).
    int   adcSum();
    /// @brief Return the ADC sum over all large tiles (east + west).
    int   adcSumLarge();
    /// @brief Return the ADC sum over all BBC tiles.
    int   adcSumAll();
    /// @brief Return the online time difference (z-vertex proxy) from the layer-2 DSM.
    unsigned short onlineTimeDifference() const; // z vertex from Layer2 DSM in channel 

  /////////////////////////////////////////////
  // No longer supported after 2003. Do not use
    int   nHitEast();
    int   nHitWest();
    int   nHitEastLarge();
    int   nHitWestLarge();
    int   nHit();
    int   nHitLarge();
    int   nHitAll();
    int   tdcEarliestEast(); // 2002 = common start = smaller channel is earlier
    int   tdcEarliestWest(); // 2003 = common stop  = larger channel os earlier
  // No longer supported after 2003. Do not use
  //////////////////////////////////////////////

    /// @brief Return the z-vertex position (cm) estimated from BBC timing.
    float zVertex(); //z vertex in cm

    /// @brief Set the ADC value for PMT at index @p i.
    void  setAdc(unsigned int, unsigned short);
    /// @brief Set the TDC value for PMT at index @p i.
    void  setTdc(unsigned int, unsigned short);
    /// @brief Set the hardware register word at index @p i.
    void  setRegister(unsigned int, unsigned short);
    /// @brief Set the pedestal data word at index @p i.
    void  setPedestal(unsigned int, unsigned short);
    /// @brief Set the scalar value at index @p i.
    void  setScalar(unsigned int, unsigned int);
    /// @brief Set the online time difference word from the layer-2 DSM.
    void  setOnlineTimeDifference(unsigned short);
    /// @brief Print BBC data to stdout for debugging.
    void  dump();
    
    /// @brief Return the run year encoded in this object.
    unsigned int   year() const;
    /// @brief Set the run year for this object.
    void           setYear(unsigned int);
    
protected:
    enum {
        mMaxPMTs = 48,     ///< Maximum number of BBC PMT channels.
        mMaxRegisters = 2, ///< Number of BBC hardware register words.
        mMaxPedData = 128, ///< Number of pedestal data words.
        mMaxScalars = 32   ///< Number of scalar (rate counter) words.
    };
    char mBeg[1];//!
    UShort_t mAdc[mMaxPMTs];       ///< ADC values for each BBC PMT.
    UShort_t mTdc[mMaxPMTs];       ///< TDC values for each BBC PMT.
    UShort_t mReg[mMaxRegisters];  ///< BBC hardware register words.
    UShort_t mPed[mMaxPedData];    ///< Pedestal data (mean, width, MIP peak, MIP width).
    UShort_t mScl[mMaxScalars];    ///< Scalar (rate counter) values.
    UInt_t   mYear;                ///< Run year for this data object.
    UInt_t   mDSMVTX;              ///< Online time difference from DSM layer-2 (z-vertex proxy).
    char mEnd[1];//!
    ClassDef(StBbcTriggerDetector,5)
};

inline unsigned int
StBbcTriggerDetector::numberOfPMTs() const {return mMaxPMTs;}

inline unsigned int
StBbcTriggerDetector::numberOfRegisters() const {return mMaxRegisters;}

inline unsigned int
StBbcTriggerDetector::numberOfPedestalData() const {return mMaxPedData;}

inline unsigned int
StBbcTriggerDetector::numberOfScalars() const {return mMaxScalars;}
        
inline unsigned short
StBbcTriggerDetector::pedestal(unsigned int id) const {return pedestalData(id);}

inline unsigned short
StBbcTriggerDetector::pedestalWidth(unsigned int id) const {return pedestalData(id+32);}

inline unsigned short
StBbcTriggerDetector::mip(unsigned int id) const {return pedestalData(id+64);}

inline unsigned short
StBbcTriggerDetector::mipWidth(unsigned int id) const {return pedestalData(id+96);}

inline unsigned int
StBbcTriggerDetector::year() const {return mYear;}

inline void
StBbcTriggerDetector::setYear(unsigned int v) {mYear = v;}

inline unsigned short
StBbcTriggerDetector::onlineTimeDifference() const {return mDSMVTX;}

inline void
StBbcTriggerDetector::setOnlineTimeDifference(unsigned short v) {mDSMVTX = v;}

#endif
