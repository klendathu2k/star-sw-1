/*!
 * \class StMtdPidTraits 
 * \author Frank Geurts, Feb 2013
 */
/***************************************************************************
 *
 * $Id: StMtdPidTraits.h,v 2.3 2014/07/22 01:45:13 ullrich Exp $
 *
 * Author: Frank Geurts (Rice)
 ***************************************************************************
 *
 * $Log: StMtdPidTraits.h,v $
 * Revision 2.3  2014/07/22 01:45:13  ullrich
 * Added residuals (dz,dy) between matched track-hit pairs nd access functions (Rongrong Ma)
 *
 * Revision 2.2  2014/05/29 16:58:06  ullrich
 * Added new member mExpTimeOfFlight and referring access methods.
 *
 * Revision 1.2  2014/05/22 19:04:22  marr
 * locally backup /star/u/marr/data02/mtd/dev/StRoot/StEvent/StMtdPidTraits.h
 *
 * Revision 2.1  2013/02/21 00:23:09  ullrich
 * Initial Revision.
 *
 ***************************************************************************/
#ifndef StMtdPidTraits_hh
#define StMtdPidTraits_hh

/// @file StMtdPidTraits.h
/// @brief Particle identification traits for a track matched to the STAR Muon Telescope Detector (MTD).

#include "StTrackPidTraits.h"
#include "StMtdHit.h"
#include "StThreeVectorF.hh"

/// @brief Particle identification traits for a track matched to the STAR Muon Telescope Detector (MTD).
class StMtdPidTraits : public StTrackPidTraits {
public:
    /// @brief Default constructor.
    StMtdPidTraits();
    /// @brief Destructor.
    ~StMtdPidTraits();
    
    /// @brief Returns a pointer to the associated MTD hit.
    StMtdHit*       mtdHit();
    /// @brief Returns a const pointer to the associated MTD hit.
    const StMtdHit* mtdHit() const;
    
    /// Matching information
    /// @brief Returns the track-to-hit matching quality flag.
    unsigned char    matchFlag()  const;
    /// @brief Returns the local Y coordinate of the extrapolated track [cm].
    float            yLocal()     const;
    /// @brief Returns the local Z coordinate of the extrapolated track [cm].
    float            zLocal()     const;
    /// @brief Returns the local incident angle of the track [rad].
    float            thetaLocal() const;
    /// @brief Returns the residual ΔY between matched track and hit [cm].
    float            deltaY()     const;
    /// @brief Returns the residual ΔZ between matched track and hit [cm].
    float            deltaZ()     const;
    
    /// @brief Returns the extrapolated track position at the MTD hit.
    StThreeVectorF&         position();
    /// @brief Returns the const extrapolated track position at the MTD hit.
    const StThreeVectorF&   position() const;
    
    /// timing for PID
    /// @brief Returns the measured time of flight [ns].
    float   timeOfFlight() const;
    /// @brief Returns the expected time of flight from track extrapolation [ns].
    float   expTimeOfFlight() const;
    /// @brief Returns the track path length from the interaction vertex to the hit [cm].
    float   pathLength() const;
    /// @brief Returns the particle velocity β = v/c.
    float   beta() const;
    
    /// PID functions
    /// @brief Returns the number of sigma from the muon hypothesis.
    float   sigmaMuon() const;
    
    /// @brief Returns the probability of the muon hypothesis.
    float   probMuon() const;
    
    /// @brief Sets the pointer to the associated MTD hit.
    void    setMtdHit(StMtdHit*);
    
    /// @brief Sets the track-to-hit matching quality flag.
    void    setMatchFlag(unsigned char);
    /// @brief Sets the local Y coordinate of the extrapolated track [cm].
    void    setYLocal(float);
    /// @brief Sets the local Z coordinate of the extrapolated track [cm].
    void    setZLocal(float);
    /// @brief Sets the local incident angle of the track [rad].
    void    setThetaLocal(float);
    /// @brief Sets the extrapolated track position at the MTD hit.
    void    setPosition( const StThreeVectorF&);   
    /// @brief Sets the residual ΔY between matched track and hit [cm].
    void    setDeltaY(float);
    /// @brief Sets the residual ΔZ between matched track and hit [cm].
    void    setDeltaZ(float);
    
