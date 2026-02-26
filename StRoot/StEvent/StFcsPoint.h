/**************************************************************************
 *
 * $Id: StFcsPoint.h,v 2.1 2021/01/11 20:25:37 ullrich Exp $
 *
 * Author: Akio Ogawa 2018
 **************************************************************************
 *
 * Description: Declaration of StFcsPoint, the StEvent FCS photon structure
 * Represents a "point" (photon etc) fitted to a cluster of FCS towers.
 *
 **************************************************************************
 *
 * $Log: StFcsPoint.h,v $
 * Revision 2.1  2021/01/11 20:25:37  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StFcsPoint_h
#define StFcsPoint_h

/// @file StFcsPoint.h
/// @brief Photon point reconstructed from an FCS cluster.

#include "StLorentzVectorD.hh"
#include "StThreeVectorD.hh"
#include "StObject.h"
#include "StFcsCluster.h"
#include "StEnumerations.h"

/// @brief A photon (or other particle) point fitted to an FCS tower cluster.
class StFcsPoint : public StObject {
public:
    /// @brief Default constructor.
    StFcsPoint();
    /// @brief Destructor.
    ~StFcsPoint();

    /// @brief Return the FCS sub-detector ID.
    unsigned short detectorId() const;
    /// @brief Return the fitted photon energy (GeV).
    float energy() const;
    float x() const;  ///< x position in local tower-grid units (cell units).
    float y() const;  ///< y position in local tower-grid units (cell units).
    unsigned int parentClusterId() const; ///< ID of the parent cluster containing this point.
    StFcsCluster* cluster(); ///< Return the parent cluster of this photon point.
    int nParentClusterPhotons()   const; ///< Number of photon points in the parent cluster.
    const StThreeVectorD& xyz()   const; ///< Position in global STAR coordinates (cm).
    /// @brief Return the photon four-momentum.
    const StLorentzVectorD& fourMomentum() const;

    /// @brief Set the sub-detector ID.
    void setDetectorId(unsigned short detectorId);
    /// @brief Set the fitted photon energy.
    void setEnergy(float energy);
    /// @brief Set the x position in local coordinates.
    void setX(float x);
    /// @brief Set the y position in local coordinates.
    void setY(float y);
    /// @brief Set the parent cluster pointer.
    void setCluster(StFcsCluster* cluster);
    /// @brief Set the number of photons in the parent cluster.
    void setNParentClusterPhotons(int nclph);
    /// @brief Set the photon position in global STAR coordinates.
    void setXYZ(const StThreeVectorD& p3);
    /// @brief Set the photon four-momentum.
    void setFourMomentum(const StLorentzVectorD& p4);
    
    /// @brief Print photon point properties.
    void print(int option=0);

private:
    UShort_t mDetectorId=0;  ///< FCS sub-detector ID (North ECal/HCal=0/2, South ECal/HCal=1/3).
    Float_t  mEnergy=0;      ///< Fitted photon energy (GeV).
    Float_t  mX=0.0;         ///< Fitted x-position in local tower-grid units.
    Float_t  mY=0.0;         ///< Fitted y-position in local tower-grid units.
    Int_t    mNParentClusterPhotons=0;  ///< Number of photons in the parent cluster
    StFcsCluster* mCluster=0;        ///< Pointer to the parent cluster (not owned).
    StLorentzVectorD mFourMomentum;  ///< Photon four-momentum.  ///< Photon 4-momentum
    StThreeVectorD   mXYZ;           ///< Photon position in global STAR coordinates (cm).

    ClassDef(StFcsPoint, 1)
};

inline unsigned short StFcsPoint::detectorId() const { return mDetectorId; }
inline float StFcsPoint::energy() const { return mEnergy; }
inline float StFcsPoint::x() const { return mX; } // x position in cm at which point intersects the sub-detector.
inline float StFcsPoint::y() const { return mY; } // y position in cm at which point intersects the sub-detector.
inline unsigned int StFcsPoint::parentClusterId() const { return mCluster->id(); } //parent cluster Id
inline StFcsCluster* StFcsPoint::cluster() { return mCluster; } //  Parent cluster of the photon.
inline int StFcsPoint::nParentClusterPhotons() const { return mNParentClusterPhotons; } // Number of points in parent cluster
inline const StThreeVectorD& StFcsPoint::xyz() const { return mXYZ; }
inline const StLorentzVectorD& StFcsPoint::fourMomentum() const { return mFourMomentum; }
inline void StFcsPoint::setDetectorId(unsigned short det) { mDetectorId = det; }
inline void StFcsPoint::setEnergy(float energy) { mEnergy = energy; }
inline void StFcsPoint::setX(float xpos) { mX = xpos; }
inline void StFcsPoint::setY(float ypos) { mY = ypos; }
inline void StFcsPoint::setCluster(StFcsCluster* cluster) { mCluster = cluster; }
inline void StFcsPoint::setNParentClusterPhotons(int nclph) { mNParentClusterPhotons = nclph; }
inline void StFcsPoint::setXYZ(const StThreeVectorD& p3) { mXYZ = p3; }
inline void StFcsPoint::setFourMomentum(const StLorentzVectorD& p4) { mFourMomentum = p4; }

#endif  // StFcsPoint_h

