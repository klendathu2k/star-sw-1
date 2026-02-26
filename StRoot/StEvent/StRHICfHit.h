#ifndef StRHICfHit_hh
#define StRHICfHit_hh

/// @file StRHICfHit.h
/// @brief Reconstructed hit data from the RHICf calorimeter towers.

#include <TObject.h>
#include <TArrayI.h>
#include <TArrayF.h>
#include "StEnumerations.h"

/// @brief Reconstructed shower data from the RHICf dual-tower calorimeter.
///
/// Stores energy deposits in GSO scintillator plates and position-sensitive bars,
/// shower depth parameters (L20, L90), the layer of maximum energy deposition, and
/// the results of single- and multi-shower decomposition fits for both calorimeter towers.
class StRHICfHit : public TObject 
{
  public:
    /// @brief Default constructor.
    StRHICfHit();
    ~StRHICfHit();

    /// @brief Clears all reconstructed hit data.
    void clear();

    /// @brief Allocates the optional dynamic data arrays (L20, L90, shower fit results).
    void initDataArray(); 
    /// @brief Releases the optional dynamic data arrays.
    void deleteDataArray();

    /// @brief Returns true if the optional dynamic data arrays have been allocated.
    Bool_t isSaveDataArray(); 

    /// @brief Sets the energy deposited in a GSO sampling plate (GeV).
    void setPlateEnergy(Int_t tower, Int_t plate, Float_t val);
    /// @brief Sets the energy deposited in a GSO position-sensitive bar (GeV).
    void setGSOBarEnergy(Int_t tower, Int_t layer, Int_t xy, Int_t bar, Float_t val);

    /// @brief Sets the shower depth L20 (radiation lengths to 20% energy containment) for a tower.
    void setL20(Int_t tower, Float_t val);
    /// @brief Sets the shower depth L90 (radiation lengths to 90% energy containment) for a tower.
    void setL90(Int_t tower, Float_t val);

    /// @brief Sets the GSO layer index of maximum energy deposition for the given shower order.
    void setGSOMaxLayer(Int_t tower, Int_t order, Int_t val);
    /// @brief Sets the bar bin of maximum shower peak in the given layer and projection.
    void setMaxPeakBin(Int_t tower, Int_t layer, Int_t xy, Int_t val);

    /// @brief Sets the number of single-shower candidates in the given layer and projection.
    void setSingleHitNum(Int_t tower, Int_t layer, Int_t xy, Int_t val);
    /// @brief Sets the reconstructed transverse position of a single shower (mm).
    void setSingleHitPos(Int_t tower, Int_t layer, Int_t xy, Float_t val);
    /// @brief Sets the peak height of the single-shower profile fit.
    void setSinglePeakHeight(Int_t tower, Int_t layer, Int_t xy, Float_t val);
    /// @brief Sets the chi-squared of the single-shower profile fit.
    void setSingleFitChi2(Int_t tower, Int_t layer, Int_t xy, Float_t val);

    /// @brief Sets the number of multi-shower candidates reconstructed in a tower.
    void setMultiHitNum(Int_t tower, Int_t val);
    /// @brief Sets the transverse position of a multi-shower component (mm).
    void setMultiHitPos(Int_t tower, Int_t layer, Int_t xy, Int_t order, Float_t val);
    /// @brief Sets the peak height of a multi-shower profile component.
    void setMultiPeakHeight(Int_t tower, Int_t layer, Int_t xy, Int_t order, Float_t val);
    /// @brief Sets the raw (unfit) peak height of a multi-shower component.
    void setMultiPeakRaw(Int_t tower, Int_t layer, Int_t xy, Int_t order, Float_t val);
    /// @brief Sets the energy sum attributed to a multi-shower component (GeV).
    void setMultiEnergySum(Int_t tower, Int_t layer, Int_t xy, Int_t order, Float_t val);
    /// @brief Sets the chi-squared of the multi-shower profile fit.
    void setMultiFitChi2(Int_t tower, Int_t layer, Int_t xy, Float_t val);

    /// @brief Returns the energy deposited in a GSO sampling plate (GeV).
    Float_t getPlateEnergy(Int_t tower, Int_t plate);
    /// @brief Returns the energy deposited in a GSO position-sensitive bar (GeV).
    Float_t getGSOBarEnergy(Int_t tower, Int_t layer, Int_t xy, Int_t bar);

    /// @brief Returns the shower depth L20 (radiation lengths) for a tower.
    Float_t getL20(Int_t tower);
    /// @brief Returns the shower depth L90 (radiation lengths) for a tower.
    Float_t getL90(Int_t tower);

