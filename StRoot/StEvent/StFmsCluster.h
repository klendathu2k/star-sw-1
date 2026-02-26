/****************************************************************************
 *
 * $Id: StFmsCluster.h,v 2.2 2015/08/26 16:51:59 ullrich Exp $
 *
 * Author: Thomas Burton, Yuxi Pan, 2014
 ****************************************************************************
 *
 * Description: Implementation of StFmsCluster, a group of
 *              adjacent FMS towers.
 * A cluster is formed by the energy depositions (hits) of one or more
 * particles over a group of FMS towers.
 ****************************************************************************
 *
 * $Log: StFmsCluster.h,v $
 * Revision 2.2  2015/08/26 16:51:59  ullrich
 * Added print out fct and operator.
 *
 * Revision 2.1  2015/02/14 18:56:00  ullrich
 * Initial Revision.
 *
 *
 ****************************************************************************/
#ifndef STFMSCLUSTER_H
#define STFMSCLUSTER_H

/// @file StFmsCluster.h
/// @brief FMS cluster: a group of adjacent FMS calorimeter towers sharing a common shower.

#include "StLorentzVectorF.hh"

#include "StObject.h"

#include "StContainers.h"  // For StPtrVecFmsHit, StPtrVecFmsPoint
#include "StEnumerations.h"


/// @brief A cluster of adjacent FMS towers formed by energy deposits from one or more particles.
class StFmsCluster : public StObject {
public:
    /// @brief Default constructor.
    StFmsCluster();
    /// @brief Destructor.
    ~StFmsCluster();
    
    /// @brief Return the detector ID.
    unsigned short detectorId() const;
    /// @brief Return the cluster category (see StFmsClusterCategory).
    int category() const;
    /// @brief Return the number of towers in the cluster.
    int nTowers() const;
    /// @brief Return the number of fitted photon points in the cluster.
    int nPhotons() const;
    /// @brief Return the total cluster energy (0th moment).
    float energy() const;
    float x() const;  ///< Mean x ("center of gravity") in local grid coordinate (1st moment).
    float y() const;  ///< Mean y ("center of gravity") in local grid coordinate (1st moment).
    float sigmaMax() const; ///< Maximum 2nd moment (along major axis).
    float sigmaMin() const; ///< Minimum 2nd moment.
    float chi2Ndf1Photon() const; ///< chi^2/ndf for 1-photon fit.
    float chi2Ndf2Photon() const; ///< chi^2/ndf for 2-photon fit.
    int id() const; ///< Eventwise cluster ID.
    const StLorentzVectorF& fourMomentum() const; ///< Cluster four-momentum.
    /// @brief Set the detector ID.
    void setDetectorId(unsigned short detector);
    /// @brief Set the cluster category.
    void setCategory(int catag);
    /// @brief Set the number of towers.
    void setNTowers(int numbTower);
    /// @brief Set the total cluster energy.
    void setEnergy(float energy);
    /// @brief Set the x position of the cluster centroid in local coordinates.
    void setX(float x0);
    /// @brief Set the y position of the cluster centroid in local coordinates.
    void setY(float y0);
    /// @brief Set the minimum 2nd moment.
    void setSigmaMin(float sigmaMax);
    /// @brief Set the maximum 2nd moment.
    void setSigmaMax(float sigmaMax);
    /// @brief Set chi^2/ndf for 1-photon fit.
    void setChi2Ndf1Photon(float chi2ndfph1);
    /// @brief Set chi^2/ndf for 2-photon fit.
    void setChi2Ndf2Photon(float chi2ndfph2);
    /// @brief Set the eventwise cluster ID.
    void setId(float cluid);
    /// @brief Set the cluster four-momentum.
    void setFourMomentum(StLorentzVectorF p4);
    /// @brief Return the list of tower hits in this cluster.
    StPtrVecFmsHit& hits();
    const StPtrVecFmsHit& hits() const;
    /// @brief Return the list of fitted photon points in this cluster.
    StPtrVecFmsPoint& points();
    const StPtrVecFmsPoint& points() const;
   
