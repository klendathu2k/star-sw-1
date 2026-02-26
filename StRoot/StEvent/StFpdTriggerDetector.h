/***************************************************************************
 *
 * $Id: StFpdTriggerDetector.h,v 2.2 2007/07/11 23:06:45 perev Exp $
 *
 * Author: Akio Ogawa, Jul 2004
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StFpdTriggerDetector.h,v $
 * Revision 2.2  2007/07/11 23:06:45  perev
 * Cleanup+fix StXXXTriggerDetector
 *
 * Revision 2.1  2004/08/03 17:20:33  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StFpdTriggerDetector_hh
#define StFpdTriggerDetector_hh

/// @file StFpdTriggerDetector.h
/// @brief Legacy FPD (Forward Pion Detector) trigger detector data.

#include "StObject.h"
#include "StEnumerations.h"

class dst_TrgDet_st;
class StTriggerData;

/// @brief Stores FPD trigger data including per-tower ADC values and DSM layer-1/2 sums.
class StFpdTriggerDetector : public StObject {
public:
    /// @brief Default constructor.
    StFpdTriggerDetector();
    /// @brief Construct from legacy trigger detector structure.
    StFpdTriggerDetector(const dst_TrgDet_st&);
    /// @brief Construct from raw trigger data.
    StFpdTriggerDetector(const StTriggerData&);
    StFpdTriggerDetector(const StFpdTriggerDetector&);           
    StFpdTriggerDetector& operator=(const StFpdTriggerDetector&);
    /// @brief Destructor.
    ~StFpdTriggerDetector();

    /// @brief Clear all data.
    void clear();
    /// @brief Dump all trigger data to standard output.
    void dump() const;
    /// @brief Return the number of towers for module @p nstbps.
    unsigned int  numberOfTowers(unsigned int nstbps) const;
    /// @brief Return the total number of FPD modules.
    unsigned int  numberOfModules() const;
    /// @brief Return the number of DSM layer-1 boards.
    unsigned int  numberOfLayer1Boards() const;

    /// @brief Return tower ADC for a given east/west side, module, and tower index.
    unsigned int adc(StBeamDirection eastwest, unsigned int nstbps, unsigned int tower) const;
    /// @brief Return DSM layer-1 sum for a given side, module, and board.
    unsigned int layer1(StBeamDirection eastwest, unsigned int nstbps, unsigned int board) const;
    /// @brief Return DSM layer-2 sum for a given side and module.
    unsigned int layer2(StBeamDirection eastwest, unsigned int nstbps) const;

    /// @brief Set a tower ADC value.
    void setAdc(StBeamDirection eastwest, unsigned int nstbps, unsigned int tower, unsigned char v);
    /// @brief Set a DSM layer-1 sum.
    void setLayer1(StBeamDirection eastwest, unsigned int nstbps, unsigned int board, unsigned short v);
    /// @brief Set a DSM layer-2 sum.
    void setLayer2(StBeamDirection eastwest, unsigned int nstbps, unsigned short v);
        
protected:
    enum {mMaxNS = 49, mMaxTB = 25, mMaxPS =  7, mMaxModule = 6, mMaxBoard = 4};

private: 
    void init();

private:    
    char mBeg[1];//!
    unsigned int mMaxTower[mMaxModule];//!
    UChar_t*     mAdc[2][mMaxModule];  //!

    UChar_t  mEN[mMaxNS]; ///< East north module ADC values.
    UChar_t  mES[mMaxNS]; ///< East south module ADC values.
    UChar_t  mET[mMaxTB]; ///< East top module ADC values.
    UChar_t  mEB[mMaxTB]; ///< East bottom module ADC values.
    UChar_t  mEPN[mMaxPS]; ///< East pre-shower north ADC values.
    UChar_t  mEPS[mMaxPS]; ///< East pre-shower south ADC values.
    UChar_t  mWN[mMaxNS]; ///< West north module ADC values.
    UChar_t  mWS[mMaxNS]; ///< West south module ADC values.
    UChar_t  mWT[mMaxTB]; ///< West top module ADC values.
    UChar_t  mWB[mMaxTB]; ///< West bottom module ADC values.
    UChar_t  mWPN[mMaxPS]; ///< West pre-shower north ADC values.
    UChar_t  mWPS[mMaxPS]; ///< West pre-shower south ADC values.
    UShort_t mLayer1[2][mMaxModule][mMaxBoard];    ///< DSM layer-1 sums [east/west][module][board].
    UShort_t mLayer2[2][mMaxModule];               ///< DSM layer-2 sums [east/west][module].
    char mEnd[1];//!

    ClassDef(StFpdTriggerDetector,2)
};
#endif