    /// @brief Returns the GSO layer index of maximum energy deposition for the given shower order.
    Int_t getGSOMaxLayer(Int_t tower, Int_t order);
    /// @brief Returns the bar bin of maximum shower peak in the given layer and projection.
    Int_t getMaxPeakBin(Int_t tower, Int_t layer, Int_t xy);

    /// @brief Returns the number of single-shower candidates in the given layer and projection.
    Int_t getSingleHitNum(Int_t tower, Int_t layer, Int_t xy);
    /// @brief Returns the reconstructed transverse position of a single shower (mm).
    Float_t getSingleHitPos(Int_t tower, Int_t layer, Int_t xy);
    /// @brief Returns the peak height of the single-shower profile fit.
    Float_t getSinglePeakHeight(Int_t tower, Int_t layer, Int_t xy);
    /// @brief Returns the chi-squared of the single-shower profile fit.
    Float_t getSingleFitChi2(Int_t tower, Int_t layer, Int_t xy);

    /// @brief Returns the number of multi-shower candidates in a tower.
    Int_t getMultiHitNum(Int_t tower);
    /// @brief Returns the transverse position of a multi-shower component (mm).
    Float_t getMultiHitPos(Int_t tower, Int_t layer, Int_t xy, Int_t order);
    /// @brief Returns the peak height of a multi-shower profile component.
    Float_t getMultiPeakHeight(Int_t tower, Int_t layer, Int_t xy, Int_t order);
    /// @brief Returns the raw (unfit) peak height of a multi-shower component.
    Float_t getMultiPeakRaw(Int_t tower, Int_t layer, Int_t xy, Int_t order);
    /// @brief Returns the energy sum attributed to a multi-shower component (GeV).
    Float_t getMultiEnergySum(Int_t tower, Int_t layer, Int_t xy, Int_t order);
    /// @brief Returns the chi-squared of the multi-shower profile fit.
    Float_t getMultiFitChi2(Int_t tower, Int_t layer, Int_t xy);

  private:
    Float_t mPlateE[kRHICfNtower][kRHICfNplate];                          ///< GSO sampling plate energies per tower and plate (GeV).
    Float_t mGSOBarSmallE[kRHICfNlayer][kRHICfNxy][kRHICfNbarSmall];     ///< GSO bar energies for the small calorimeter tower (GeV).
    Float_t mGSOBarLargeE[kRHICfNlayer][kRHICfNxy][kRHICfNbarLarge];     ///< GSO bar energies for the large calorimeter tower (GeV).

    TArrayF* mL20 = nullptr;           ///< Shower depth at 20% energy containment (radiation lengths), per tower.
    TArrayF* mL90 = nullptr;           ///< Shower depth at 90% energy containment (radiation lengths), per tower.

    TArrayI* mGSOMaxLayer = nullptr;   ///< Layer index of maximum GSO energy deposition, per tower and shower order.
    TArrayI* mMaxPeakBin = nullptr;    ///< Bar bin of maximum shower peak, per tower, layer, and projection.

    TArrayI* mSingleHitNum = nullptr;       ///< Number of single-shower candidates, per tower, layer, and projection.
    TArrayF* mSingleHitPos = nullptr;       ///< Transverse position of single shower (mm), per tower, layer, and projection.
    TArrayF* mSinglePeakHeight = nullptr;   ///< Peak height of single-shower profile fit, per tower, layer, and projection.
    TArrayF* mSingleChiSquare = nullptr;    ///< Chi-squared of single-shower profile fit, per tower, layer, and projection.

    TArrayI* mMultiHitNum = nullptr;        ///< Number of multi-shower candidates, per tower.
    TArrayF* mMultiHitPos = nullptr;        ///< Transverse position of each multi-shower component (mm).
    TArrayF* mMultiPeakHeight = nullptr;    ///< Peak height of each multi-shower profile component.
    TArrayF* mMultiPeakRaw = nullptr;       ///< Raw (unfit) peak height of each multi-shower component.
    TArrayF* mMultiEnergySum = nullptr;     ///< Energy sum attributed to each multi-shower component (GeV).
    TArrayF* mMultiChiSquare = nullptr;     ///< Chi-squared of multi-shower profile fit, per tower, layer, and projection.

    Bool_t mIsSaveDataArray; ///< True if the optional dynamic data arrays have been allocated.

  ClassDef(StRHICfHit,1)
};

#endif
