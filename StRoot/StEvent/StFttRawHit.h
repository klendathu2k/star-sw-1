/***************************************************************************
 *
 * $Id: StFttRawHit.h,v 1.0 2021/11/18 18:52:38 jdb Exp $
 *
 * Author: Philipp Weidenkaff, April 2018
 ***************************************************************************
 *
 * Description: Data class for sTGC raw hit in StEvent
 *
 ***************************************************************************/
#ifndef STFTTRAWHIT_H
#define STFTTRAWHIT_H

/// @file StFttRawHit.h
/// @brief Data structure for a single FTT (Forward sTGC Tracker) raw strip hit.

#include <Stiostream.h>
#include "StObject.h"
#include "StEnumerations.h"


/// @brief Stores the raw digitised signal from one FTT strip channel for a single bunch crossing.
///
/// Contains both the electronics readout coordinates (sector, RDO, FEB, VMM, channel)
/// and the mapped detector coordinates (plane, quadrant, row, strip, orientation).
class StFttRawHit : public StObject {
public:
    /**
    ** @brief Default constructor.
    **/
    StFttRawHit();

    StFttRawHit(    UChar_t mSector, UChar_t mRDO, UChar_t mFEB, 
                    UChar_t mVMM, UChar_t mChannel, UShort_t mADC, 
                    UShort_t mBCID, Short_t mTB, Short_t mBCIDDelta );

    ~StFttRawHit() {}

    void setRaw(    UChar_t mSector, UChar_t mRDO, UChar_t mFEB, 
                    UChar_t mVMM, UChar_t mChannel, UShort_t mADC, 
                    UShort_t mBCID, Short_t mTB, Short_t mBCIDDelta );

    void setMapping( UChar_t mPlane, UChar_t mQuadrant, UChar_t mRow, UChar_t mStrip, UChar_t mOrientation );

    void setTime( Short_t mTime ) { this->mTime = mTime; }
    void setIdTruth( UShort_t id ) { mIdTruth = id; }
    void setQaTruth( UShort_t qa ) { mQaTruth = qa; }

    // consant getters
    UChar_t sector() const;
    UChar_t rdo() const;
    UChar_t feb() const;
    UChar_t vmm() const;
    UChar_t channel() const;
    UShort_t adc() const;
    UShort_t bcid() const;
    Short_t dbcid() const;
    Short_t tb() const;
    Short_t time() const;

    UChar_t plane() const;
    UChar_t quadrant() const;
    UChar_t row() const;
    UChar_t strip() const;
    UChar_t orientation() const;
    UShort_t idTruth() const { return mIdTruth; }
    UShort_t qaTruth() const { return mQaTruth; }

protected:
    UChar_t mSector;      ///< Detector sector number.
    UChar_t mRDO;         ///< RDO board number.
    UChar_t mFEB;         ///< Front-End Board (FEB) number.
    UChar_t mVMM;         ///< VMM ASIC chip number on the FEB.
    UChar_t mChannel;     ///< Channel number on the VMM chip.
    UShort_t mADC;        ///< Raw ADC pulse height.
    UShort_t mBCID;       ///< Bunch-crossing ID at the time of the hit.
    Short_t mTB;          ///< Time bin index relative to the trigger.
    Short_t mBCIDDelta;   ///< Difference between hit BCID and trigger BCID.
    Short_t mTime;        ///< Calibrated time (BCID delta converted to ns).

    // mapped information
    UChar_t mPlane;       ///< Mapped FTT detector plane index.
    UChar_t mQuadrant;    ///< Mapped FTT quadrant index.
    UChar_t mRow;         ///< Mapped strip row within the quadrant.
    UChar_t mStrip;       ///< Mapped strip index within the row.
    UChar_t mOrientation; ///< Strip orientation (horizontal or vertical).

    UShort_t mIdTruth=0;  ///< MC truth track ID for embedding studies.
    UShort_t mQaTruth=0;  ///< MC truth quality (fraction of charge from the truth track, %).

    ClassDef( StFttRawHit, 4 );
};

ostream& operator << ( ostream&, const StFttRawHit& digi ); // Printing operator

inline UChar_t  StFttRawHit::sector()      const { return mSector;      };
inline UChar_t  StFttRawHit::rdo()         const { return mRDO;         };
inline UChar_t  StFttRawHit::feb()         const { return mFEB;         };
inline UChar_t  StFttRawHit::vmm()         const { return mVMM;         };
inline UChar_t  StFttRawHit::channel()     const { return mChannel;     };
inline UShort_t StFttRawHit::adc()         const { return mADC;         };
inline UShort_t StFttRawHit::bcid()        const { return mBCID;        };
inline Short_t  StFttRawHit::dbcid()       const { return mBCIDDelta;   };
inline Short_t  StFttRawHit::tb()          const { return mTB;          };
inline Short_t  StFttRawHit::time()        const { return mTime;        };

inline UChar_t  StFttRawHit::plane()       const { return mPlane;       };
inline UChar_t  StFttRawHit::quadrant()    const { return mQuadrant;    };
inline UChar_t  StFttRawHit::row()         const { return mRow;         };
inline UChar_t  StFttRawHit::strip()       const { return mStrip;       };
inline UChar_t  StFttRawHit::orientation() const { return mOrientation; };


#endif // STETOFDIGI_H
