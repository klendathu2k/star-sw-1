/*!
 * \class StTofPidTraits 
 * \author Thomas Ullrich, Dec 2000
 */
/***************************************************************************
 *
 * $Id: StTofPidTraits.h,v 2.7 2004/07/15 16:36:25 ullrich Exp $
 *
 * Author: Thomas Ullrich, Dec 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTofPidTraits.h,v $
 * Revision 2.7  2004/07/15 16:36:25  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.6  2004/07/12 21:04:16  jeromel
 * Commented out clone()
 *
 * Revision 2.5  2004/07/08 16:56:35  ullrich
 * New class member introduced.
 *
 * Revision 2.4  2002/02/22 22:56:52  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/03/24 03:34:59  perev
 * clone() -> clone() const
 *
 * Revision 2.2  2000/12/09 02:13:23  perev
 * default StObject::clone() const used
 *
 * Revision 2.1  2000/12/08 03:52:42  ullrich
 * Initial Revision
 *
 ***************************************************************************/
#ifndef StTofPidTraits_hh
#define StTofPidTraits_hh

/// @file StTofPidTraits.h
/// @brief Particle identification traits for a track matched to the legacy STAR TOF detector.

#include "StTrackPidTraits.h"

/// @brief Particle identification traits for a track matched to the legacy STAR TOF detector.
class StTofPidTraits : public StTrackPidTraits {
public:
    /// @brief Default constructor.
    StTofPidTraits();
    /// @brief Constructs with tray/module/cell address and time-of-flight observables.
    StTofPidTraits(int, int, int, float, float, float);
    /// @brief Destructor.
    ~StTofPidTraits();
    
    //StTofPidTraits(const StTofPidTraits&) {/* nopt */}
    //StTofPidTraits& operator=(const StTofPidTraits&) {/* nopt */}

    /// @brief Returns the TOF tray number of the matched cell.
    int     tray() const;
    /// @brief Returns the module number within the tray.
    int     module() const;
    /// @brief Returns the cell number within the module.
    int     cell() const;
    /// @brief Returns the measured time of flight [ns].
    float   tof() const;
    /// @brief Returns the track path length from the interaction vertex to the hit [cm].
    float   pathLength() const;
    /// @brief Returns the particle velocity β = v/c.
    float   beta() const;

    /// @brief Returns the number of sigma from the electron hypothesis.
    float   sigmaElectron() const;
    /// @brief Returns the number of sigma from the pion hypothesis.
    float   sigmaPion() const;
    /// @brief Returns the number of sigma from the kaon hypothesis.
    float   sigmaKaon() const;
    /// @brief Returns the number of sigma from the proton hypothesis.
    float   sigmaProton() const;
        
    /// @brief Sets the number of sigma from the electron hypothesis.
    void    setSigmaElectron(float);
    /// @brief Sets the number of sigma from the pion hypothesis.
    void    setSigmaPion(float);
    /// @brief Sets the number of sigma from the kaon hypothesis.
    void    setSigmaKaon(float);
    /// @brief Sets the number of sigma from the proton hypothesis.
    void    setSigmaProton(float);

    /// @brief Prints the PID traits to the given option string.
    void    Print(Option_t *opt = "") const;

private:
    Int_t     mTray;    ///< TOF tray number of the matched cell
    Int_t     mModule;  ///< Module number within the tray
    Int_t     mCell;    ///< Cell number within the module
    Float_t   mTof;     ///< Measured time of flight [ns]
    Float_t   mPathLength;  ///< Track path length from vertex to hit [cm]
    Float_t   mBeta;    ///< Particle velocity β = v/c

    Float_t   mSigmaElectron;  ///< Number of sigma from electron hypothesis
    Float_t   mSigmaPion;      ///< Number of sigma from pion hypothesis
    Float_t   mSigmaKaon;      ///< Number of sigma from kaon hypothesis
    Float_t   mSigmaProton;    ///< Number of sigma from proton hypothesis

    ClassDef(StTofPidTraits,2)
};

inline int StTofPidTraits::tray() const { return mTray; }
inline int StTofPidTraits::module() const { return mModule; }
inline int StTofPidTraits::cell() const { return mCell; }
inline float StTofPidTraits::tof() const { return mTof; }
inline float StTofPidTraits::pathLength() const { return mPathLength; }
inline float StTofPidTraits::beta() const { return mBeta; }
inline float StTofPidTraits::sigmaElectron() const { return mSigmaElectron; }
inline float StTofPidTraits::sigmaPion() const { return mSigmaPion; }
inline float StTofPidTraits::sigmaKaon() const { return mSigmaKaon; }
inline float StTofPidTraits::sigmaProton() const { return mSigmaProton; }
inline void StTofPidTraits::setSigmaElectron(float sigma) { mSigmaElectron=sigma; }
inline void StTofPidTraits::setSigmaPion(float sigma) { mSigmaPion=sigma; }
inline void StTofPidTraits::setSigmaKaon(float sigma) { mSigmaKaon=sigma; }
inline void StTofPidTraits::setSigmaProton(float sigma) { mSigmaProton=sigma; }

#endif
