/*!
 * \class StTpcPixel 
 * \author Thomas Ullrich, Jan 1999
 */
/***************************************************************************
 *
 * $Id: StTpcPixel.h,v 2.8 2015/10/09 17:46:15 ullrich Exp $
 *
 * Author: Thomas Ullrich, Jan 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTpcPixel.h,v $
 * Revision 2.8  2015/10/09 17:46:15  ullrich
 * Changed type of mIdTruth from ushort to int.
 *
 * Revision 2.7  2009/11/23 22:20:51  ullrich
 * Minor cleanup performed, fixed compiler warnings.
 *
 * Revision 2.6  2004/08/06 15:37:43  fisyak
 * Add clster id
 *
 * Revision 2.5  2004/04/26 16:33:35  fisyak
 * Make use of StTpcPixel
 *
 * Revision 2.4  2002/02/22 22:56:52  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:44  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  1999/12/13 20:16:34  ullrich
 * Changed numbering scheme for hw_position unpack methods (STAR conventions).
 *
 * Revision 2.1  1999/10/13 19:44:04  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StTpcPixel_hh
#define StTpcPixel_hh

/// @file StTpcPixel.h
/// @brief Single ADC pixel (pad × time-bin sample) from the TPC readout.

#include "StObject.h"
#include "Stiostream.h"
#include "StEnumerations.h"

/// @brief Single ADC pixel (one pad × one time-bin sample) from the TPC readout.
class StTpcPixel : public StObject {
public:
    /// @brief Construct a TPC pixel with detector, sector, padrow, pad, time-bin, ADC, and MC truth info.
    StTpcPixel(unsigned char Detector = 0, unsigned char Sector = 0, unsigned char Row = 0,
               unsigned char Pad = 0, unsigned short TimeBin = 0,unsigned short Adc=0, 
               int IdTruth=0, short Id=0) :
        mDetector(Detector),  mSector(Sector), mRow(Row), 
        mPad(Pad), mTimeBin(TimeBin), mAdc(Adc), mIdTruth(IdTruth), mId(Id) {}
    /// @brief Destructor.
    virtual ~StTpcPixel() {}
    /// @brief Returns the detector identifier.
    unsigned char   detector() const;
    /// @brief Returns the TPC sector number (1–24).
    unsigned char   sector()   const;
    /// @brief Returns the TPC padrow number.
    unsigned char   padrow()   const;
    /// @brief Returns the pad number within the padrow.
    unsigned char   pad()      const;
    /// @brief Returns the time-bin index within the drift window.
    unsigned short  timebin()  const;
    /// @brief Returns the raw ADC value for this pixel.
    unsigned short  adc()      const;
    /// @brief Returns the Monte Carlo truth track ID associated with this pixel.
    int             idTruth()  const;
    /// @brief Returns the cluster ID this pixel is assigned to.
    short           id()       const;
    /// @brief Prints pixel information to the given output option.
    virtual void Print(Option_t *option="") const;

protected:
    UChar_t   mDetector; ///< Detector identifier.
    UChar_t   mSector;   ///< TPC sector number (1–24).
    UChar_t   mRow;      ///< Padrow number within the sector.
    UChar_t   mPad;      ///< Pad number within the padrow.
    UShort_t  mTimeBin;  ///< Time-bin index within the drift window.
    UShort_t  mAdc;      ///< Raw ADC value for this pad × time-bin sample.
    Int_t     mIdTruth;  ///< Monte Carlo truth track ID.
    Short_t   mId;       ///< Cluster ID this pixel belongs to.
    
    ClassDef(StTpcPixel, 2)  //StTpcPixel structure
};

inline unsigned char   StTpcPixel::detector() const {return mDetector;}
inline unsigned char   StTpcPixel::sector()   const {return mSector;}
inline unsigned char   StTpcPixel::padrow()   const {return mRow;}
inline unsigned char   StTpcPixel::pad()      const {return mPad;}
inline unsigned short  StTpcPixel::timebin()  const {return mTimeBin;}
inline unsigned short  StTpcPixel::adc()      const {return mAdc;}
inline int             StTpcPixel::idTruth()  const {return mIdTruth;}
inline short            StTpcPixel::id()       const {return mId;}

ostream& operator<< (ostream&, const StTpcPixel&);
#endif
