/***************************************************************************
 *
 * $Id: StFmsHit.h,v 2.1 2010/01/08 22:42:31 ullrich Exp $
 *
 * Author: Jingguo Ma, Dec 2009
 ***************************************************************************
 *
 * Description: StFmsHit is data for individual cell 
 *
 ***************************************************************************
 *
 * $Log: StFmsHit.h,v $
 * Revision 2.1  2010/01/08 22:42:31  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StFmsHit_hh
#define StFmsHit_hh

/// @file StFmsHit.h
/// @brief Raw hit (tower signal) in the FMS (Forward Meson Spectrometer).

#include "Stiostream.h"
#include "StObject.h"

/// @brief A single FMS tower hit, storing detector address, ADC/TDC values, and calibrated energy.
class StFmsHit : public StObject {
public:
    /// @brief Default constructor.
    StFmsHit();
    /// @brief Constructor with full hit information.
    StFmsHit(unsigned short det, unsigned short ch,
             unsigned short qtcrate, unsigned short qtslot,
             unsigned short qtch, unsigned short adc,
             unsigned short tdc, float e);
    /// @brief Destructor.
    ~StFmsHit();
    
    /// @brief Return the FMS sub-detector ID.
    unsigned short detectorId() const;
    /// @brief Return the tower channel number within the detector.
    unsigned short channel() const;
    /// @brief Return the QT board crate number.
    unsigned short qtCrate() const;
    /// @brief Return the QT board slot number.
    unsigned short qtSlot() const;
    /// @brief Return the QT board channel number.
    unsigned short qtChannel() const;
    /// @brief Return the raw ADC value.
    unsigned short adc() const;
    /// @brief Return the TDC value.
    unsigned short tdc() const;
    /// @brief Return the calibrated energy deposit (GeV).
    float          energy() const;
    
    /// @brief Set the detector ID.
    void setDetectorId(unsigned short);
    /// @brief Set the tower channel number.
    void setChannel(unsigned short);
    /// @brief Set the QT board crate number.
    void setQtCrate(unsigned short);
    /// @brief Set the QT board slot number.
    void setQtSlot(unsigned short);
    /// @brief Set the QT board channel number.
    void setQtChannel(unsigned short);
    /// @brief Set the raw ADC value.
    void setAdc(unsigned short);
    /// @brief Set the TDC value.
    void setTdc(unsigned short);
    /// @brief Set the calibrated energy deposit.
    void setEnergy(float);
    /// @brief Set all hit fields at once.
    void setFmsHit(unsigned short det, unsigned short ch,
                   unsigned short qtcrate, unsigned short qtslot,
                   unsigned short qtch, unsigned short adc,
                   unsigned short tdc, float e);
    
  /// @brief Print hit properties.
  void print(Option_t *option="") const;

protected:
    /// @brief Encode QT crate, slot, and channel into the packed data word.
    void encodeQTCrtSlotCh(unsigned short qtcrate, unsigned short qtslot, unsigned short qtch);
    
protected:
    UShort_t mDetectorId;  ///< FMS sub-detector ID.
    UShort_t mChannel;     ///< Tower channel number within the detector.
    UShort_t mQTCrtSlotCh; ///< Packed QT crate/slot/channel: 4 bits crate, 4 bits slot, 8 bits channel.
    UShort_t mAdc;         ///< Raw ADC value.
    UShort_t mTdc;         ///< TDC value.
    Float_t  mEnergy;      ///< Calibrated energy deposit (GeV).
    
    ClassDef(StFmsHit,1)
};

ostream& operator<<(ostream&, const StFmsHit&);
#endif
