/***************************************************************************
*
* $Id: StIstHit.h,v 2.2 2016/02/25 17:10:20 ullrich Exp $
*
* Author: Yaping Wang, March 2013
****************************************************************************
* Description:
* Data structure for individual IST hit (a 1D cluster).
***************************************************************************/

#ifndef StIstHit_hh
#define StIstHit_hh
/// @file StIstHit.h
/// @brief Intermediate Silicon Tracker (IST) hit class for the STAR HFT.

#include "StarClassLibrary/StMemoryPool.hh"
#include "StEvent/StHit.h"
#include "StEvent/StEnumerations.h"

using namespace StIstConsts;


/// @brief Reconstructed hit in the STAR Intermediate Silicon Tracker (IST), a 1D strip cluster.
class StIstHit : public StHit
{
public:
   /// @brief Constructor with geometry and cluster properties.
   StIstHit(unsigned char ladder = -1, unsigned char sensor = -1, float Charge = 0., float ChargeErr = 0.,
      unsigned char maxTB = 0, unsigned char nRawHits = 1, unsigned char nRawHitsZ = 0,
      unsigned char nRawHitsRPhi = 0);
   /// @brief Constructor with global position, hardware address, and charge.
   StIstHit(const StThreeVectorF &position, const StThreeVectorF &error, unsigned int hwPosition, float charge,
      unsigned char trackRefCount = 0);

   /// @brief Returns the detector identifier (kIstId).
   StDetectorId detector() const;
   /// @brief Sets the detector identifier.
   void    setDetectorId(StDetectorId);

   /// @brief Returns the ladder number (derived from hardware position).
   unsigned char getLadder() const;
   /// @brief Returns the sensor number within the ladder (derived from hardware position).
   unsigned char getSensor() const;
   /// @brief Returns the time bin with maximum charge deposition.
   unsigned char getMaxTimeBin() const;
   /// @brief Returns the charge measurement uncertainty.
   float         getChargeErr() const;
   /// @brief Returns the total cluster size (number of raw hits).
   unsigned char getNRawHits() const;
   /// @brief Returns the cluster size along the z (beam) direction.
   unsigned char getNRawHitsZ() const;
   /// @brief Returns the cluster size along the r-phi direction.
   unsigned char getNRawHitsRPhi() const;
   /// @brief Returns the local hit coordinate along the given axis.
   float         localPosition(unsigned int ) const;

   /// @brief Sets the time bin index with maximum charge.
   void setMaxTimeBin(unsigned char tb);
   /// @brief Sets the charge uncertainty.
   void setChargeErr(float chargeErr);
   /// @brief Sets the total cluster size.
   void setNRawHits(unsigned char nRawHits);
   /// @brief Sets the cluster size in z.
   void setNRawHitsZ(unsigned char nRawHitsZ);
   /// @brief Sets the cluster size in r-phi.
   void setNRawHitsRPhi(unsigned char nRawHitsRPhi);
   /// @brief Sets the local hit position within the sensor.
   void setLocalPosition(float, float, float);
    
   void* operator new(size_t sz, void *p) { return p; }
   void* operator new(size_t)             { return mPool.alloc(); }
   void  operator delete(void* p)         { mPool.free(p); }

protected:
   UChar_t mMaxTimeBin;        ///< max charge time bin
   Float_t mChargeErr;         ///< charge uncertainty
   UChar_t mNRawHits;          ///< nRawHits: cluster size
   UChar_t mNRawHitsZ;         ///< cluster size in Z direction
   UChar_t mNRawHitsRPhi;      ///< cluster size in r-phi direction
   Float_t mLocalPosition[3];  ///< local position of hit inside the sensor
   StDetectorId mDetectorId; ///< Detector identifier.

   static StMemoryPool mPool;  //!

   ClassDef(StIstHit, 1)
};

ostream &operator<<(ostream &, const StIstHit &);

inline unsigned char StIstHit::getLadder() const          { return 1 + (mHardwarePosition - 1) / kIstNumSensorsPerLadder;};
inline unsigned char StIstHit::getSensor() const          { return 1 + (mHardwarePosition - 1) % kIstNumSensorsPerLadder;};
inline unsigned char StIstHit::getMaxTimeBin() const      { return mMaxTimeBin;         };
inline float StIstHit::getChargeErr()    const            { return mChargeErr;          };
inline unsigned char StIstHit::getNRawHits() const        { return mNRawHits;           };
inline unsigned char StIstHit::getNRawHitsZ() const       { return mNRawHitsZ;          };
inline unsigned char StIstHit::getNRawHitsRPhi() const    { return mNRawHitsRPhi;       };

inline void StIstHit::setMaxTimeBin(unsigned char tb)             { mMaxTimeBin = tb;             };
inline void StIstHit::setChargeErr(float chargeErr)               { mChargeErr = chargeErr;       };
inline void StIstHit::setNRawHits(unsigned char nRawHits)         { mNRawHits = nRawHits;         };
inline void StIstHit::setNRawHitsZ(unsigned char nRawHitsZ)       { mNRawHitsZ = nRawHitsZ;       };
inline void StIstHit::setNRawHitsRPhi(unsigned char nRawHitsRPhi) { mNRawHitsRPhi = nRawHitsRPhi; };

#endif


/***************************************************************************
*
* $Log: StIstHit.h,v $
* Revision 2.2  2016/02/25 17:10:20  ullrich
* Implemented detector() which is now a pure abstract method in StHit.
*
* Revision 2.1  2014/04/10 16:16:24  jeromel
* Add hit def for Ist (Thmas OK-ed)
*
* Revision 1.10  2014/03/13 22:17:09  smirnovd
* Minor whitespace alignment fixes
*
* Revision 1.9  2014/03/13 22:10:30  smirnovd
* Expand tabs and trim trailing whitespace
*
* Revision 1.8  2014/03/13 22:10:21  smirnovd
* Fixed constructor's initialization list
*
* Revision 1.7  2014/03/13 22:10:12  smirnovd
* Move some constants from StIstUtil/StIstConsts.h to StEvent/StEnumerations.h to avoid external dependance of StEvent on StIstUtil
*
* Revision 1.6  2014/02/26 21:18:08  smirnovd
* Style corrected with astyle -s3 -p -H -A3 -k3 -O -o -y -Y -f
*
* Revision 1.5  2014/02/26 01:35:36  ypwang
* get rid of meanColumn/meanRow/Apv transformations and local position uncertainties to avoid external constants access
*
* Revision 1.4  2014/02/25 17:04:50  ypwang
* get rid of mClusteringType and its accessory/modifier functions
*
*
****************************************************************************
* StIstHit.h,v 1.0
* Revision 1.0 2013/11/04 15:25:30 Yaping
* Initial version
****************************************************************************/