    /// @brief Sets the measured time of flight [ns].
    void    setTimeOfFlight(float);
    /// @brief Sets the expected time of flight [ns].
    void    setExpTimeOfFlight(float);
    /// @brief Sets the track path length [cm].
    void    setPathLength(float);
    /// @brief Sets the particle velocity β = v/c.
    void    setBeta(float);
    
    /// @brief Sets the number of sigma from the muon hypothesis.
    void    setSigmaMuon(float);
    
    /// @brief Sets the probability of the muon hypothesis.
    void    setProbMuon(float);
    
private:
#ifdef __CINT__
    StObjLink        mMtdHit;
#else
    StLink<StMtdHit>  mMtdHit;
#endif //__CINT__
    
    UChar_t   mMatchFlag;        ///< Track-to-hit matching quality flag
    Float_t   mYLocal;           ///< Local Y coordinate of extrapolated track [cm]
    Float_t   mZLocal;           ///< Local Z coordinate of extrapolated track [cm]
    Float_t   mThetaLocal;       ///< Local incident angle of track [rad]
    StThreeVectorF  mPosition;   ///< Extrapolated track position at the MTD hit [cm]
    
    Float_t   mTimeOfFlight;     ///< Measured time of flight [ns]
    Float_t   mPathLength;       ///< Track path length from vertex to hit [cm]
    Float_t   mBeta;             ///< Particle velocity β = v/c
    
    Float_t   mSigmaMuon;        ///< Number of sigma from muon hypothesis
    
    Float_t   mProbMuon;         ///< Probability of muon hypothesis
    Float_t   mExpTimeOfFlight;  ///< Expected time of flight from track extrapolation [ns]

    Float_t   mDeltaY;           ///< Residual ΔY between matched track-hit pair [cm]
    Float_t   mDeltaZ;           ///< Residual ΔZ between matched track-hit pair [cm]
    
    
    ClassDef(StMtdPidTraits,3)
};

inline unsigned char StMtdPidTraits::matchFlag() const       { return mMatchFlag;       }
inline float StMtdPidTraits::yLocal() const                  { return mYLocal;          }
inline float StMtdPidTraits::zLocal() const                  { return mZLocal;          }
inline float StMtdPidTraits::deltaY() const                  { return mDeltaY;          }
inline float StMtdPidTraits::deltaZ() const                  { return mDeltaZ;          }
inline float StMtdPidTraits::thetaLocal() const              { return mThetaLocal;      }
inline float StMtdPidTraits::timeOfFlight() const            { return mTimeOfFlight;    }
inline float StMtdPidTraits::expTimeOfFlight() const         { return mExpTimeOfFlight; }
inline float StMtdPidTraits::pathLength() const              { return mPathLength;      }
inline float StMtdPidTraits::beta() const                    { return mBeta;            }
inline float StMtdPidTraits::sigmaMuon() const               { return mSigmaMuon;       }
inline float StMtdPidTraits::probMuon() const                { return mProbMuon;        }

inline void StMtdPidTraits::setMatchFlag(unsigned char flag) { mMatchFlag=flag;         }
inline void StMtdPidTraits::setYLocal(float y)               { mYLocal=y;               }
inline void StMtdPidTraits::setZLocal(float z)               { mZLocal=z;               }
inline void StMtdPidTraits::setDeltaY(float y)               { mDeltaY=y;               }
inline void StMtdPidTraits::setDeltaZ(float z)               { mDeltaZ=z;               }
inline void StMtdPidTraits::setThetaLocal(float theta)       { mThetaLocal=theta;       }
inline void StMtdPidTraits::setTimeOfFlight(float t)         { mTimeOfFlight=t;         }
inline void StMtdPidTraits::setExpTimeOfFlight(float t)      { mExpTimeOfFlight=t;      }
inline void StMtdPidTraits::setPathLength(float s)           { mPathLength=s;           }
inline void StMtdPidTraits::setBeta(float beta)              { mBeta=beta;              }
inline void StMtdPidTraits::setSigmaMuon(float sigma)        { mSigmaMuon=sigma;        }
inline void StMtdPidTraits::setProbMuon(float prob)          { mProbMuon=prob;          }

#endif
