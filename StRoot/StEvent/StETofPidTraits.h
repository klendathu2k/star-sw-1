/***************************************************************************
 *
 * $Id: StETofPidTraits.h,v 2.1 2019/02/11 18:41:19 ullrich Exp $
 *
 * Author: Florian Seck, August 2018
 ***************************************************************************
 *
 * Description: Data class for storing eTOF PID information for tracks
 * matched to eTOF hits
 *
 ***************************************************************************
 *
 * $Log: StETofPidTraits.h,v $
 * Revision 2.1  2019/02/11 18:41:19  ullrich
 * Initial Revision
 *
 *
 ***************************************************************************/ 
#ifndef STETOFPIDTRAITS_H
#define STETOFPIDTRAITS_H

/// @file StETofPidTraits.h
/// @brief Particle identification traits for a track matched to the STAR Endcap Time of Flight (eTOF) detector.


#include "StTrackPidTraits.h"
#include "StETofHit.h"
#include "StThreeVectorF.hh"
#include "StEnumerations.h"



/// @brief Particle identification traits for a track matched to the STAR Endcap TOF (eTOF) detector.
class StETofPidTraits : public StTrackPidTraits {
public:
    /// @brief Default constructor.
    StETofPidTraits();
    /// @brief Destructor.
    ~StETofPidTraits();
    
    /// @brief Returns a pointer to the associated eTOF hit.
    StETofHit*       etofHit();
    /// @brief Returns a const pointer to the associated eTOF hit.
    const StETofHit* etofHit() const;
    
    /// matching information
    /// @brief Returns the track-to-hit matching quality flag.
    unsigned short   matchFlag()  const;
    /// @brief Returns the local X coordinate of the extrapolated track on the counter [cm].
    float            localX()     const;
    /// @brief Returns the local Y coordinate of the extrapolated track on the counter [cm].
    float            localY()     const;
    /// @brief Returns the local incident angle of the track [rad].
    float            thetaLocal() const;
    /// @brief Returns the residual ΔX between matched track and hit [cm].
    float            deltaX()     const;
    /// @brief Returns the residual ΔY between matched track and hit [cm].
    float            deltaY()     const;
    
    /// @brief Returns the extrapolated track position at the eTOF hit.
    StThreeVectorF&         position();
    /// @brief Returns the const extrapolated track position at the eTOF hit.
    const StThreeVectorF&   position() const;
    
    /// timing for PID
    /// @brief Returns the measured time of flight [ns].
    float   timeOfFlight() const;
    /// @brief Returns the track path length from the interaction vertex to the hit [cm].
    float   pathLength()   const;
    /// @brief Returns the particle velocity β = v/c.
    float   beta()         const;
    
    /// PID functions  --  to be added (?)


    /// setters
    /// @brief Sets the pointer to the associated eTOF hit.
    void    setETofHit( StETofHit* hit );
    
    /// @brief Sets the track-to-hit matching quality flag.
    void    setMatchFlag( const unsigned short& flag );

    /// @brief Sets the local X coordinate of the extrapolated track [cm].
    void    setLocalX(     const float& );
    /// @brief Sets the local Y coordinate of the extrapolated track [cm].
    void    setLocalY(     const float& );
    /// @brief Sets the local incident angle of the track [rad].
    void    setThetaLocal( const float& );
    /// @brief Sets the residual ΔX between matched track and hit [cm].
    void    setDeltaX(     const float& );
    /// @brief Sets the residual ΔY between matched track and hit [cm].
    void    setDeltaY(     const float& );
    
    /// @brief Sets the extrapolated track position at the eTOF hit.
    void    setPosition( const StThreeVectorF& ); 

    /// @brief Sets the measured time of flight [ns].
    void    setTimeOfFlight( const float& );
    /// @brief Sets the track path length [cm].
    void    setPathLength(   const float& );
    /// @brief Sets the particle velocity β = v/c.
    void    setBeta(         const float& );
    
private:
    StETofHit* mETofHit;   //$LINK

    UShort_t        mMatchFlag;        ///< Track-to-hit matching quality flag
    Float_t         mLocalX;           ///< Local X coordinate of extrapolated track [cm]
    Float_t         mLocalY;           ///< Local Y coordinate of extrapolated track [cm]
    Float_t         mThetaLocal;       ///< Local incident angle of track [rad]
    Float_t         mDeltaX;           ///< Residual ΔX between matched track and hit [cm]
    Float_t         mDeltaY;           ///< Residual ΔY between matched track and hit [cm]
    StThreeVectorF  mPosition;         ///< Extrapolated track position at the eTOF hit [cm]
   
    Float_t         mTimeOfFlight;     ///< Measured time of flight [ns]
    Float_t         mPathLength;       ///< Track path length from vertex to hit [cm]
    Float_t         mBeta;             ///< Particle velocity β = v/c


    ClassDef( StETofPidTraits, 1 )
};

inline unsigned short StETofPidTraits::matchFlag()     const { return mMatchFlag;    }
inline float          StETofPidTraits::localX()        const { return mLocalX;       }
inline float          StETofPidTraits::localY()        const { return mLocalY;       }
inline float          StETofPidTraits::deltaX()        const { return mDeltaX;       }
inline float          StETofPidTraits::deltaY()        const { return mDeltaY;       }
inline float          StETofPidTraits::thetaLocal()    const { return mThetaLocal;   }
inline float          StETofPidTraits::timeOfFlight()  const { return mTimeOfFlight; }
inline float          StETofPidTraits::pathLength()    const { return mPathLength;   }
inline float          StETofPidTraits::beta()          const { return mBeta;         }

inline void StETofPidTraits::setMatchFlag(     const unsigned short& flag ) { mMatchFlag    = flag;    }
inline void StETofPidTraits::setLocalX(        const float& x )             { mLocalX       = x;       }
inline void StETofPidTraits::setLocalY(        const float& y )             { mLocalY       = y;       }
inline void StETofPidTraits::setDeltaX(        const float& x )             { mDeltaX       = x;       }
inline void StETofPidTraits::setDeltaY(        const float& y )             { mDeltaY       = y;       }
inline void StETofPidTraits::setThetaLocal(    const float& theta )         { mThetaLocal   = theta;   }
inline void StETofPidTraits::setTimeOfFlight(  const float& t )             { mTimeOfFlight = t;       }
inline void StETofPidTraits::setPathLength(    const float& s )             { mPathLength   = s;       }
inline void StETofPidTraits::setBeta(          const float& beta )          { mBeta         = beta;    }

#endif
