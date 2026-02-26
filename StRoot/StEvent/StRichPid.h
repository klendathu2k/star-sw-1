/*!
 * \class StRichPid 
 * \author Matt Horsley, Sep 2000
 */
/***************************************************************************
 *
 * $Id: StRichPid.h,v 2.7 2002/02/22 22:56:49 jeromel Exp $
 *
 * Author: Matt Horsley, Sep 2000
 ***************************************************************************
 *
 * Description: Definition of PID object
 *
 ***************************************************************************
 *
 * $Log: StRichPid.h,v $
 * Revision 2.7  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.6  2001/04/05 04:00:40  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.5  2000/11/27 17:19:09  lasiuk
 * keep the constant angle
 *
 * Revision 2.4  2000/11/25 11:51:52  lasiuk
 * remove D vector and replace with a container of StRichPhotonInfo
 *
 * Revision 2.3  2000/11/21 19:47:36  lasiuk
 * add the d information for each hit
 * use the TArrayF
 *
 * Revision 2.2  2000/11/01 16:45:46  lasiuk
 * Keep the pointers to the hits that are associated with the track
 * in order to use the bit flag information.  These are kept
 * in an StPtrVec (does not own the hits)  The PDG encoded number
 * is kept as a data member now
 *
 * Revision 2.1  2000/09/28 10:54:00  ullrich
 * Initial Revision.
 *
 ***************************************************************************/
#ifndef StRichPid_hh
#define StRichPid_hh

/// @file StRichPid.h
/// @brief RICH particle-identification hypothesis for a single particle species.

#include "StObject.h"
#include "StContainers.h"

#include "StRichHit.h"

#include "StParticleDefinition.hh"
#include "StThreeVectorD.hh"
#include "StEnumerations.h"


/// @brief RICH PID hypothesis for one particle species, containing ring statistics and associated hits.
class StRichPid : public StObject {
public:
    /// @brief Default constructor.
    StRichPid();
    ~StRichPid();
    /// @brief Constructor with particle type, MIP residual, and full/truncated ring statistics.
    StRichPid(StParticleDefinition* particle, StThreeVectorD resid,
              float totAzim,  float totArea,
              unsigned short totHits, float trunAzim,
              float trunArea, unsigned short trunHits);

    // StRichPid(const StRichPid&) {}
    // StRichPid& operator=(const StRichPid&) {}

    int operator==(const StRichPid&) const;

    /// @brief Returns a const reference to the RICH hits associated with this hypothesis.
    const StPtrVecRichHit&      getAssociatedRichHits() const;
    /// @brief Returns a mutable reference to the RICH hits associated with this hypothesis.
    StPtrVecRichHit&            getAssociatedRichHits();
    /// @brief Adds a RICH hit to the list of hits associated with this hypothesis.
    void                        addHit(StRichHit*);

    /// @brief Returns a const reference to the photon-geometry information vector.
    const StSPtrVecRichPhotonInfo& getPhotonInfo();
    /// @brief Returns a pointer to the photon-geometry record at index i.
    StRichPhotonInfo*              getPhotonInfo(int);   // should be constant
    /// @brief Appends a photon-geometry record to the collection.
    void                           addPhotonInfo(StRichPhotonInfo*);

    /// @brief Sets the particle species hypothesis for this Cherenkov ring.
    void setRingType(StParticleDefinition* particle);
    /// @brief Sets the residual between the MIP impact point and the ring centre.
    void setMipResidual(StThreeVectorD t);

    /// @brief Sets the total azimuthal coverage of the Cherenkov ring (radians).
    void setTotalAzimuth(float);
    /// @brief Sets the total pad-plane area subtended by the Cherenkov ring.
    void setTotalArea(float);
    /// @brief Sets the total number of photon hits on the full ring.
    void setTotalHits(unsigned short);
    /// @brief Sets the photon hit density for the full ring.
    void setTotalDensity(float);

    /// @brief Returns the total azimuthal coverage of the Cherenkov ring (radians).
    float  getTotalAzimuth() const;
    /// @brief Returns the total pad-plane area subtended by the Cherenkov ring.
    float  getTotalArea()    const;
    /// @brief Returns the total number of photon hits on the full ring.
    unsigned short getTotalHits()    const;
    /// @brief Returns the photon hit density for the full ring.
    float  getTotalDensity() const;

    /// @brief Sets the truncated (constant-area cut) azimuthal coverage.
    void setTruncatedAzimuth(float);
    /// @brief Sets the truncated pad-plane area after the constant-area cut.
    void setTruncatedArea(float);
    /// @brief Sets the number of photon hits surviving the constant-area cut.
    void setTruncatedHits(unsigned short);
    /// @brief Sets the photon hit density after the constant-area cut.
    void setTruncatedDensity(float);

    /// @brief Returns the truncated azimuthal coverage after the constant-area cut.
    float  getTruncatedAzimuth() const;
    /// @brief Returns the truncated pad-plane area after the constant-area cut.
    float  getTruncatedArea()    const;
    /// @brief Returns the number of photon hits after the constant-area cut.
    unsigned short getTruncatedHits()    const;
    /// @brief Returns the photon hit density after the constant-area cut.
    float  getTruncatedDensity() const;

