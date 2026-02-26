/*!
 * \class StEmcTriggerDetector 
 * \author Alex Suaide, Feb 2002
 */
/***************************************************************************
 *
 * $Id: StEmcTriggerDetector.h,v 2.5 2007/07/11 23:06:45 perev Exp $
 *
 * Author: Alex Suaide, Feb 2002
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEmcTriggerDetector.h,v $
 * Revision 2.5  2007/07/11 23:06:45  perev
 * Cleanup+fix StXXXTriggerDetector
 *
 * Revision 2.4  2004/08/03 17:22:16  ullrich
 * Major update by Akio and Marco.
 *
 * Revision 2.3  2004/02/11 01:42:09  ullrich
 * Added new constructor to load data from StTriggerData.
 *
 * Revision 2.2  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.1  2002/02/20 03:11:46  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StEmcTriggerDetector_hh
#define StEmcTriggerDetector_hh

/// @file StEmcTriggerDetector.h
/// @brief Level-0 DSM trigger quantities derived from the STAR Barrel and Endcap EMC.

#include "StObject.h"
#include "StEnumerations.h"

class dst_TrgDet_st;
class StTriggerData;

/// @brief Level-0 DSM trigger quantities (high-tower ADC and patch sums) from the STAR Barrel and Endcap EMC.
class StEmcTriggerDetector : public StObject {
public:
    StEmcTriggerDetector();
    /// @brief Constructs trigger quantities from the legacy dst_TrgDet_st trigger data structure.
    StEmcTriggerDetector(const dst_TrgDet_st&);
    /// @brief Constructs trigger quantities from the online StTriggerData object.
    StEmcTriggerDetector(const StTriggerData&);
    // StEmcTriggerDetector(const StEmcTriggerDetector&);            use default
    // StEmcTriggerDetector& operator=(const StEmcTriggerDetector&); use default
    ~StEmcTriggerDetector();

    /// @brief Returns the total number of BEMC trigger towers.
    int   numberOfTowers() const;
    
    /// @brief Returns the highest-tower ADC in the given BEMC trigger patch.
    int   highTower(unsigned int) const;
    /// @brief Returns the ADC sum of the given BEMC trigger patch.
    int   patch(unsigned int) const;
    /// @brief Returns the highest-tower ADC in the given EEMC trigger patch.
    int   highTowerEndcap(unsigned int) const;
    /// @brief Returns the ADC sum of the given EEMC trigger patch.
    int   patchEndcap(unsigned int) const;
    /// @brief Returns the BEMC DSM layer-1 word at the given index.
    unsigned short bemcLayer1(int idx) const;
    /// @brief Returns the EEMC DSM layer-1 word at the given index.
    unsigned short eemcLayer1(int idx) const;
    /// @brief Returns the combined EMC DSM layer-2 word at the given index.
    unsigned short emcLayer2(int idx) const;

    /// @brief Sets the highest-tower ADC for the given BEMC trigger patch.
    void  setHighTower(unsigned int, int);
    /// @brief Sets the ADC sum for the given BEMC trigger patch.
    void  setPatch(unsigned int, int);
    /// @brief Sets the highest-tower ADC for the given EEMC trigger patch.
    void  setHighTowerEndcap(unsigned int, int);
    /// @brief Sets the ADC sum for the given EEMC trigger patch.
    void  setPatchEndcap(unsigned int, int);
        
protected:
    enum {mNPatch = 300,      ///< Number of BEMC trigger patches.
          mENPatch = 90,      ///< Number of EEMC trigger patches.
          mNBemcLayer1 = 48,  ///< Number of BEMC DSM layer-1 input words.
	  mNEemcLayer1 = 16,  ///< Number of EEMC DSM layer-1 input words.
          mNEmcLayer2 = 8};   ///< Number of combined EMC DSM layer-2 input words.

private:    
    // Layer0 DSM input (from detector)
    char mBeg[1];//!
    Char_t  mHighTower[mNPatch];  ///< Highest-tower ADC per BEMC trigger patch (DSM layer-0 input).
    Char_t  mPatch[mNPatch];      ///< ADC sum per BEMC trigger patch (DSM layer-0 input).
    
    Char_t mEHighTower[mENPatch]; ///< Highest-tower ADC per EEMC trigger patch (DSM layer-0 input).
    Char_t mEPatch[mENPatch];     ///< ADC sum per EEMC trigger patch (DSM layer-0 input).

    // Higher DSM layer inputs (mainly for experts/debugging)
    unsigned short mBemcLayer1[mNBemcLayer1]; ///< BEMC DSM layer-1 input words (for expert/debugging use).
    unsigned short mEemcLayer1[mNEemcLayer1]; ///< EEMC DSM layer-1 input words (for expert/debugging use).
    unsigned short mEmcLayer2[mNEmcLayer2];   ///< Combined EMC DSM layer-2 input words (for expert/debugging use).
    char mEnd[1];//!;
    
    ClassDef(StEmcTriggerDetector,3)
};
#endif
