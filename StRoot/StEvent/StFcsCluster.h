/****************************************************************************
 *
 * $Id: StFcsCluster.h,v 2.1 2021/01/11 20:25:37 ullrich Exp $
 *
 * Author: Akio Ogawa 2018
 ****************************************************************************
 *
 * Description: Implementation of StFcsCluster, a group ofadjacent FCS towers.
 * A cluster is formed by the energy depositions (hits) of one or more
 * particles over a group of FCS towers.
 ****************************************************************************
 *
 * $Log: StFcsCluster.h,v $
 * Revision 2.1  2021/01/11 20:25:37  ullrich
 * Initial Revision
 *
 ****************************************************************************/
#ifndef STFCSCLUSTER_H
#define STFCSCLUSTER_H

/// @file StFcsCluster.h
/// @brief Cluster of adjacent FCS (Forward Calorimeter System) towers.

#include "StLorentzVectorD.hh"

#include "StObject.h"

#include "StEvent/StContainers.h"  // For StPtrVecFcsHit, StPtrVecFcsPoint
#include "StEvent/StEnumerations.h"

class StFcsPoint;
class StFwdTrack;

/// @brief A group of adjacent FCS towers formed by energy deposits from one or more particles.
class StFcsCluster : public StObject {
public:
    /// @brief Default constructor.
    StFcsCluster();
    /// @brief Destructor.
    ~StFcsCluster();
    
    int id() const; ///< Eventwise cluster ID.
    unsigned short detectorId() const; ///< FCS sub-detector ID.
    int category() const; ///< Cluster category (see StFcsClusterCategory).
    int nTowers() const; ///< Number of non-zero-energy tower hits in this cluster.
    int nNeighbor() const; ///< Number of neighbouring clusters.
    int nPoints() const; ///< Number of fitted photon points in this cluster.
    float energy() const; ///< Total cluster energy (GeV).
    float x() const;  ///< Mean x position (centroid) in local tower-grid coordinates (1st moment).
    float y() const;  ///< Mean y position (centroid) in local tower-grid coordinates (1st moment).
    float sigmaMax() const; ///< Maximum 2nd moment (along the major axis).
    float sigmaMin() const; ///< Minimum 2nd moment (along the minor axis).
    float theta() const;    ///< Angle in the x-y plane defining the minor-axis direction (radians).
    float chi2Ndf1Photon() const; ///< chi^2/ndf for the 1-photon shape fit.
    float chi2Ndf2Photon() const; ///< chi^2/ndf for the 2-photon shape fit.
    const StLorentzVectorD& fourMomentum() const; ///< Cluster four-momentum.

    /// @brief Set the eventwise cluster ID.
    void setId(int cluid);
    /// @brief Set the detector ID.
    void setDetectorId(unsigned short detector);
    /// @brief Set the cluster category.
    void setCategory(int catag);
    /// @brief Set the number of towers.
    void setNTowers(int numbTower);
    /// @brief Set the total cluster energy.
    void setEnergy(float energy);
    /// @brief Set the x centroid in local coordinates.
    void setX(float x0);
    /// @brief Set the y centroid in local coordinates.
    void setY(float y0);
    /// @brief Set the minimum 2nd moment.
    void setSigmaMin(float sigmaMax);
    /// @brief Set the maximum 2nd moment.
    void setSigmaMax(float sigmaMax);
    /// @brief Set the minor-axis angle.
    void setTheta(float theta);
    /// @brief Set chi^2/ndf for the 1-photon fit.
    void setChi2Ndf1Photon(float chi2ndfph1);
    /// @brief Set chi^2/ndf for the 2-photon fit.
    void setChi2Ndf2Photon(float chi2ndfph2);
    /// @brief Set the cluster four-momentum.
    void setFourMomentum(StLorentzVectorD p4);
    // Hits
    /// @brief Return the list of tower hits in this cluster.
    StPtrVecFcsHit& hits();
    const StPtrVecFcsHit& hits() const;
    // Neighbors
    /// @brief Add a neighbouring cluster.
    void addNeighbor(StFcsCluster* neighbor);
    /// @brief Return the list of neighbouring clusters.
    StPtrVecFcsCluster& neighbor();
    const StPtrVecFcsCluster& neighbor() const;    
    // Points
    /// @brief Return the list of fitted photon points.
    StPtrVecFcsPoint& points();
    const StPtrVecFcsPoint& points() const;
    /// @brief Add a single fitted photon point.
    void addPoint(StFcsPoint* p);
    /// @brief Add two fitted photon points (2-photon fit result).
    void addPoint(StFcsPoint* p1, StFcsPoint* p2);
    // Tracks
    /// @brief Return the list of associated forward tracks.
    StPtrVecFwdTrack& tracks();
    const StPtrVecFwdTrack& tracks() const;
    /// @brief Associate a forward track with this cluster.
    void addTrack(StFwdTrack* p);
    /// @brief Sort associated tracks by descending transverse momentum.
    void sortTrackByPT();

