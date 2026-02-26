/// @brief FMS (Forward Meson Spectrometer) trigger detector: stores QT ADC/TDC data and DSM trigger sums.
/***************************************************************************
 *
 * $Id: StFmsTriggerDetector.h,v 2.6 2010/01/13 17:51:55 ullrich Exp $
 *
 * Author: Akio Ogawa, Apr 2007
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StFmsTriggerDetector.h,v $
 * Revision 2.6  2010/01/13 17:51:55  ullrich
 * New clearFlag() for mudst reading, Data member mNumHeader gets //!
 *
 * Revision 2.4  2009/02/23 22:29:57  ullrich
 * Fixed problem when running over 2009 data (solution by Pibero)
 *
 * Revision 2.3  2007/12/11 18:11:13  ullrich
 * Fix bugs in QT decoding (Akio).
 *
 * Revision 2.2  2007/07/11 23:06:45  perev
 * Cleanup+fix StXXXTriggerDetector
 *
 * Revision 2.1  2007/07/02 20:21:55  ullrich
 * Initial Revision.
 *
 *
 **************************************************************************/
#ifndef StFmsTriggerDetector_hh
#define StFmsTriggerDetector_hh

/// @file StFmsTriggerDetector.h
/// @brief FMS trigger detector data: QT board ADC/TDC and DSM outputs.

#include "StObject.h"

class StTriggerData;

/// @brief Stores FMS trigger data including QT board ADC/TDC values and DSM layer outputs.
class StFmsTriggerDetector : public StObject {
public: 
    /// @brief Default constructor.
    StFmsTriggerDetector();
    /// @brief Constructor: populate from raw trigger data.
    StFmsTriggerDetector(const StTriggerData&);
    /// @brief Destructor.
    virtual ~StFmsTriggerDetector();
    // StFmsTriggerDetector(const StFmsTriggerDetector&);            use default
    // StFmsTriggerDetector& operator=(const StFmsTriggerDetector&); use default

    /// @brief Clear the flag used to indicate valid data.
    void clearFlag();
 
    /// @brief Return the number of QT data words (hits).
    unsigned int   nHit() const;
    /// @brief Return raw QT data word at the given index.
    unsigned int   hit(int line) const;
    /// @brief Return decoded ADC value for the given crate/address/daughter-card/channel.
    unsigned short adc(int crate,  int addr,  int dcard,  int dch);
    /// @brief Return decoded TDC value for the given crate/address/daughter-card/channel.
    unsigned short tdc(int crate,  int addr,  int dcard,  int dch);

    unsigned char  DSM(int ch) const {return mDSM[ch];}   ///< Return DSM layer-0 output byte for channel @p ch.
    unsigned char  DSM01(int ch) const {return mDSM01[ch];} ///< Return DSM01 output byte for channel @p ch.
    unsigned char  DSM02(int ch) const {return mDSM02[ch];} ///< Return DSM02 output byte for channel @p ch.
    unsigned short DSM1(int ch) const {return mDSM1[ch];}  ///< Return DSM layer-1 output word for channel @p ch.
    unsigned short DSM2(int ch) const {return mDSM2[ch];}  ///< Return DSM layer-2 output word for channel @p ch.

    /// @brief Dump all trigger data to standard output for debugging.
    void  dump();
        
protected:
    /// @brief Decode raw QT data words into ADC/TDC arrays.
    void decode();
    unsigned short getNHT(int) const; ///< Extract hit-count field from a QT word.
    unsigned short getADR(int) const; ///< Extract address field from a QT word.
    unsigned short getCRT(int) const; ///< Extract crate field from a QT word.
    unsigned short getADC(int) const; ///< Extract ADC field from a QT word.
    unsigned short getTDC(int) const; ///< Extract TDC field from a QT word.
    unsigned short getQT8(int) const; ///< Extract QT8 field from a QT word.
    unsigned short getCHA(int) const; ///< Extract channel field from a QT word. 
    
protected:
    enum {
      mMaxLine     = 1600,
      mMaxDSM      = 256,
      mMaxDSM01    = 112,
      mMaxDSM02    = 16,
      mMaxDSM1     = 16,
      mMaxDSM2     = 8,
      mMaxCrate    = 4,
      mMaxAddr     = 16,
      mMaxDCard    = 4,
      mMaxChan     = 8,
      mOffsetCrate = 11,
      mOffsetAddr  = 16
    }; //!
    
    char mBeg[1];//!
    UInt_t   mNumQTdata;  ///< Number of valid QT data words.
    UInt_t   mQTdata[mMaxLine]; ///< Raw QT data words from the FMS electronics.
    Char_t   mDSM[mMaxDSM];   ///< DSM layer-0 output bytes.
    Char_t   mDSM01[mMaxDSM01]; ///< DSM01 output bytes.
    Char_t   mDSM02[mMaxDSM02]; ///< DSM02 output bytes.
    UShort_t mDSM1[mMaxDSM1];   ///< DSM layer-1 output words.
    UShort_t mDSM2[mMaxDSM2];   ///< DSM layer-2 output words.

    int  mNumHeader; //!
    unsigned short mADC[mMaxCrate][mMaxAddr][mMaxDCard][mMaxChan]; //!
    unsigned short mTDC[mMaxCrate][mMaxAddr][mMaxDCard][mMaxChan]; //!
    char mEnd[1];//!

    ClassDef(StFmsTriggerDetector,4)      
};

inline unsigned int   StFmsTriggerDetector::nHit() const {return mNumQTdata;} 
inline unsigned short StFmsTriggerDetector::getNHT(int v) const {return (unsigned short)  (v & 0x000000FF); }
inline unsigned short StFmsTriggerDetector::getADR(int v) const {return (unsigned short) ((v & 0x001F0000) >> 16);}
inline unsigned short StFmsTriggerDetector::getCRT(int v) const {return (unsigned short) ((v & 0xFF000000) >> 24);}
inline unsigned short StFmsTriggerDetector::getADC(int v) const {return (unsigned short)  (v & 0x00000FFF);}
inline unsigned short StFmsTriggerDetector::getTDC(int v) const {return (unsigned short) ((v & 0x001F0000) >> 16);}
inline unsigned short StFmsTriggerDetector::getQT8(int v) const {return (unsigned short) ((v & 0x18000000) >> 27);}
inline unsigned short StFmsTriggerDetector::getCHA(int v) const {return (unsigned short) ((v & 0xE0000000) >> 29);} 

#endif
