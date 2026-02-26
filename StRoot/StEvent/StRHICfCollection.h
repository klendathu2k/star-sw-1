#ifndef StRHICfCollection_hh
#define StRHICfCollection_hh

/// @file StRHICfCollection.h
/// @brief Top-level container for RHICf detector data in a single event.

#include <vector>

#include "Stiostream.h"
#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"

#include "StRHICfRawHit.h"
#include "StRHICfHit.h"
#include "StRHICfPoint.h"

/// @brief Top-level event data container for the RHICf forward calorimeter.
///
/// Holds the raw-hit, reconstructed-hit, and shower-point collections together with
/// run-level metadata. The RHICf run and event numbers originate from the detector's
/// own DAQ system and are independent of the STAR run and event numbers.
class StRHICfCollection : public StObject 
{
  public:
    /// @brief Default constructor.
    StRHICfCollection();
    ~StRHICfCollection();

    /// @brief Clears all data containers and resets metadata.
    void clear();

    /// @brief Returns a pointer to the raw-hit data collection.
    StRHICfRawHit* rawHitCollection();
    /// @brief Returns a pointer to the reconstructed-hit data collection.
    StRHICfHit* hitCollection();

    /// @brief Adds a single reconstructed shower point to the collection.
    void addPoint(StRHICfPoint* pointColl); 
    /// @brief Replaces the shower-point collection with the supplied vector.
    void addPointCollection(std::vector<StRHICfPoint*> coll);
    /// @brief Returns a reference to the vector of reconstructed shower points.
    std::vector<StRHICfPoint*>& pointCollection();
    /// @brief Returns a const reference to the vector of reconstructed shower points.
    const std::vector<StRHICfPoint*>& pointCollection() const;

    /// @brief Marks all data arrays as saved.
    void isAllSave();
    /// @brief Sets the RHICf-internal run number.
    void setRHICfRunNumber(UInt_t run);
    /// @brief Sets the RHICf-internal event number.
    void setRHICfEventNumber(UInt_t event);
    /// @brief Sets the bunch crossing number.
    void setBunchNumber(UInt_t bunch);
    /// @brief Sets the run type flag.
    void setRunType(UInt_t type);
    /// @brief Sets the RHICf trigger number.
    void setTriggerNumber(UInt_t trigger);
    /// @brief Sets the run timestamp for the given time-slot index.
    void setRunTime(Int_t idx, UInt_t time);
    /// @brief Sets the trigger mask (TRGM) value.
    void setRunTRGM(UInt_t trgm);

    /// @brief Returns the number of reconstructed shower points in this event.
    UInt_t numberOfPoints() const;
    /// @brief Returns the RHICf-internal run number.
    UInt_t getRHICfRunNumber() const;
    /// @brief Returns the RHICf-internal event number.
    UInt_t getRHICfEventNumber() const;
    /// @brief Returns the bunch crossing number.
    UInt_t getBunchNumber();
    /// @brief Returns the run type flag.
    UInt_t getRunType();
    /// @brief Returns the RHICf trigger number.
    UInt_t getTriggerNumber();
    /// @brief Returns the run timestamp for the given time-slot index.
    UInt_t getRunTime(Int_t idx);
    /// @brief Returns the trigger mask (TRGM) value.
    UInt_t getRunTRGM();

  private:
    StRHICfRawHit* mRHICfRawHitColl;  ///< Raw ADC/TDC data collection.
    StRHICfHit* mRHICfHitColl;        ///< Reconstructed hit data collection.
    std::vector<StRHICfPoint*> mRHICfPointColl; ///< Collection of reconstructed shower points.

    /// @note The RHICf run and event numbers originate from the detector's own DAQ system
    ///       and are independent of the STAR run and event numbers.
    UInt_t mRHICfRunNumber;   ///< RHICf-internal run number.
    UInt_t mRHICfEventNumber; ///< RHICf-internal event number.
    UInt_t mBunchNumber;      ///< Bunch crossing number.
    UInt_t mRunType;          ///< Run type flag.
    UInt_t mRHICfTrigger;     ///< RHICf trigger number.
    UInt_t mRunTime[kRHICfNorder]; ///< Run timestamps for each time-slot index.
    UInt_t mRunTRGM;          ///< Trigger mask (TRGM) value.

  ClassDef(StRHICfCollection,2)
};

#endif
