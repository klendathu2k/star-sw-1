/*!
 * \class StL0Trigger
 * \author Thomas Ullrich, Sep 1999
 * \brief Level-0 (hardware) trigger data for a STAR event.
 */
/***************************************************************************
 *
 * $Id: StL0Trigger.h,v 2.10 2005/10/10 19:24:38 ullrich Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StL0Trigger.h,v $
 * Revision 2.10  2005/10/10 19:24:38  ullrich
 * run number range check, and updated for run5.
 *
 * Revision 2.9  2004/08/03 17:22:16  ullrich
 * Major update by Akio and Marco.
 *
 * Revision 2.8  2003/07/29 19:50:53  ullrich
 * Fix for spin bits added.
 *
 * Revision 2.7  2002/11/26 02:19:11  perev
 * StEventMaker ITTF modif
 *
 * Revision 2.6  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.5  2002/02/15 00:18:13  ullrich
 * Changed signature of bunchCrossingId7bit().
 *
 * Revision 2.4  2002/01/09 15:37:12  ullrich
 * Bunch crossing id and spin bit extraction functions added.
 *
 * Revision 2.3  2001/07/21 00:46:54  ullrich
 * Changed nMaxBcData from 8 to 16 since table changed.
 *
 * Revision 2.2  2001/07/19 00:04:07  ullrich
 * Updated to handle new trigger info.
 *
 * Revision 2.1  2001/04/05 04:00:38  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.0  1999/10/12 18:42:26  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StL0Trigger_hh
#define StL0Trigger_hh

/// @file StL0Trigger.h
/// @brief Level-0 hardware trigger data including DSM outputs, bunch-crossing IDs, and spin bits.

#include "StTrigger.h"

class dst_L0_Trigger_st;
class dst_TrgDet_st;
class StTriggerData;

/// @brief Stores Level-0 (L0) hardware trigger data: DSM outputs, CTB/MWC topologies,
///        bunch-crossing identifiers, and proton spin bits.
class StL0Trigger : public StTrigger {
public:
    StL0Trigger();
    /// @brief Populate from legacy DST L0 trigger table.
    void set(const dst_L0_Trigger_st*);
    /// @brief Populate from legacy DST trigger detector table.
    void set(const dst_TrgDet_st    *);
    /// @brief Populate from decoded StTriggerData object.
    void set(const StTriggerData    *);
    // StL0Trigger(const StL0Trigger&);            use default
    // StL0Trigger& operator=(const StL0Trigger&); use default
    ~StL0Trigger();

    /// @brief Return the number of valid entries in the coarse pixel array.
    unsigned int    coarsePixelArraySize();
    /// @brief Return the coarse pixel value at index @p i (CTB topology map).
    int             coarsePixelArray(unsigned int);
    /// @brief Return the MWC+CTB combined multiplicity.
    int             mwcCtbMultiplicity() const;
    /// @brief Return the MWC+CTB dipole asymmetry.
    int             mwcCtbDipole() const;
    /// @brief Return the MWC+CTB event topology word.
    int             mwcCtbTopology() const;
    /// @brief Return the MWC+CTB transverse moment.
    int             mwcCtbMoment() const;
    /// @brief Return the DSM input word.
    unsigned short  dsmInput() const;
    /// @brief Return the detector busy status byte.
    unsigned char   detectorBusy() const; 
    /// @brief Return the trigger token.
    unsigned short  triggerToken() const;
    /// @brief Return the DSM address word.
    unsigned short  dsmAddress() const;  
    /// @brief Return the additional bits byte.
    unsigned char   addBits() const;
    /// @brief Return the number of valid entries in the last DSM array.
    unsigned int    lastDsmArraySize() const;
    /// @brief Return the last DSM array value at index @p i.
    unsigned short  lastDsmArray(unsigned int);
    /// @brief Return the number of valid entries in the bunch-crossing data array.
    unsigned int    bcDataArraySize() const;
    /// @brief Return the bunch-crossing data array value at index @p i.
    unsigned short  bcDataArray(unsigned int);

    /// @brief Return the 7-bit bunch-crossing ID for beam @p beam (0=yellow, 1=blue).
    unsigned int bunchCrossingId7bit(int) const;
    /// @brief Return the 48-bit bunch-crossing ID.
    unsigned int bunchCrossingId() const;
    
    /// @brief Return the combined spin bits word for beam @p beam.
    int spinBits(int) const;
    /// @brief Return the yellow beam spin-up bit for crossing @p idx.
    int spinBitYellowUp(int) const;
    /// @brief Return the yellow beam spin-down bit for crossing @p idx.
    int spinBitYellowDown(int) const;
    /// @brief Return the blue beam spin-up bit for crossing @p idx.
    int spinBitBlueUp(int) const;
    /// @brief Return the blue beam spin-down bit for crossing @p idx.
    int spinBitBlueDown(int) const;

    /// @brief Set the MWC+CTB multiplicity.
    void setMwcCtbMultiplicity(int);
    /// @brief Set the MWC+CTB dipole asymmetry.
    void setMwcCtbDipole(int);
    /// @brief Set the MWC+CTB topology word.
    void setMwcCtbTopology(int);
    /// @brief Set the MWC+CTB moment.
    void setMwcCtbMoment(int);
    /// @brief Set the coarse pixel array value at index @p i.
    void setCoarsePixelArray(unsigned int, int);
    /// @brief Set the DSM input word.
    void setDsmInput(unsigned short);
    /// @brief Set the detector busy status byte.
    void setDetectorBusy(unsigned char); 
    /// @brief Set the trigger token.
    void setTriggerToken(unsigned short);
    /// @brief Set the DSM address word.
    void setDsmAddress(unsigned short);  
    /// @brief Set the additional bits byte.
    void setAddBits(unsigned char);   
    /// @brief Set last DSM array value at index @p i.
    void setLastDsmArray(unsigned int, unsigned short);
    /// @brief Set bunch-crossing data array value at index @p i.
    void setBcDataArray(unsigned int, unsigned short);
    
protected:
    enum {mMaxPixels = 32, mMaxLastDsm = 8, mMaxBcData = 16};

private:    
    Int_t         mCoarsePixelArray[mMaxPixels]; ///< Coarse CTB pixel map for topology.
    Int_t         mMwcCtbMultiplicity;           ///< MWC+CTB combined particle multiplicity.
    Int_t         mMwcCtbDipole;                 ///< MWC+CTB dipole asymmetry.
    Int_t         mMwcCtbTopology;               ///< MWC+CTB event topology word.
    Int_t         mMwcCtbMoment;                 ///< MWC+CTB transverse moment.
    UShort_t      mDsmInput;                     ///< DSM input word.
    UChar_t       mDetectorBusy;                 ///< Detector busy status byte.
    UShort_t      mTriggerToken;                 ///< Trigger token from the TCU.
    UShort_t      mDsmAddress;                   ///< DSM address word.
    UChar_t       mAddBits;                      ///< Additional bits byte.
    UShort_t      mLastDsmArray[mMaxLastDsm];    ///< Last DSM layer output words.
    UShort_t      mBcDataArray[mMaxBcData];      ///< Bunch-crossing data words (spin bits etc.).

    ClassDef(StL0Trigger,2)
};
#endif
