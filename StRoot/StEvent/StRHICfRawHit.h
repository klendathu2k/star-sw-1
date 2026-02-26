#ifndef StRHICfRawHit_hh
#define StRHICfRawHit_hh

/// @file StRHICfRawHit.h
/// @brief Raw ADC and TDC data from the RHICf detector front-end electronics.

#include <TObject.h>
#include "StEnumerations.h"

/// @brief Raw digitised signals from the RHICf detector front-end electronics.
///
/// Stores per-channel ADC values for the GSO sampling plates and position-sensitive bars
/// (in both normal and delayed readout modes for pedestal subtraction), TDC timing values,
/// and the CAD0/GPI0/GPI1 trigger-related signal words.
class StRHICfRawHit : public TObject 
{
  public:
    /// @brief Default constructor.
    StRHICfRawHit();
    ~StRHICfRawHit();

    /// @brief Clears all raw ADC and TDC data.
    void clear();

    /// @brief Sets the ADC value for a GSO sampling plate in the given gain range.
    void setPlateADC(Int_t tower, Int_t plate, Int_t range, Int_t adc);
    /// @brief Sets the delayed ADC value (pedestal sample) for a GSO sampling plate in the given gain range.
    void setPlateADCDelay(Int_t tower, Int_t plate, Int_t range, Int_t adc);
    /// @brief Sets the ADC value for a GSO position-sensitive bar.
    void setGSOBarADC(Int_t tower, Int_t layer, Int_t xy, Int_t bar, Int_t adc);
    /// @brief Sets the TDC value for the given channel index.
    void setTDC(Int_t idx, UInt_t val);
    /// @brief Sets the CAD0 trigger signal word for the given index.
    void setCAD0(Int_t idx, UInt_t val);
    /// @brief Sets the GPI0 trigger signal word for the given index.
    void setGPI0(Int_t idx, UInt_t val);
    /// @brief Sets the GPI1 trigger signal word for the given index.
    void setGPI1(Int_t idx, UInt_t val);

    /// @brief Returns the ADC value for a GSO sampling plate in the given gain range.
    UShort_t getPlateADC(Int_t tower, Int_t plate, Int_t range);
    /// @brief Returns the delayed ADC value (pedestal sample) for a GSO sampling plate.
    UShort_t getPlateADCDelay(Int_t tower, Int_t plate, Int_t range);
    /// @brief Returns the ADC value for a GSO position-sensitive bar.
    UShort_t getGSOBarADC(Int_t tower, Int_t layer, Int_t xy, Int_t bar);
    /// @brief Returns the TDC value for the given channel index.
    UInt_t getTDC(Int_t idx);
    /// @brief Returns the CAD0 trigger signal word for the given index.
    UInt_t getCAD0(Int_t idx);
    /// @brief Returns the GPI0 trigger signal word for the given index.
    UInt_t getGPI0(Int_t idx);
    /// @brief Returns the GPI1 trigger signal word for the given index.
    UInt_t getGPI1(Int_t idx);

  private:
    UShort_t mPlateADC[kRHICfNtower][kRHICfNplate][kRHICfNrange];      ///< ADC values of GSO sampling plates, indexed by tower, plate, and gain range.
    UShort_t mPlateADCDelay[kRHICfNtower][kRHICfNplate][kRHICfNrange]; ///< Delayed ADC values of GSO plates for pedestal estimation, indexed by tower, plate, and gain range.
    UShort_t mGSOSmallADC[kRHICfNlayer][kRHICfNxy][kRHICfNbarSmall];   ///< ADC values of GSO position-sensitive bars in the small calorimeter tower.
    UShort_t mGSOLargeADC[kRHICfNlayer][kRHICfNxy][kRHICfNbarLarge];   ///< ADC values of GSO position-sensitive bars in the large calorimeter tower.
    UInt_t mTDC[kRHICfNtdc];   ///< TDC timing values for each channel.
    UInt_t mCAD0[kRHICfNcad0]; ///< CAD0 trigger signal words.
    UInt_t mGPI0[kRHICfNgpi0]; ///< GPI0 trigger signal words.
    UInt_t mGPI1[kRHICfNgpi1]; ///< GPI1 trigger signal words.

  ClassDef(StRHICfRawHit,1)
};

#endif
