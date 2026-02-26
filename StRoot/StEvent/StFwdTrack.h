/***************************************************************************
 *
 * $Id: StFwdTrack.h,v 2.1 2021/01/11 20:25:37 ullrich Exp $
 *
 * Author: jdb, Feb 2022
 ***************************************************************************
 *
 * Description: StFwdTrack stores the Forward tracks built from Fst and Ftt
 *
 ***************************************************************************
 *
 * $Log: StFwdTrack.h,v $
 * Revision 2.1  2021/01/11 20:25:37  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StFwdTrack_hh
#define StFwdTrack_hh

/// @file StFwdTrack.h
/// @brief Data structures for forward-rapidity tracks reconstructed by the STAR Forward Tracker.

#include "Stiostream.h"
#include "StObject.h"
#include <vector>
#include "StThreeVectorD.hh"
#include "StContainers.h"
#include <climits>

class StFcsCluster;


/// @brief Stores the projected position and momentum of a forward track at a specific sub-detector plane.
struct StFwdTrackProjection : public StObject {
    StFwdTrackProjection() {}
    StFwdTrackProjection ( const StFwdTrackProjection & other) {
        mXYZ = other.mXYZ;
        mMom = other.mMom;
        mDetId = other.mDetId;
        memcpy( mCov, other.mCov, sizeof( mCov ) );
    }
    StFwdTrackProjection(   unsigned short detId, 
                            StThreeVectorD xyz, 
                            StThreeVectorD mom, 
                            float c[9] ) {
        set( detId, xyz, mom, c );
    }

    void set(   unsigned short detId, 
                StThreeVectorD xyz, 
                StThreeVectorD mom, 
                float c[9]) {
        mDetId = detId;
        mXYZ = xyz;
		mMom = mom;
        memcpy( mCov, c, sizeof(mCov) ); 
    }
    void set(const StFwdTrackProjection &other ){
        mDetId = other.mDetId;
        mXYZ   = other.mXYZ;
        mMom   = other.mMom;
        memcpy( mCov, other.mCov, sizeof(mCov) ); 
    }
    StThreeVectorD mXYZ;   ///< Projected position at the detector surface (cm, global STAR coordinates).
    StThreeVectorD mMom;   ///< Projected momentum at the detector surface (GeV/c).
    unsigned char mDetId;  ///< Detector identifier for this projection surface.
    float mCov[9];         ///< 3×3 position covariance matrix (row-major, cm²).

    float dx(){
        return sqrt( mCov[0] );
    }
    float dy(){
        return sqrt( mCov[4] );
    }
    float dz(){
        return sqrt( mCov[8] );
    }

    ClassDef(StFwdTrackProjection, 1)
};

/// @brief Stores a single seed-step hit point used in forward track finding.
struct StFwdTrackSeedPoint : public StObject {
    StFwdTrackSeedPoint() {}
    StFwdTrackSeedPoint(    StThreeVectorD xyz, 
                            short detsec, 
                            unsigned short trackId, 
                            float cov[9] ){
        mXYZ = xyz;
        mSector = detsec;
        mTrackId = trackId;
        memcpy( mCov, cov, sizeof( mCov ));
    }

    short detectorId() const { return mSector / 10; }
    short sector() const { return mSector % 10; }
    
    StThreeVectorD mXYZ;    ///< 3D position of the seed point (cm, global STAR coordinates).
    unsigned short mTrackId; ///< Track candidate identifier.
    short mSector;           ///< Packed detector/sector code: detId × 10 + sector.
    float mCov[9];           ///< 3×3 position covariance matrix (row-major, cm²).
    
    ClassDef(StFwdTrackSeedPoint, 1)
};

/// @brief Represents a reconstructed track in the STAR forward rapidity region.
///
/// Combines hits from the Forward Silicon Tracker (FST) and Forward sTGC Tracker (FTT),
/// fitted with GenFit.  Stores fit quality, kinematic state, projections to sub-detectors,
/// and optional associations to FCS calorimeter clusters.
class StFwdTrack : public StObject {

public:
    StFwdTrack(  );
    // dtor needed for releasing associations
    ~StFwdTrack(  );

    vector<StFwdTrackProjection> mProjections; ///< Track projections to each instrumented sub-detector plane.
    vector<StFwdTrackSeedPoint> mFTTPoints;    ///< Seed points from the Forward sTGC Tracker (FTT).
    vector<StFwdTrackSeedPoint> mFSTPoints;    ///< Seed points from the Forward Silicon Tracker (FST).

    StFwdTrackProjection getProjectionFor(  int detectorId, 
                            size_t index = 0 );

    StThreeVectorD momentum() const;
    StThreeVectorD momentumAt(size_t _id = 0) const;
    char charge() const;


     // Quality of the fit
    bool   didFitConverge() const;
    bool   didFitConvergeFully() const;
    short    numberOfFailedPoints() const;
    double chi2() const;
    double ndf() const;
    double pval() const;

    // error on pT upper and lower 1-sigma values
    // add access to cov matrix

    // Number of fit points used by GenFit
    short   numberOfFitPoints() const;
    
    // Number of points used in the track seed step
    short   numberOfSeedPoints() const;
    UShort_t idTruth() const { return mIdTruth; }
    UShort_t qaTruth() const { return mQATruth; }
    StThreeVectorD dca() const { return StThreeVectorD( mDCA[0], mDCA[1], mDCA[2] ); }
    UChar_t vertexIndex() const {
        // extract bits 7…2:
        return (mVtxIndex >> 2) & 0x3F;
    }
    UChar_t trackType() const {
        // extract bits 1…0:
        return mVtxIndex & 0x03;
    }
    UChar_t vertexIndexRaw() const { return mVtxIndex; }
    UShort_t globalTrackIndex() const { return mGlobalTrackIndex; }

    bool isGlobalTrack() const { return (trackType() == StFwdTrack::kGlobal); }
    bool isBeamLineConstrainedTrack() const { return (trackType() == StFwdTrack::kBeamlineConstrained); }
    bool isPrimaryTrack() const { return (trackType() == StFwdTrack::kPrimaryVertexConstrained); }
    bool isFwdVertexConstrainedTrack() const { return (trackType() == StFwdTrack::kForwardVertexConstrained); }

    void setPrimaryMomentum( StThreeVectorD mom ) { mPrimaryMomentum = mom; }
    void setDidFitConverge( bool lDidFitConverge ) { mDidFitConverge = lDidFitConverge; }
    void setDidFitConvergeFully( bool lDidFitConvergeFully ) { mDidFitConvergeFully = lDidFitConvergeFully;}
    void setNumberOfFailedPoints( short lNumberOfFailedPoints ) { mNumberOfFailedPoints = lNumberOfFailedPoints;}
    void setNumberOfSeedPoints( short lNumberOfSeedPoints ) { mNumberOfSeedPoints = lNumberOfSeedPoints;}
    void setNumberOfFitPoints( short lNumberOfFitPoints ) { mNumberOfFitPoints = lNumberOfFitPoints;}
    void setChi2( float lChi2 ) { mChi2 = lChi2;}
    void setNDF( float lNDF ) { mNDF = lNDF;}
    void setPval( float lPval ) { mPval = lPval;}
    void setCharge( short  lCharge ) { mCharge = lCharge;}
    void setMc( UShort_t idt, UShort_t qual ) { mIdTruth = idt; mQATruth = qual; }
    void setDCA( StThreeVectorD dca ) { mDCA[0] = dca.x(); mDCA[1] = dca.y(); mDCA[2] = dca.z(); }
    void setDCA( float dcaX, float dcaY, float dcaZ ) { mDCA[0] = dcaX; mDCA[1] = dcaY; mDCA[2] = dcaZ; }
    void setVtxIndex( UChar_t vtxIndex ) { mVtxIndex = pack6and2( vtxIndex, trackType() ); }
    void setTrackType( UChar_t trackType ) { mVtxIndex = pack6and2( vertexIndex(), trackType ); }
    void setVtxIndexAndTrackType( UChar_t vtxIndex, UChar_t trackType ) { mVtxIndex = pack6and2( vtxIndex, trackType ); }
    void setGlobalTrackIndex( UShort_t index ) { mGlobalTrackIndex = index; }

    // ECAL clusters
    StPtrVecFcsCluster& ecalClusters();
    const StPtrVecFcsCluster& ecalClusters() const;
    void addEcalCluster(StFcsCluster* p);
    void sortEcalClusterByET();
    // HCAL clusters
    StPtrVecFcsCluster& hcalClusters();
    const StPtrVecFcsCluster& hcalClusters() const;
    void addHcalCluster(StFcsCluster* p);
    void sortHcalClusterByET();

    enum StFwdTrackType { kGlobal=0, kBeamlineConstrained=1, kPrimaryVertexConstrained=2, kForwardVertexConstrained=3 };

    static unsigned char inline pack6and2(unsigned int A, unsigned int B) {
        // mask to ensure they fit:
        A &= 0x3F;       // 0x3F = 0b00111111
        B &= 0x03;       // 0x03 = 0b00000011

        // put A in the **high** 6 bits, B in the **low** 2 bits
        return static_cast<unsigned char>((A << 2) | B);
    }

protected:

    // Track quality and convergence
    bool mDidFitConverge;        ///< True if the GenFit track fit converged.
    bool mDidFitConvergeFully;   ///< True if the fit converged in both forward and backward directions.
    short mNumberOfFailedPoints; ///< Number of track points that failed the fit update step.
    short mNumberOfSeedPoints;   ///< Number of hits used in the track seeding step.
    short mNumberOfFitPoints;    ///< Number of hits used in the final fit (seed hits + vertex constraint).
    float mChi2;                 ///< Chi-squared of the track fit.
    float mNDF;                  ///< Number of degrees of freedom of the track fit.
    float mPval;                 ///< P-value of the track fit.
    short mCharge;               ///< Electric charge of the track (-1, 0, or +1).
    StThreeVectorD mPrimaryMomentum; ///< Momentum vector at the primary vertex (GeV/c).
    StPtrVecFcsCluster mEcalClusters; ///< Associated FCS electromagnetic calorimeter clusters.
    StPtrVecFcsCluster mHcalClusters; ///< Associated FCS hadronic calorimeter clusters.
    
    UShort_t mIdTruth;  ///< MC truth track ID for embedding studies.
    UShort_t mQATruth;  ///< MC truth quality (percentage of hits from the corresponding MC track).

    float mDCA[3];      ///< Distance of closest approach to the primary vertex (x, y, z in cm).
    /// @brief Packed byte: bits 7–2 = vertex index (6 bits), bits 1–0 = track type (2 bits).
    UChar_t mVtxIndex;
    UShort_t mGlobalTrackIndex; ///< Index of the corresponding global track in the global track collection.
    
    ClassDef(StFwdTrack,4)
};

#endif

