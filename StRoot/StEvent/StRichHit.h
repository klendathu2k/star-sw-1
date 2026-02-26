/*!
 * \class StRichHit 
 * \author Brian Lasiuk, May 2000
 */
/***************************************************************************
 *
 * $Id: StRichHit.h,v 2.8 2016/02/25 17:10:20 ullrich Exp $
 *
 * Author: Brian Lasiuk, May 2000
 ***************************************************************************
 *
 * Description: Definition of the persistent Hit object
 *
 ***************************************************************************
 *
 * $Log: StRichHit.h,v $
 * Revision 2.8  2016/02/25 17:10:20  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.7  2004/07/15 16:36:25  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.6  2003/09/02 17:58:05  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.5  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.4  2001/04/05 04:00:40  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.3  2001/03/24 03:34:54  perev
 * clone() -> clone() const
 *
 * Revision 2.2  2000/11/01 16:44:22  lasiuk
 * SCHEMA EVOL: cluster number is now signed and the number of pads
 * that were used in creating a hit is stored.  Bit manipulation
 * functions were changed to use StRichHitFlags from StEnumerations.
 *
 * Revision 2.1  2000/05/22 21:44:50  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRichHit_hh
#define StRichHit_hh

/// @file StRichHit.h
/// @brief Persistent reconstructed hit in the RICH detector, derived from a pad cluster.

#include <Stiostream.h>
#include "StHit.h"

/// @brief Reconstructed hit in the RICH detector, representing a Cherenkov photon or MIP impact point.
class StRichHit : public StHit {
public:
    /// @brief Default constructor.
    StRichHit();
    /// @brief Constructor with global position and position error.
    StRichHit(const StThreeVectorF& xg, const StThreeVectorF& dx);
    /// @brief Constructor with position, error, hardware position, charge, max ADC, and track counter.
    StRichHit(const StThreeVectorF& xg, const StThreeVectorF& dx,
              unsigned int hp, float q, float maxAdc, unsigned char tc);

    virtual ~StRichHit();
    //StRichHit(const StRichHit&){ /* nopt */ }
    //StRichHit& operator=(const StRichHit&){/* nopt */}

    /// @brief Returns the hit position in local (pad-plane) coordinates.
    const    StThreeVectorF& local()    const;
    /// @brief Returns the hit position in internal (detector) coordinates.
    const    StThreeVectorF& internal() const;
    /// @brief Returns the maximum ADC amplitude among pads in the associated cluster.
    float    maxAmplitude()             const;
    /// @brief Returns the index of the parent cluster.
    int            clusterNumber()            const;
    /// @brief Returns the track identifier associated with this hit.
    unsigned int   track()                    const;
    /// @brief Returns the number of pads contributing to this hit.
    unsigned short numberOfPads()             const;

    /// @brief Returns the hit-flag bit field (also reserved for future use).
    unsigned int  reservedLong()             const;
    /// @brief Returns the reserved float word.
    float         reservedFloat()            const;

    /// @brief Returns a mutable reference to the local-coordinate position.
    StThreeVectorF&  local();
    /// @brief Returns a mutable reference to the internal-coordinate position.
    StThreeVectorF&  internal();

    /// @brief Sets the maximum ADC amplitude of the cluster.
    void setMaxAmplitude(float);
    /// @brief Sets the parent cluster index.
    void setClusterNumber(int);
//  void setClusterNumber(unsigned int);
    /// @brief Sets the number of pads contributing to this hit.
    void setNumberOfPads(unsigned short);
    /// @brief Sets the track identifier.
    void setTrack(unsigned int);

    /// @brief Sets the hit-flag bit field.
    void setReservedLong(unsigned int);
    /// @brief Sets the reserved float word.
    void setReservedFloat(float);

    /// @brief Returns true if the specified hit flag bit is set.
    bool         isSet(StRichHitFlag f) const;
    /// @brief Sets the specified hit flag bit.
    void         setBit(StRichHitFlag f);
    /// @brief Clears the specified hit flag bit.
    void         unSetBit(StRichHitFlag f);
    /// @brief Returns the full hit-flag bit field.
    unsigned int flags() const;
    /// @brief Returns the detector identifier (kRichId).
    StDetectorId detector() const;

protected:
    StThreeVectorF mLocal;          ///< Hit position in local pad-plane coordinates
    StThreeVectorF mLError;         ///< Position error in local coordinates
    StThreeVectorF mInternal;       ///< Hit position in internal detector coordinates
    StThreeVectorF mSigma;          ///< Position uncertainty in internal coordinates

    Float_t        mMaxAmplitude;   ///< Maximum ADC amplitude of the associated cluster
    Int_t          mClusterNumber;  ///< Index of the parent cluster
//  UInt_t         mClusterNumber;
    UInt_t         mTrack;          ///< Track identifier associated with this hit

    UInt_t         mReservedLong;   ///< Hit-flag bit field (also reserved for future use)
    Float_t        mReservedFloat;  ///< Reserved float (future use)

    UInt_t         mNumberOfPads;   ///< Number of pads contributing to this hit

    ClassDef(StRichHit,1)
};

inline StDetectorId StRichHit::detector() const {return kRichId;}

inline const StThreeVectorF& StRichHit::local() const {return mLocal;}
inline StThreeVectorF& StRichHit::local() {return mLocal;}
inline const StThreeVectorF& StRichHit::internal() const {return mInternal;}
inline StThreeVectorF& StRichHit::internal() {return mInternal;}
inline void StRichHit::setMaxAmplitude(float m) {mMaxAmplitude = m;}
inline float StRichHit::maxAmplitude() const {return mMaxAmplitude;}
inline void StRichHit::setClusterNumber(int no) {mClusterNumber = no;}
inline int StRichHit::clusterNumber() const {return mClusterNumber;}
//   inline void StRichHit::setClusterNumber(unsigned int no) {mClusterNumber = no;}
//   inline unsigned int StRichHit::clusterNumber() const {return mClusterNumber;}
inline void StRichHit::setTrack(unsigned int tck) { mTrack = tck;}
inline unsigned int StRichHit::track() const {return mTrack;}
inline unsigned short StRichHit::numberOfPads() const { return mNumberOfPads;}
inline void StRichHit::setNumberOfPads(unsigned short n) { mNumberOfPads = n;}

inline unsigned int StRichHit::reservedLong() const {return mReservedLong; }
inline float StRichHit::reservedFloat() const {return mReservedFloat; }
inline void StRichHit::setReservedLong(unsigned int el) {mReservedLong = el;}
inline void StRichHit::setReservedFloat(float fl) {mReservedFloat = fl;}

inline bool StRichHit::isSet(StRichHitFlag f) const { return (mReservedLong & f); }
inline void StRichHit::setBit(StRichHitFlag f) { mReservedLong |= f; }
inline void StRichHit::unSetBit(StRichHitFlag f) { mReservedLong &= ~(f);}
inline unsigned int StRichHit::flags() const{ return (mReservedLong); }

//non-members
ostream& operator<<(ostream& os, const StRichHit& hit);

#endif