    /// @brief Print cluster properties.
    void print(Option_t *option="") const;

private:
    UShort_t mDetectorId;  ///< Detector ID, starts from 1.
    Int_t mCategory;  ///< Category of cluster (see StFmsClusterCategory).
    Int_t mNTowers;  ///< Number of non-zero-energy tower hits in the cluster.
    Float_t mEnergy;  ///< Total energy of the cluster (0th moment).
    Float_t mX;  ///< Mean x position (centroid) in local grid coordinates (1st moment).
    Float_t mY;  ///< Mean y position (centroid) in local grid coordinates (1st moment).
    Float_t mSigmaMin;  ///< Minimum 2nd moment (along minor axis).
    Float_t mSigmaMax;  ///< Maximum 2nd moment (along major axis).
    Float_t mChi2Ndf1Photon;  ///< chi^2/ndf for 1-photon fit.
    Float_t mChi2Ndf2Photon;  ///< chi^2/ndf for 2-photon fit.
    Int_t mId;  ///< Eventwise cluster ID.
    StLorentzVectorF mFourMomentum;  ///< Cluster four-momentum.
    StPtrVecFmsPoint mPhotons; ///< Fitted photon points in this cluster.
    StPtrVecFmsHit mHits;  ///< Tower hits belonging to this cluster.

    ClassDef(StFmsCluster, 1)
};

ostream& operator<<(ostream&, const StFmsCluster&);

//
//   Inline functions
//
inline unsigned short StFmsCluster::detectorId() const { return mDetectorId; }
inline int StFmsCluster::category() const { return mCategory; }
inline int StFmsCluster::nTowers() const { return mNTowers; }
inline int StFmsCluster::nPhotons() const { return mPhotons.size(); }
inline float StFmsCluster::energy() const { return mEnergy; }
inline float StFmsCluster::x() const { return mX; } // Mean x ("center of gravity") in local grid coordinate (1st moment).
inline float StFmsCluster::y() const { return mY; } // Mean y ("center of gravity") in local grid coordinate (1st moment).
inline float StFmsCluster::sigmaMax() const { return mSigmaMax; } // Maximum 2nd moment (along major axis).
inline float StFmsCluster::sigmaMin() const { return mSigmaMin; } // Minimum 2nd moment.
inline float StFmsCluster::chi2Ndf1Photon() const { return mChi2Ndf1Photon; } // chi^2/ndf for 1-photon fit to the cluster.
inline float StFmsCluster::chi2Ndf2Photon() const { return mChi2Ndf2Photon; } // chi^2/ndf for 2-photon fit to the cluster.
inline int StFmsCluster::id() const { return mId; } // Cluster ID
inline const StLorentzVectorF& StFmsCluster::fourMomentum() const { return mFourMomentum; } // Cluster four-momentum (px, py, pz, E)
inline void StFmsCluster::setDetectorId(unsigned short detector) { mDetectorId = detector; }
inline void StFmsCluster::setCategory(int catag) { mCategory = catag; }
inline void StFmsCluster::setNTowers(int numbTower) { mNTowers = numbTower; }
inline void StFmsCluster::setEnergy(float energy) { mEnergy = energy; }
inline void StFmsCluster::setX(float x0) { mX = x0; }
inline void StFmsCluster::setY(float y0) { mY = y0; }
inline void StFmsCluster::setSigmaMin(float sigmaMax) { mSigmaMin = sigmaMax; }
inline void StFmsCluster::setSigmaMax(float sigmaMax) { mSigmaMax = sigmaMax; }
inline void StFmsCluster::setChi2Ndf1Photon(float chi2ndfph1) { mChi2Ndf1Photon = chi2ndfph1; }
inline void StFmsCluster::setChi2Ndf2Photon(float chi2ndfph2) { mChi2Ndf2Photon = chi2ndfph2; }
inline void StFmsCluster::setId(float cluid) { mId = cluid; }
inline void StFmsCluster::setFourMomentum(StLorentzVectorF p4) { mFourMomentum = p4; }
inline StPtrVecFmsHit& StFmsCluster::hits() { return mHits; }
inline const StPtrVecFmsHit& StFmsCluster::hits() const { return mHits; }
inline StPtrVecFmsPoint& StFmsCluster::points() { return mPhotons; }
inline const StPtrVecFmsPoint& StFmsCluster::points() const { return mPhotons; }


#endif  // STFMSCLUSTER_H