    /// @brief Print cluster properties.
    void print(Option_t *option="") const;

private:
    Int_t mId=-1;             ///< Eventwise cluster ID.
    UShort_t mDetectorId=0;   ///< FCS sub-detector ID, starts from 1.
    Int_t mCategory=0;        ///< Cluster category (see StFcsClusterCategory).
    Int_t mNTowers=0;         ///< Number of non-zero-energy tower hits.
    Float_t mEnergy=0.0;      ///< Total cluster energy, i.e. 0th moment (GeV).
    Float_t mX=0.0;  ///< Mean x centroid in local tower-grid coordinates (1st moment).
    Float_t mY=0.0;  ///< Mean y centroid in local tower-grid coordinates (1st moment).
    Float_t mSigmaMin=0.0;        ///< Minimum 2nd moment (minor-axis width).
    Float_t mSigmaMax=0.0;        ///< Maximum 2nd moment (major-axis width).
    Float_t mTheta=0.0;           ///< Angle of the minor axis in the x-y plane (radians).
    Float_t mChi2Ndf1Photon=0.0;  ///< chi^2/ndf for the 1-photon shape fit.
    Float_t mChi2Ndf2Photon=0.0;  ///< chi^2/ndf for the 2-photon shape fit.
    StLorentzVectorD mFourMomentum;  ///< Cluster four-momentum.
    StPtrVecFcsHit mHits;            ///< Tower hits belonging to this cluster.
    StPtrVecFcsCluster mNeighbor;    ///< Neighbouring clusters.
    StPtrVecFcsPoint mPoints;        ///< Fitted photon points in this cluster.
    StPtrVecFwdTrack mTracks;        ///< Forward tracks associated with this cluster.

    ClassDef(StFcsCluster, 3)
};


inline int StFcsCluster::id() const { return mId; } // Cluster ID
inline unsigned short StFcsCluster::detectorId() const { return mDetectorId; }
inline int StFcsCluster::category() const { return mCategory; }
inline int StFcsCluster::nTowers() const { return mNTowers; }
inline int StFcsCluster::nNeighbor() const { return mNeighbor.size(); }
inline int StFcsCluster::nPoints() const { return mPoints.size(); }
inline float StFcsCluster::energy() const { return mEnergy; }
inline float StFcsCluster::x() const { return mX; } // Mean x ("center of gravity") in local grid coordinate (1st moment).
inline float StFcsCluster::y() const { return mY; } // Mean y ("center of gravity") in local grid coordinate (1st moment).
inline float StFcsCluster::sigmaMax() const { return mSigmaMax; } // Maximum 2nd moment (along major axis).
inline float StFcsCluster::sigmaMin() const { return mSigmaMin; } // Minimum 2nd moment.
inline float StFcsCluster::theta() const { return mTheta; } // Angle in x-y plane that defines the direction of least-2nd-sigma
inline float StFcsCluster::chi2Ndf1Photon() const { return mChi2Ndf1Photon; } // chi^2/ndf for 1-photon fit to the cluster.
inline float StFcsCluster::chi2Ndf2Photon() const { return mChi2Ndf2Photon; } // chi^2/ndf for 2-photon fit to the cluster.
inline const StLorentzVectorD& StFcsCluster::fourMomentum() const { return mFourMomentum; } // Cluster four-momentum (px, py, pz, E)
inline void StFcsCluster::setDetectorId(unsigned short detector) { mDetectorId = detector; }
inline void StFcsCluster::setCategory(int catag) { mCategory = catag; }
inline void StFcsCluster::setNTowers(int numbTower) { mNTowers = numbTower; }
inline void StFcsCluster::setEnergy(float energy) { mEnergy = energy; }
inline void StFcsCluster::setX(float x0) { mX = x0; }
inline void StFcsCluster::setY(float y0) { mY = y0; }
inline void StFcsCluster::setSigmaMin(float sigmaMax) { mSigmaMin = sigmaMax; }
inline void StFcsCluster::setSigmaMax(float sigmaMax) { mSigmaMax = sigmaMax; }
inline void StFcsCluster::setTheta(float theta) { mTheta = theta; }
inline void StFcsCluster::setChi2Ndf1Photon(float chi2ndfph1) { mChi2Ndf1Photon = chi2ndfph1; }
inline void StFcsCluster::setChi2Ndf2Photon(float chi2ndfph2) { mChi2Ndf2Photon = chi2ndfph2; }
inline void StFcsCluster::setId(int cluid) { mId = cluid; }
inline void StFcsCluster::setFourMomentum(StLorentzVectorD p4) { mFourMomentum = p4; }
inline StPtrVecFcsHit& StFcsCluster::hits() { return mHits; }
inline const StPtrVecFcsHit& StFcsCluster::hits() const { return mHits; }
inline StPtrVecFcsCluster& StFcsCluster::neighbor() { return mNeighbor; }
inline const StPtrVecFcsCluster& StFcsCluster::neighbor() const { return mNeighbor; }
inline StPtrVecFcsPoint& StFcsCluster::points() { return mPoints; }
inline const StPtrVecFcsPoint& StFcsCluster::points() const { return mPoints; }
inline StPtrVecFwdTrack& StFcsCluster::tracks() { return mTracks; }
inline const StPtrVecFwdTrack& StFcsCluster::tracks() const { return mTracks; }

#endif  // STFCSCLUSTER_H
