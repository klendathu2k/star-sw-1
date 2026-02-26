/*!
 * \class StBTofPidTraits 
 * \author Xin Dong, Nov 2008
 */
/***************************************************************************
 *
 * $Id: StBTofPidTraits.h,v 2.2 2009/12/08 23:24:46 fine Exp $
 *
 * Author: Xin Dong, Nov 2008
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StBTofPidTraits.h,v $
 * Revision 2.2  2009/12/08 23:24:46  fine
 * Fix issue  #1748 http://www.star.bnl.gov/rt2/Ticket/Display.html?id=1748
 *
 * Revision 2.1  2008/12/22 20:31:00  ullrich
 * Initial Revision.
 *
 *
 ***************************************************************************/
#ifndef StBTofPidTraits_hh
#define StBTofPidTraits_hh

/// @file StBTofPidTraits.h
/// @brief Particle identification traits for a track matched to the STAR Barrel Time of Flight (BTOF) detector.

#include "StTrackPidTraits.h"
#include "StBTofHit.h"
#include "StThreeVectorF.hh"

/// @brief Particle identification traits for a track matched to the STAR Barrel TOF (BTOF) detector.
class StBTofPidTraits : public StTrackPidTraits {
public:
    /// @brief Default constructor.
    StBTofPidTraits();
    /// @brief Destructor.
    ~StBTofPidTraits();
    
    /// @brief Returns a pointer to the associated BTOF hit.
    StBTofHit*       tofHit();
    /// @brief Returns a const pointer to the associated BTOF hit.
    const StBTofHit* tofHit() const;

    /// Matching information
    /// @brief Returns the track-to-hit matching quality flag.
    unsigned char    matchFlag() const;
    /// @brief Returns the local Y coordinate of the extrapolated track [cm].
    float            yLocal() const;
    /// @brief Returns the local Z coordinate of the extrapolated track [cm].
    float            zLocal() const;
    /// @brief Returns the local incident angle of the track [rad].
    float            thetaLocal() const;
    
    /// @brief Returns the extrapolated track position at the BTOF hit.
    StThreeVectorF&         position();
    /// @brief Returns the const extrapolated track position at the BTOF hit.
    const StThreeVectorF&   position() const;

    /// timing for PID
    /// @brief Returns the time-over-threshold of the BTOF hit [ns].
    float   tot() const;
    /// @brief Returns the measured time of flight [ns].
    float   timeOfFlight() const;
    /// @brief Returns the track path length from the interaction vertex to the hit [cm].
    float   pathLength() const;
    /// @brief Returns the particle velocity β = v/c.
    float   beta() const;
    
    /// PID functions
    /// @brief Returns the number of sigma from the electron hypothesis.
    float   sigmaElectron() const;
    /// @brief Returns the number of sigma from the pion hypothesis.
    float   sigmaPion() const;
    /// @brief Returns the number of sigma from the kaon hypothesis.
    float   sigmaKaon() const;
    /// @brief Returns the number of sigma from the proton hypothesis.
    float   sigmaProton() const;
    
    /// @brief Returns the probability of the electron hypothesis.
    float   probElectron() const;
    /// @brief Returns the probability of the pion hypothesis.
    float   probPion() const;
    /// @brief Returns the probability of the kaon hypothesis.
    float   probKaon() const;
    /// @brief Returns the probability of the proton hypothesis.
    float   probProton() const;

    /// @brief Sets the pointer to the associated BTOF hit.
    void    setTofHit(StBTofHit*);
            
    /// @brief Sets the track-to-hit matching quality flag.
    void    setMatchFlag(unsigned char);
    /// @brief Sets the local Y coordinate of the extrapolated track [cm].
    void    setYLocal(float);
    /// @brief Sets the local Z coordinate of the extrapolated track [cm].
    void    setZLocal(float);
    /// @brief Sets the local incident angle of the track [rad].
    void    setThetaLocal(float);
    /// @brief Sets the extrapolated track position at the BTOF hit.
    void    setPosition( const StThreeVectorF&);                            

    /// @brief Sets the measured time of flight [ns].
    void    setTimeOfFlight(float);
    /// @brief Sets the track path length [cm].
    void    setPathLength(float);
    /// @brief Sets the particle velocity β = v/c.
    void    setBeta(float);

    /// @brief Sets the number of sigma from the electron hypothesis.
    void    setSigmaElectron(float);
    /// @brief Sets the number of sigma from the pion hypothesis.
    void    setSigmaPion(float);
    /// @brief Sets the number of sigma from the kaon hypothesis.
    void    setSigmaKaon(float);
    /// @brief Sets the number of sigma from the proton hypothesis.
    void    setSigmaProton(float);

