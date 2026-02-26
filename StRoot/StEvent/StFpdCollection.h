/***************************************************************************
 *
 * $Id: StFpdCollection.h,v 2.5 2002/09/25 14:04:17 akio Exp $
 *
 * Author: Akio Ogawa, Jan 2002
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StFpdCollection.h,v $
 * Revision 2.5  2002/09/25 14:04:17  akio
 * Bug fix in the service functions, no change in data
 *
 * Revision 2.4  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2002/01/17 18:38:11  ullrich
 * Several new methods added. Bug fixed.
 *
 * Revision 2.2  2002/01/09 15:37:55  ullrich
 * AdcSum functions and scaler infos added.
 *
 * Revision 2.1  2002/01/03 20:57:36  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StFpdCollection_hh
#define StFpdCollection_hh

/// @file StFpdCollection.h
/// @brief Legacy FPD (Forward Pion Detector) raw data collection.

#include "StObject.h"

/// @brief Raw data container for the legacy FPD (Forward Pion Detector), storing ADC, TDC, pedestal, and scaler arrays.
class StFpdCollection : public StObject {
public:
    /// @brief Default constructor.
    StFpdCollection();
    /// @brief Destructor.
    virtual ~StFpdCollection();
    // StFpdCollection(const StFpdCollection&);            use default
    // StFpdCollection& operator=(const StFpdCollection&); use default
    
    /// @brief Return the number of ADC channels.
    unsigned int    numberOfADC() const;
    /// @brief Return the number of TDC channels.
    unsigned int    numberOfTDC() const;
    /// @brief Return the number of hardware register values.
    unsigned int    numberOfRegisters() const;
    /// @brief Return the number of pedestal values.
    unsigned int    numberOfPedestal() const;
    /// @brief Return the number of PMTs shared with the EEMC.
    unsigned int    numberOfPMTpEEMC() const;
    /// @brief Return the number of Pb-glass PMTs.
    unsigned int    numberOfPMTPbg() const;
    /// @brief Return the number of SMD x-strip PMTs.
    unsigned int    numberOfPMTSmdX() const;
    /// @brief Return the number of SMD y-strip PMTs.
    unsigned int    numberOfPMTSmdY() const;
    /// @brief Return the number of scaler channels.
    unsigned int    numberOfScalers() const;
    
    /// @brief Return pointer to the ADC array.
    unsigned short*       adc();
    const unsigned short* adc() const;
    /// @brief Return pointer to the TDC array.
    unsigned short*       tdc();
    const unsigned short* tdc() const;
    /// @brief Return a hardware register value.
    unsigned short        registers(unsigned int) const;
    /// @brief Return pointer to the pedestal array.
    unsigned short*       pedestal();
    const unsigned short* pedestal() const;
    /// @brief Return the scaler value at index @p i.
    unsigned int          scaler(unsigned int) const;

    /// @brief Return ADC for north detector tower at index @p i.
    unsigned short  north(unsigned int) const;
    /// @brief Return ADC for south detector tower at index @p i.
    unsigned short  south(unsigned int) const;
    /// @brief Return ADC for top detector tower at index @p i.
    unsigned short  top(unsigned int) const;
    /// @brief Return ADC for bottom detector tower at index @p i.
    unsigned short  bottom(unsigned int) const;
    /// @brief Return ADC for SMD x-strip at index @p i.
    unsigned short  smdx(unsigned int) const;
    /// @brief Return ADC for SMD y-strip at index @p i.
    unsigned short  smdy(unsigned int) const;
    /// @brief Return ADC for pre-shower layer 1 at index @p i.
    unsigned short  pres1(unsigned int) const;
    /// @brief Return ADC for pre-shower layer 2 at index @p i.
    unsigned short  pres2(unsigned int) const;
    /// @brief Return ADC for the south veto counter.
    unsigned short  southVeto() const;

    /// @brief Return summed ADC for all north towers.
    unsigned int sumAdcNorth() const;
    /// @brief Return summed ADC for all south towers.
    unsigned int sumAdcSouth() const;
    /// @brief Return summed ADC for all top towers.
    unsigned int sumAdcTop() const;
    /// @brief Return summed ADC for all bottom towers.
    unsigned int sumAdcBottom() const;
    /// @brief Return summed ADC for pre-shower layer 1.
    unsigned int sumAdcPreShower1() const;
    /// @brief Return summed ADC for pre-shower layer 2.
    unsigned int sumAdcPreShower2() const;
    /// @brief Return summed ADC for all SMD x-strips.
    unsigned int sumAdcSmdX() const;
    /// @brief Return summed ADC for all SMD y-strips.
    unsigned int sumAdcSmdY() const;
    
    /// @brief Set an ADC value.
    void setAdc(unsigned int, unsigned short);
    /// @brief Set a TDC value.
    void setTdc(unsigned int, unsigned short);
    /// @brief Set a hardware register value.
    void setRegister(unsigned int, unsigned short);
    /// @brief Set a pedestal value.
    void setPedestal(unsigned int, unsigned short);
    /// @brief Set a scaler value.
    void setScaler(unsigned int, unsigned int);

    /// @brief Return the event token.
    unsigned int token() const;
    /// @brief Set the event token.
    void setToken(unsigned int);

    /// @brief Dump all data to standard output for debugging.
    void dump();
    
protected:
    enum {mMaxAdc = 256,
	  mMaxTdc = 8,
	  mMaxRegisters = 3,
	  mMaxPedestal = 256,
          mMaxPMTpEEMC = 12,
          mMaxPMTPbg   = 16,
          mMaxPMTSmdX  = 60,
          mMaxPMTSmdY  = 100,
          mMaxScalers  = 128};
    
    UShort_t mAdc[mMaxAdc]; ///< ADC values for all FPD channels.
    UShort_t mTdc[mMaxTdc]; ///< TDC values.
    UShort_t mReg[mMaxRegisters]; ///< Hardware register values.
    UShort_t mPed[mMaxPedestal]; ///< Pedestal values.
    UInt_t   mScl[mMaxScalers]; ///< Scaler values.
    UShort_t mToken; ///< Event token.
  
    ClassDef(StFpdCollection,2)
};

inline unsigned int
StFpdCollection::numberOfADC() const {return mMaxAdc;}          

inline unsigned int
StFpdCollection::numberOfTDC() const {return mMaxTdc;}          

inline unsigned int
StFpdCollection::numberOfRegisters() const {return mMaxRegisters;}

inline unsigned int
StFpdCollection::numberOfPedestal() const {return mMaxPedestal;}

inline unsigned int
StFpdCollection::numberOfPMTpEEMC() const {return mMaxPMTpEEMC;}

inline unsigned int
StFpdCollection::numberOfPMTPbg() const {return mMaxPMTPbg;}

inline unsigned int
StFpdCollection::numberOfPMTSmdX() const {return mMaxPMTSmdX;}

inline unsigned int
StFpdCollection::numberOfPMTSmdY() const {return mMaxPMTSmdY;}

inline unsigned int
StFpdCollection::numberOfScalers() const {return mMaxScalers;}

inline unsigned int
StFpdCollection::token() const {return mToken;}

#endif