    /// @brief Returns the constant pad-plane area cut value used in truncated calculations.
    float  getConstantAreaCut()  const;
    /// @brief Sets the constant pad-plane area cut value.
    void   setConstantAreaCut(float);

    /// @brief Returns a pointer to the particle species definition for this hypothesis.
    StParticleDefinition* getRingType()       const;
    /// @brief Returns the PDG-encoded particle number for this hypothesis.
    int                 getParticleNumber() const;

    /// @brief Returns the residual between the MIP impact point and the ring centre.
    StThreeVectorD        getMipResidual() const;

    /// @brief Returns true if the specified PID flag bit is set.
    bool isSet(StRichPidFlag) const;
    /// @brief Sets the specified PID flag bit.
    void setBit(StRichPidFlag);
    /// @brief Clears the specified PID flag bit.
    void unSetBit(StRichPidFlag);

private:
    StParticleDefinition*  mParticleType;//!
    Int_t                  mParticleNumber;     ///< PDG-encoded particle number

    StPtrVecRichHit         mAssociatedHits;    ///< RICH hits on the ring for this hypothesis (not owned)
    StSPtrVecRichPhotonInfo mPhotonInfo;        ///< Per-photon geometry records

    StThreeVectorD          mMipResidual;       ///< Residual between MIP impact point and ring centre

    Float_t  mTotalAzimuth;     ///< Azimuthal coverage of the full ring (radians)
    Float_t  mTotalArea;        ///< Pad-plane area of the full ring
    UShort_t mTotalHits;        ///< Number of photon hits on the full ring
    Float_t  mTotalDensity;     ///< Photon hit density on the full ring

    Float_t  mTruncatedAzimuth; ///< Azimuthal coverage after constant-area cut
    Float_t  mTruncatedArea;    ///< Pad-plane area after constant-area cut
    UShort_t mTruncatedHits;    ///< Number of photon hits after constant-area cut
    Float_t  mTruncatedDensity; ///< Photon hit density after constant-area cut

    Float_t  mConstantAreaCut;  ///< Constant pad-plane area cut applied in truncated calculations

    UInt_t   mFlags;            ///< PID status flag bit field

    ClassDef(StRichPid,1)
};


// sets
inline void StRichPid::setMipResidual(StThreeVectorD t) { mMipResidual=t;}

inline void StRichPid::setTotalAzimuth(float t) { mTotalAzimuth=t;}
inline void StRichPid::setTotalArea(float t)    { mTotalArea=t;}
inline void StRichPid::setTotalHits(unsigned short t)   { mTotalHits=t;}
inline void StRichPid::setTotalDensity(float t) {mTotalDensity=t;}

inline void StRichPid::setTruncatedAzimuth(float t) { mTruncatedAzimuth=t;}
inline void StRichPid::setTruncatedArea(float t)    { mTruncatedArea=t;}
inline void StRichPid::setTruncatedHits(unsigned short t)   { mTruncatedHits=t;}
inline void StRichPid::setTruncatedDensity(float t) {mTruncatedDensity=t;}


// containers
inline const StPtrVecRichHit& StRichPid::getAssociatedRichHits() const {return mAssociatedHits;}
inline StPtrVecRichHit&       StRichPid::getAssociatedRichHits() {return mAssociatedHits;}
inline void  StRichPid::addHit(StRichHit* hit) { mAssociatedHits.push_back(hit); }

// gets

inline StParticleDefinition* StRichPid::getRingType() const { return mParticleType;}
inline int StRichPid::getParticleNumber() const {return mParticleNumber;}
inline float  StRichPid::getTotalAzimuth() const { return mTotalAzimuth;}
inline float  StRichPid::getTotalArea()    const { return mTotalArea;}
inline unsigned short StRichPid::getTotalHits()    const { return mTotalHits;}
inline float  StRichPid::getTotalDensity() const { return mTotalDensity;}

inline float  StRichPid::getTruncatedAzimuth() const { return mTruncatedAzimuth;}
inline float  StRichPid::getTruncatedArea()    const { return mTruncatedArea;}
inline unsigned short StRichPid::getTruncatedHits()    const { return mTruncatedHits;}
inline float  StRichPid::getTruncatedDensity() const { return mTruncatedDensity;}

inline float  StRichPid::getConstantAreaCut() const {return mConstantAreaCut;}
inline void     StRichPid::setConstantAreaCut(float c) { mConstantAreaCut = c;}



inline StThreeVectorD  StRichPid::getMipResidual() const     { return mMipResidual;}


// Flag operations
inline bool StRichPid::isSet(StRichPidFlag f) const { return (mFlags & f);}
inline void StRichPid::setBit(StRichPidFlag f)   { (mFlags |= f);}
inline void StRichPid::unSetBit(StRichPidFlag f) { (mFlags &= ~(f));}

//non-members
//ostream& operator<<(ostream& os, const StRichPid& hit);
#endif
