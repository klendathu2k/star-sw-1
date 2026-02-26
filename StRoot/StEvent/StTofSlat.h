/*!
 * \class StTofSlat 
 * \author Wei-Ming Zhang, Dec 2000
 */
/***************************************************************************
 *
 * $Id: StTofSlat.h,v 2.9 2004/02/05 17:58:51 ullrich Exp $
 *
 * Author: Wei-Ming Zhang, Dec 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTofSlat.h,v $
 * Revision 2.9  2004/02/05 17:58:51  ullrich
 * Changed $LINK to StLink mechanism.
 *
 * Revision 2.8  2003/07/28 21:00:22  ullrich
 * Revised version: new but not inheriting from StHit as before.
 *
 * Revision 2.7  2003/07/11 00:01:04  jeromel
 * Re-adding preceeding revision
 *
 * Revision 2.4  2003/05/21 18:23:18  ullrich
 * Major Revision of ToF classes (F. Geurts)
 *
 * Revision 2.3  2002/02/22 22:56:52  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:43  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  2000/12/21 23:52:23  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTofSlat_hh
#define StTofSlat_hh

/// @file StTofSlat.h
/// @brief Legacy STAR TOF scintillator slat matched to a reconstructed track.

#include "StObject.h"
#include "StThreeVectorF.hh"
#include "StContainers.h"
class StTrack;

/// @brief Legacy STAR TOF scintillator slat matched to a reconstructed track.
class StTofSlat : public StObject {
public:
    /// @brief Default constructor.
    StTofSlat();
    /// @brief Constructs with slat index, ADC/TDC values, associated track, and position.
    StTofSlat(unsigned short, unsigned short, unsigned short, StTrack*,
              float, unsigned short, unsigned short);
    /// @brief Destructor.
    ~StTofSlat();
    
    /// @brief Equality comparison operator.
    int operator==(const StTofSlat&) const;
    /// @brief Inequality comparison operator.
    int operator!=(const StTofSlat&) const;

    /// @brief Returns the slat index.
    unsigned short        slatIndex() const;
    /// @brief Returns the raw ADC value.
    unsigned short        adc() const;
    /// @brief Returns the raw TDC value.
    unsigned short        tdc() const;
    /// @brief Returns a pointer to the associated reconstructed track.
    StTrack*              associatedTrack();
    /// @brief Returns a const pointer to the associated reconstructed track.
    const StTrack*        associatedTrack() const;
    /// @brief Returns the z-coordinate of the hit position [cm].
    float                 zHit() const;
    /// @brief Returns the hit profile bitmask within the slat.
    unsigned short        hitProf() const;
    /// @brief Returns the track-to-slat matching quality flag.
    unsigned short        matchFlag() const;
    /// @brief Returns the 3D hit position [cm].
    const StThreeVectorF& position() const;
    
    /// @brief Sets the slat index.
    void setSlatIndex(unsigned short);
    /// @brief Sets the raw ADC value.
    void setAdc(unsigned short);
    /// @brief Sets the raw TDC value.
    void setTdc(unsigned short);
    /// @brief Sets the pointer to the associated reconstructed track.
    void setAssociatedTrack(StTrack*);
    /// @brief Sets the z-coordinate of the hit position [cm].
    void setZHit(float);
    /// @brief Sets the hit profile bitmask.
    void setHitProf(unsigned short);
    /// @brief Sets the track-to-slat matching quality flag.
    void setMatchFlag(unsigned short);
    /// @brief Sets the 3D hit position [cm].
    void setPosition(const StThreeVectorF&);
    
protected:
    UShort_t  mSlatIndex;  ///< Slat index
    UShort_t  mAdc;        ///< Raw ADC value
    UShort_t  mTdc;        ///< Raw TDC value
    // StTrack  *mAssociatedTrack; //$LINK
#ifdef __CINT__
    StObjLink        mAssociatedTrack;		
#else
    StLink<StTrack>  mAssociatedTrack;		
#endif //__CINT__
    Float_t   mZhit;       ///< Z coordinate of the hit position [cm]
    UShort_t  mHitProf;    ///< Hit profile bitmask within the slat
    UShort_t  mMatchFlag;  ///< Track-to-slat matching quality flag
    StThreeVectorF mPosition;  ///< 3D hit position [cm]

    ClassDef(StTofSlat,4)
};

inline void
StTofSlat::setSlatIndex(unsigned short slatId) {mSlatIndex = slatId;}

inline void
StTofSlat::setAdc(unsigned short rawAdc) {mAdc = rawAdc;}

inline void
StTofSlat::setTdc(unsigned short rawTdc) {mTdc = rawTdc;}

inline void
StTofSlat::setZHit(float zhit) {mZhit = zhit;}

inline void
StTofSlat::setHitProf(unsigned short hitprof) {mHitProf = hitprof;}

inline void
StTofSlat::setMatchFlag(unsigned short matchflag) {mMatchFlag = matchflag;}

inline unsigned short
StTofSlat::slatIndex() const {return mSlatIndex;}

inline unsigned short
StTofSlat::adc()  const {return mAdc;}

inline unsigned short
StTofSlat::tdc()  const {return mTdc;}

inline float
StTofSlat::zHit() const {return mZhit;}

inline unsigned short
StTofSlat::hitProf() const {return mHitProf;}

inline unsigned short
StTofSlat::matchFlag() const {return mMatchFlag;}

#endif
