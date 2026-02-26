#ifndef StFstRawHit_hh
#define StFstRawHit_hh

/// @file StFstRawHit.h
/// @brief Raw hit (single pad/channel readout) for the Forward Silicon Tracker (FST).

#include "StObject.h"
#include "StEvent/StFstConsts.h"


/// @brief Stores raw ADC samples and metadata for one FST readout channel (pad) in a single event.
///
/// The charge array holds pedestal-subtracted (physics mode) or raw (calibration mode) ADC values
/// for each time bin.  Geometry is encoded via channelId and geoId.
///
/// @author Shenghui Zhang, Aug. 2021
class StFstRawHit : public StObject
{
public:
   //constructors
   StFstRawHit();

   template<typename Container>
   StFstRawHit(int channelId, int geoId,
      const Container &charges, const Container &chargeErrs = Container{},
      UChar_t maxTimeBin = 1, UShort_t idTruth = 0);

   StFstRawHit(const StFstRawHit &rawHit);

   //accessors
   /// @brief Returns the hardware channel identifier (0–36863).
   int               getChannelId()  const; ///< 0-36863
   /// @brief Returns the geometry identifier (0–36863).
   int               getGeoId()      const; ///< 0-36863
   /// @brief Returns the seed-hit flag (0 = not a seed hit, 1 = seed hit).
   int               getSeedhitflag() const; ///< 0 or 1
   /// @brief Returns the FST disk number (1–3).
   unsigned char     getDisk()       const; ///< 1-3
   /// @brief Returns the wedge number within the disk (1–36).
   unsigned char     getWedge()      const; ///< 1-36
   /// @brief Returns the phi-strip index within the wedge (0–127).
   unsigned char     getPhiStrip()   const; ///< 0-127
   /// @brief Returns the r-strip index within the wedge (0–7).
   unsigned char     getRStrip()     const; ///< 0-7
   /// @brief Returns the charge (ADC value) for the given time bin.
   float             getCharge(int tb = 0)    const;
   /// @brief Returns the charge error for the given time bin.
   float             getChargeErr(int tb = 0) const;
   /// @brief Returns the time bin index with the maximum ADC value.
   unsigned char     getMaxTimeBin() const;
   /// @brief Returns the RDO board number (1–6).
   unsigned char     getRdo()        const; ///< 1-6
   /// @brief Returns the ARM index (0–2).
   unsigned char     getArm()        const; ///< 0-2
   /// @brief Returns the APV chip index (0–15).
   unsigned char     getApv()        const; ///< 0-15
   /// @brief Returns the sensor index within the APV (0–2).
   unsigned char     getSensor()     const; ///< 0-2
   /// @brief Returns the channel index within the APV (0–127).
   unsigned char     getChannel()    const; ///< 0-127
   /// @brief Returns the default time bin index used when none is specified.
   static unsigned char  getDefaultTimeBin();
   /// @brief Returns the MC-truth track ID; 0 denotes background (embedding).
   unsigned short    getIdTruth()    const; ///< for embedding, 0 as background

   //modifiers
   /// @brief Sets the hardware channel identifier.
   void setChannelId(int rChannelId);
   /// @brief Sets the geometry identifier.
   void setGeoId(int rChannelId);
   /// @brief Sets the seed-hit flag.
   void setSeedhitflag(int rSeedhitflag);
   /// @brief Sets the charge for a single time bin (or all bins when tb == -1).
   void setCharge(float charge, int tb = -1);

   /// Overwrites this channel's charges in all time bins by values in the
   /// provided container
   template<typename Container>
   void setCharges(const Container& charges) {
      std::copy( std::begin(charges), std::end(charges), mCharge);
   }

   /// @brief Sets the charge error for a single time bin (or all bins when tb == -1).
   void setChargeErr(float chargeErr, int tb = -1);
   /// @brief Sets the time bin index with the maximum ADC value.
   void        setMaxTimeBin(int tb) ;
   /// @brief Sets the default time bin index used when none is specified.
   static void setDefaultTimeBin( int tb );
   /// @brief Sets the MC-truth track ID (0 denotes background in embedding).
   void        setIdTruth(unsigned short idTruth);

   using StObject::Print;
   void Print(int nTimeBins) const;

private:

   Int_t       mChannelId;                 ///< channel Id, numbering from 0 to 36863
   Int_t       mGeoId;                     ///< geometry Id, numbering from 0 to 36863
   Int_t       mSeedhitflag;               ///< seed hit flag, 0 not a seed hit & 1 mean a seed hit
   Float_t     mCharge[kFstNumTimeBins];   ///< pedestal non-subtracted ADC value saved in calibration mode;
                                           ///< pedestal and CMN subtracted in physics mode
   Float_t     mChargeErr[kFstNumTimeBins];///< charge error in all time bins
   UChar_t     mMaxTimeBin;                ///< the max ADC time bin index of the raw hit
   UShort_t    mIdTruth;                   ///< for embedding, 0 as background

   static UChar_t mDefaultTimeBin; ///< Default time bin index used when none is specified.

   ClassDef(StFstRawHit, 1)
};


/*! Functor for sorting raw hits in ascending order by geometry id mGeoId, i.e. the pad Id order. */
struct rawHitPtrLessThan {
   bool operator() (const StFstRawHit *rawHit1, const StFstRawHit *rawHit2) const;
};

#endif