    /// @brief Sets the probability of the electron hypothesis.
    void    setProbElectron(float);
    /// @brief Sets the probability of the pion hypothesis.
    void    setProbPion(float);
    /// @brief Sets the probability of the kaon hypothesis.
    void    setProbKaon(float);
    /// @brief Sets the probability of the proton hypothesis.
    void    setProbProton(float);

private:
    //    StBTofHit *mBTofHit;   //$LINK
#ifdef __CINT__
    StObjLink        mBTofHit;
#else
    StLink<StBTofHit>  mBTofHit;
#endif //__CINT__

    UChar_t   mMatchFlag;    ///< Track-to-hit matching quality flag
    Float_t   mYLocal;       ///< Local Y coordinate of extrapolated track [cm]
    Float_t   mZLocal;       ///< Local Z coordinate of extrapolated track [cm]
    Float_t   mThetaLocal;   ///< Local incident angle of track [rad]
    StThreeVectorF  mPosition;  ///< Extrapolated track position at the BTOF hit [cm]

    Float_t   mTimeOfFlight;   ///< Measured time of flight [ns]
    Float_t   mPathLength;     ///< Track path length from vertex to hit [cm]
    Float_t   mBeta;           ///< Particle velocity β = v/c

    Float_t   mSigmaElectron;  ///< Number of sigma from electron hypothesis
    Float_t   mSigmaPion;      ///< Number of sigma from pion hypothesis
    Float_t   mSigmaKaon;      ///< Number of sigma from kaon hypothesis
    Float_t   mSigmaProton;    ///< Number of sigma from proton hypothesis
    
    Float_t   mProbElectron;   ///< Probability of electron hypothesis
    Float_t   mProbPion;       ///< Probability of pion hypothesis
    Float_t   mProbKaon;       ///< Probability of kaon hypothesis
    Float_t   mProbProton;     ///< Probability of proton hypothesis

    ClassDef(StBTofPidTraits,1)
};

inline unsigned char StBTofPidTraits::matchFlag() const { return mMatchFlag; }
inline float StBTofPidTraits::yLocal() const { return mYLocal; }
inline float StBTofPidTraits::zLocal() const { return mZLocal; }
inline float StBTofPidTraits::thetaLocal() const { return mThetaLocal; }
inline float StBTofPidTraits::timeOfFlight() const { return mTimeOfFlight; }
inline float StBTofPidTraits::pathLength() const { return mPathLength; }
inline float StBTofPidTraits::beta() const { return mBeta; }
inline float StBTofPidTraits::sigmaElectron() const { return mSigmaElectron; }
inline float StBTofPidTraits::sigmaPion() const { return mSigmaPion; }
inline float StBTofPidTraits::sigmaKaon() const { return mSigmaKaon; }
inline float StBTofPidTraits::sigmaProton() const { return mSigmaProton; }
inline float StBTofPidTraits::probElectron() const { return mProbElectron; }
inline float StBTofPidTraits::probPion() const { return mProbPion; }
inline float StBTofPidTraits::probKaon() const { return mProbKaon; }
inline float StBTofPidTraits::probProton() const { return mProbProton; }

inline void StBTofPidTraits::setMatchFlag(unsigned char flag) { mMatchFlag=flag; }
inline void StBTofPidTraits::setYLocal(float y) { mYLocal=y; }
inline void StBTofPidTraits::setZLocal(float z) { mZLocal=z; }
inline void StBTofPidTraits::setThetaLocal(float theta) { mThetaLocal=theta; }
inline void StBTofPidTraits::setTimeOfFlight(float t) { mTimeOfFlight=t; }
inline void StBTofPidTraits::setPathLength(float s) { mPathLength=s; }
inline void StBTofPidTraits::setBeta(float beta) { mBeta=beta; }
inline void StBTofPidTraits::setSigmaElectron(float sigma) { mSigmaElectron=sigma; }
inline void StBTofPidTraits::setSigmaPion(float sigma) { mSigmaPion=sigma; }
inline void StBTofPidTraits::setSigmaKaon(float sigma) { mSigmaKaon=sigma; }
inline void StBTofPidTraits::setSigmaProton(float sigma) { mSigmaProton=sigma; }
inline void StBTofPidTraits::setProbElectron(float prob) { mProbElectron=prob; }
inline void StBTofPidTraits::setProbPion(float prob) { mProbPion=prob; }
inline void StBTofPidTraits::setProbKaon(float prob) { mProbKaon=prob; }
inline void StBTofPidTraits::setProbProton(float prob) { mProbProton=prob; }

#endif
