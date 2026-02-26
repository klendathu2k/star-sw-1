/*!
 * \class StTofHit 
 * \author Wei-Ming Zhang, Dec 2000
 */
/***************************************************************************
 *
 * $Id: StTofHit.h,v 2.11 2016/02/25 17:10:20 ullrich Exp $
 *
 * Author: Wei-Ming Zhang, Dec 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTofHit.h,v $
 * Revision 2.11  2016/02/25 17:10:20  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.10  2004/07/15 16:36:25  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.9  2004/02/05 17:59:44  ullrich
 * Changed $LINK to StLink mechanism and add new member.
 *
 * Revision 2.8  2003/09/02 17:58:05  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.7  2003/07/09 20:14:20  ullrich
 * New methods added.
 *
 * Revision 2.6  2003/05/21 18:22:46  ullrich
 * Major Revision of ToF classes (F. Geurts)
 *
 * Revision 2.5  2002/02/22 22:56:51  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.4  2001/04/16 20:49:00  ullrich
 * Fixed typo in setSlatIndex().
 *
 * Revision 2.3  2001/04/05 04:00:43  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2001/03/24 03:34:59  perev
 * clone() -> clone() const
 *
 * Revision 2.1  2000/12/21 23:52:25  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTofHit_hh
#define StTofHit_hh

/// @file StTofHit.h
/// @brief Reconstructed hit in the legacy STAR Time of Flight (TOF) detector.

#include <Stiostream.h>
#include "StHit.h"

class StParticleDefinition;
class StTrack;

/// @brief Reconstructed hit in the legacy STAR Time of Flight (TOF) detector with PID information.
class StTofHit : public StHit {
public:
    /// @brief Default constructor.
    StTofHit();
    /// @brief Destructor.
    ~StTofHit();

    /// @brief Returns the TOF tray index.
    int   trayIndex() const;
    /// @brief Returns the module index within the tray.
    int   moduleIndex() const;
    /// @brief Returns the cell index within the module.
    int   cellIndex() const;
    /// @brief Returns the DAQ channel index.
    int   daqIndex() const;
    /// @brief Returns the index of the associated cell in the collection.
    int   cellCollIndex() const;
    /// @brief Returns the measured time of flight [ns].
    float timeOfFlight() const;
    /// @brief Returns the track path length from the interaction vertex to the hit [cm].
    float pathLength() const;
    /// @brief Returns the particle velocity β = v/c.
    float beta() const;

    /// @brief Returns a pointer to the associated reconstructed track.
    StTrack*       associatedTrack();
    /// @brief Returns a const pointer to the associated reconstructed track.
    const StTrack* associatedTrack() const;

    /// @brief Returns the expected TOF for an electron hypothesis [ns].
    float tofExpectedAsElectron() const;
    /// @brief Returns the expected TOF for a pion hypothesis [ns].
    float tofExpectedAsPion() const;
    /// @brief Returns the expected TOF for a kaon hypothesis [ns].
    float tofExpectedAsKaon() const;
    /// @brief Returns the expected TOF for a proton hypothesis [ns].
    float tofExpectedAsProton() const;

    /// @brief Returns the number of sigma from the electron hypothesis.
    float sigmaElectron() const;
    /// @brief Returns the number of sigma from the pion hypothesis.
    float sigmaPion() const;
    /// @brief Returns the number of sigma from the kaon hypothesis.
    float sigmaKaon() const;
    /// @brief Returns the number of sigma from the proton hypothesis.
    float sigmaProton() const;

    /// @brief Returns the particle hypothesis used for this hit.
    StParticleDefinition*       particleHypothesis();
    /// @brief Returns the const particle hypothesis used for this hit.
    const StParticleDefinition* particleHypothesis() const;

    /// @brief Sets the TOF tray index.
    void setTrayIndex(int);
    /// @brief Sets the module index within the tray.
    void setModuleIndex(int);
    /// @brief Sets the cell index within the module.
    void setCellIndex(int);
    /// @brief Sets the index of the associated cell in the collection.
    void setCellCollIndex(int);
    /// @brief Sets the DAQ channel index.
    void setDaqIndex(int);
    /// @brief Sets the measured time of flight [ns].
    void setTimeOfFlight(float);
    /// @brief Sets the track path length [cm].
    void setPathLength(float);
    /// @brief Sets the particle velocity β = v/c.
    void setBeta(float);
    /// @brief Sets the pointer to the associated reconstructed track.
    void setAssociatedTrack(StTrack*);
    /// @brief Sets the expected TOF for an electron hypothesis [ns].
    void setTofExpectedAsElectron(float);
    /// @brief Sets the expected TOF for a pion hypothesis [ns].
    void setTofExpectedAsPion(float);
    /// @brief Sets the expected TOF for a kaon hypothesis [ns].
    void setTofExpectedAsKaon(float);
    /// @brief Sets the expected TOF for a proton hypothesis [ns].
    void setTofExpectedAsProton(float);
    /// @brief Sets the number of sigma from the electron hypothesis.
    void setSigmaElectron(float);
    /// @brief Sets the number of sigma from the pion hypothesis.
    void setSigmaPion(float);
    /// @brief Sets the number of sigma from the kaon hypothesis.
    void setSigmaKaon(float);
    /// @brief Sets the number of sigma from the proton hypothesis.
    void setSigmaProton(float);
    /// @brief Sets the particle hypothesis.
    void setParticleHypothesis(StParticleDefinition*);
    
    /// @brief Returns the detector identifier for this hit.
    StDetectorId detector() const;

 protected:
    Int_t   mTrayIndex;       ///< TOF tray index
    Int_t   mModuleIndex;     ///< Module index within tray
    Int_t   mCellIndex;       ///< Cell index within module
    Int_t   mDaqIndex;        ///< DAQ channel index
    Int_t   mCellCollIndex;   ///< Index of the associated cell in the collection
    Float_t mTimeOfFlight;    ///< Measured time of flight [ns]
    Float_t mPathLength;      ///< Track path length from vertex to hit [cm]
    Float_t mBeta;            ///< Particle velocity β = v/c
    //    StTrack *mAssociatedTrack;   //$LINK
#ifdef __CINT__
    StObjLink        mAssociatedTrack;		
#else
    StLink<StTrack>  mAssociatedTrack;		
#endif //__CINT__
    Float_t mTOFExpectedAsElectron;  ///< Expected TOF for electron hypothesis [ns]
    Float_t mTOFExpectedAsPion;      ///< Expected TOF for pion hypothesis [ns]
    Float_t mTOFExpectedAsKaon;      ///< Expected TOF for kaon hypothesis [ns]
    Float_t mTOFExpectedAsProton;    ///< Expected TOF for proton hypothesis [ns]
    Float_t mSigmaElectron;          ///< Number of sigma from electron hypothesis
    Float_t mSigmaPion;              ///< Number of sigma from pion hypothesis
    Float_t mSigmaKaon;              ///< Number of sigma from kaon hypothesis
    Float_t mSigmaProton;            ///< Number of sigma from proton hypothesis
    StParticleDefinition *mParticleHypothesis;  ///< Best particle hypothesis for this hit

    ClassDef(StTofHit,3)
};

inline StDetectorId StTofHit::detector() const {return static_cast<StDetectorId>(StHit::bits(0, 4));}

#endif
