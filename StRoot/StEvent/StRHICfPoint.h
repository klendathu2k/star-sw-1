#ifndef StRHICfPoint_hh
#define StRHICfPoint_hh

/// @file StRHICfPoint.h
/// @brief Reconstructed particle shower point in the RHICf calorimeter.

#include <TObject.h>
#include "StEnumerations.h"

/// @brief Reconstructed shower point (particle impact position and energy) in the RHICf calorimeter.
///
/// Stores the transverse (x, y) shower position, particle identification code, and
/// deposited energy for a single reconstructed shower in one RHICf calorimeter tower.
class StRHICfPoint : public TObject 
{
  public:
    /// @brief Default constructor.
    StRHICfPoint();
    ~StRHICfPoint();

    /// @brief Clears all shower point data.
    void clear();

    /// @brief Sets the calorimeter tower index (0 = small tower, 1 = large tower).
    void setTowerIdx(Int_t val);
    /// @brief Sets the particle identification code for this shower.
    void setPID(Int_t pid);
    /// @brief Sets the reconstructed transverse shower position (mm).
    void setPointPos(Float_t x, Float_t y);
    /// @brief Sets the reconstructed shower energy for the two particle-type hypotheses (GeV).
    void setPointEnergy(Float_t pid1, Float_t pid2);
    /// @brief Sets the total and partial tower energy sums associated with this shower (GeV).
    void setTowerSumEnergy(Float_t all, Float_t part);

    /// @brief Returns the calorimeter tower index (0 = small, 1 = large).
    Int_t getTowerIdx();
    /// @brief Returns the particle identification code.
    Int_t getPID();
    /// @brief Returns the reconstructed shower position in the given transverse projection (mm).
    Float_t getPointPos(Int_t xy);
    /// @brief Returns the reconstructed shower energy for the given particle-type hypothesis (GeV).
    Float_t getPointEnergy(Int_t particle);
    /// @brief Returns the tower energy sum for the given order (0 = total, 1 = partial) (GeV).
    Float_t getTowerSumEnergy(Int_t order);

  private:
    Int_t mTowerIdx;    ///< Calorimeter tower index (0 = small tower, 1 = large tower).
    Int_t mParticleID;  ///< Particle identification code for this shower.

    Float_t mPointPos[kRHICfNxy];           ///< Reconstructed transverse shower position in x and y (mm).
    Float_t mPointEnergy[kRHICfNorder];     ///< Reconstructed shower energy for each particle-type hypothesis (GeV).
    Float_t mTowerSumEnergy[kRHICfNorder];  ///< Tower energy sums associated with this shower (GeV).

  ClassDef(StRHICfPoint,1)
};

#endif
