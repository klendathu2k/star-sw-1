/***************************************************************************
* $Id: StFstHit.h $
*
* Author: Shenghui Zhang, Oct. 2021
****************************************************************************
* Description:
* Data structure for individual FST hit (a 1D cluster).
***************************************************************************/

#ifndef StFstHit_hh
#define StFstHit_hh

/// @file StFstHit.h
/// @brief Data structure for an individual FST reconstructed hit (1D cluster).

#include "StarClassLibrary/StMemoryPool.hh"
#include "StEvent/StHit.h"
#include "StEvent/StEnumerations.h"
#include "StEvent/StFstConsts.h"

#include <iostream>

/// @brief Represents a single reconstructed hit (1D cluster of strips) in the Forward Silicon Tracker (FST).
///
/// Position is stored as the mean R-strip and phi-strip indices within the sensor,
/// in addition to the global 3D position inherited from StHit.
class StFstHit : public StHit
{
public:
   StFstHit(unsigned char disk = -1, unsigned char wedge = -1, unsigned char sensor = -1, unsigned char apv = -1, float Charge = 0., float ChargeErr = 0.,
      unsigned char maxTB = 0, float meanRStrip = -1, float meanPhiStrip = -1, unsigned char nRawHits = 1, unsigned char nRawHitsR = 0,
      unsigned char nRawHitsPhi = 0);
   StFstHit(const StThreeVectorF &position, const StThreeVectorF &error, unsigned int hwPosition, float charge,
      unsigned char trackRefCount = 0);

   StDetectorId detector() const;
   void    setDetectorId(StDetectorId);

   unsigned char getDisk() const;
   unsigned char getWedge() const;
   unsigned char getSensor() const;
   unsigned char getApv() const;
   unsigned char getMaxTimeBin() const;
   float         getMeanPhiStrip() const;
   float         getMeanRStrip() const;
   float         getChargeErr() const;
   unsigned char getNRawHits() const;
   unsigned char getNRawHitsR() const;
   unsigned char getNRawHitsPhi() const;
   float         localPosition(unsigned int ) const;

   void setDiskWedgeSensor(unsigned char disk, unsigned char wedge, unsigned char sensor);
   void setDisk(unsigned char disk);
   void setWedge(unsigned char wedge);
   void setSensor(unsigned char sensor);
   void setApv(unsigned char apv);
   void setMaxTimeBin(unsigned char tb);
   void setChargeErr(float chargeErr);
   void setMeanPhiStrip(float meanPhiStrip);
   void setMeanRStrip(float meanRStrip);
   void setNRawHits(unsigned char nRawHits);
   void setNRawHitsR(unsigned char nRawHitsR);
   void setNRawHitsPhi(unsigned char nRawHitsPhi);
   void setLocalPosition(float, float, float);
    
   void* operator new(size_t sz, void *p) { return p; }
   void* operator new(size_t)             { return mPool.alloc(); }
   void  operator delete(void* p)         { mPool.free(p); }

protected:
   UChar_t mApv;               ///< APV chip ID to which this hit belongs.
   UChar_t mMaxTimeBin;        ///< Time bin index of the maximum charge sample.
   Float_t mMeanRStrip;        ///< Mean R-strip index of the cluster (charge-weighted centroid).
   Float_t mMeanPhiStrip;      ///< Mean phi-strip index of the cluster (charge-weighted centroid).
   Float_t mChargeErr;         ///< Uncertainty on the reconstructed cluster charge.
   UChar_t mNRawHits;          ///< Total cluster size (number of raw hits).
   UChar_t mNRawHitsR;         ///< Cluster size in the radial (R) strip direction.
   UChar_t mNRawHitsPhi;       ///< Cluster size in the azimuthal (phi) strip direction.
   Float_t mLocalPosition[3];  ///< Local position of the hit within the sensor (x, y, z) in cm.
   StDetectorId mDetectorId;   ///< Detector identifier (kFstId).

   static StMemoryPool mPool;  //!

   ClassDef(StFstHit, 1)
};

std::ostream &operator<<(std::ostream &, const StFstHit &);

inline unsigned char StFstHit::getDisk() const            { return 1 + (mHardwarePosition - 1) / kFstNumSensorsPerWedge / kFstNumWedgePerDisk;};
inline unsigned char StFstHit::getWedge() const           { return 1 + (mHardwarePosition - 1) / kFstNumSensorsPerWedge;};
inline unsigned char StFstHit::getSensor() const          { return (mHardwarePosition - 1) % kFstNumSensorsPerWedge;};
inline unsigned char StFstHit::getApv() const             { return mApv;                };
inline unsigned char StFstHit::getMaxTimeBin() const      { return mMaxTimeBin;         };
inline float StFstHit::getMeanPhiStrip() const            { return mMeanPhiStrip;       };
inline float StFstHit::getMeanRStrip() const              { return mMeanRStrip;         };
inline float StFstHit::getChargeErr()    const            { return mChargeErr;          };
inline unsigned char StFstHit::getNRawHits() const        { return mNRawHits;           };
inline unsigned char StFstHit::getNRawHitsR() const       { return mNRawHitsR;          };
inline unsigned char StFstHit::getNRawHitsPhi() const     { return mNRawHitsPhi;        };

inline void StFstHit::setDiskWedgeSensor(unsigned char disk, unsigned char wedge, unsigned char sensor) { setHardwarePosition( 
  (1+disk)*kFstNumWedgePerDisk * kFstNumSensorsPerWedge*0 + (1 + (wedge - 1)*kFstNumSensorsPerWedge + sensor) ); }
inline void StFstHit::setApv(unsigned char apv)                   { mApv = apv;                   };
inline void StFstHit::setMaxTimeBin(unsigned char tb)             { mMaxTimeBin = tb;             };
inline void StFstHit::setMeanPhiStrip(float meanPhiStrip)         { mMeanPhiStrip = meanPhiStrip; };
inline void StFstHit::setMeanRStrip(float meanRStrip)             { mMeanRStrip = meanRStrip;     };
inline void StFstHit::setChargeErr(float chargeErr)               { mChargeErr = chargeErr;       };
inline void StFstHit::setNRawHits(unsigned char nRawHits)         { mNRawHits = nRawHits;         };
inline void StFstHit::setNRawHitsR(unsigned char nRawHitsR)       { mNRawHitsR = nRawHitsR;       };
inline void StFstHit::setNRawHitsPhi(unsigned char nRawHitsPhi)   { mNRawHitsPhi = nRawHitsPhi;   };

#endif


/***************************************************************************
* StFstHit.h,v 1.0
* Revision 1.0 2021/10/04 Shenghui Zhang
* Initial version
****************************************************************************/
