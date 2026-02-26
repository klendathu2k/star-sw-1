#ifndef STFTTCLUSTER_H
#define STFTTCLUSTER_H

/// @file StFttCluster.h
/// @brief Data structure for a strip cluster in the Forward sTGC Tracker (FTT).

#include "StObject.h"
#include <Stiostream.h>
#include "StContainers.h"  // For StPtrVecFttRawHit
#include "StEnumerations.h"

class StFttRawHit;
class StFttPoint;
class StFttDb;

/// @brief Represents a group of adjacent strips forming a cluster on one FTT plane/row.
///
/// Stores charge sum (0th moment), centroid position (1st moment), width (2nd moment),
/// and associations to contributing raw hits, neighbouring clusters, and fitted points.
class StFttCluster : public StObject {
public:
    StFttCluster();
    ~StFttCluster();
    
    int id() const; // Cluster ID
    UChar_t plane()   const; // Detector plane.
    UChar_t quadrant()   const; // detector quadrant.
    UChar_t row()   const; // detector row.
    UChar_t orientation() const;
    int nStrips() const;
    int nPoints() const;
    int nRawHits() const; // == nStrips?? need both?
    int nNeighbors() const;
    float sumAdc() const;
    float x() const;  // Mean x ("center of gravity") in local grid coordinate (1st moment).
    float sigma() const; // Maximum 2nd moment (along major axis).
    UShort_t idTruth() const { return mIdTruth; } // Get the truth ID
    UShort_t qaTruth() const { return mQaTruth; } // Get the truth quality

    void setId(int cluid);
    void setPlane(UChar_t plane);
    void setQuadrant(UChar_t quad);
    void setRow(UChar_t row);
    void setOrientation( UChar_t );
    void setNStrips(int numStrips);
    void setSumAdc(int theSumAdc);
    void setX(float x0);
    void setSigma(float sigma);
    void setIdTruth(UShort_t id) { mIdTruth = id; }
    void setQaTruth(UShort_t qa) { mQaTruth = qa; }

    StPtrVecFttRawHit& rawHits();
    const StPtrVecFttRawHit& rawHits() const;
    void addRawHit(StFttRawHit* p);
    void addNeighbor(StFttCluster* neighbor);
    StPtrVecFttCluster& neighbor();
    const StPtrVecFttCluster& neighbor() const;    
    StPtrVecFttPoint& points();
    const StPtrVecFttPoint& points() const;
    void addPoint(StFttPoint* p);
    // void print(Option_t *option="") const;

private:
    Int_t mId=-1;              ///< Event-unique cluster identifier.
    UChar_t mPlane;            ///< FTT detector plane index.
    UChar_t mQuadrant;         ///< FTT detector quadrant index.
    UChar_t mRow;              ///< Strip row within the quadrant.
    UChar_t mOrientation = kFttUnknownOrientation; ///< Strip orientation (horizontal/vertical/unknown).
    Int_t mNStrips=0;          ///< Number of strips contributing to the cluster.
    Float_t mSumAdc=0.0;       ///< Total integrated ADC charge (0th moment).
    Float_t mX=0.0;            ///< Charge-weighted centroid in local grid coordinates (1st moment).
    Float_t mSigma=0.0;        ///< RMS width along the major axis (2nd moment).
    StPtrVecFttRawHit mRawHits;     ///< Raw hits (strips) contributing to this cluster.
    StPtrVecFttCluster mNeighbors;  ///< Adjacent clusters sharing a boundary strip.
    StPtrVecFttPoint mPoints;       ///< Reconstructed space points (track intercepts) associated with this cluster.
    UShort_t mIdTruth=0; ///< MC truth track ID for embedding studies.
    UShort_t mQaTruth=0; ///< MC truth quality (fraction of charge from the truth track, %).

    ClassDef(StFttCluster, 4)
};

std::ostream& operator << ( std::ostream&, const StFttCluster& clu ); // Printing operator


inline int StFttCluster::id() const { return mId; } // Cluster ID
inline UChar_t StFttCluster::plane() const { return mPlane; }
inline UChar_t StFttCluster::quadrant() const { return mQuadrant; }
inline UChar_t StFttCluster::row() const { return mRow; }
inline UChar_t StFttCluster::orientation() const { return mOrientation; }
inline int StFttCluster::nStrips() const { return mNStrips; }
inline int StFttCluster::nRawHits() const { return mRawHits.size(); }
inline int StFttCluster::nNeighbors() const { return mNeighbors.size(); }
inline int StFttCluster::nPoints() const { return mPoints.size(); }
inline float StFttCluster::sumAdc() const { return mSumAdc; }
inline float StFttCluster::x() const { return mX; } // Mean x ("center of gravity") in local grid coordinate (1st moment).
inline float StFttCluster::sigma() const { return mSigma; } // 2nd moment

inline void StFttCluster::setPlane(UChar_t plane) { mPlane = plane; }
inline void StFttCluster::setQuadrant(UChar_t quadrant) { mQuadrant = quadrant; }
inline void StFttCluster::setRow(UChar_t row) { mRow = row; }
inline void StFttCluster::setOrientation( UChar_t so ) { mOrientation = so; }
inline void StFttCluster::setNStrips(int numStrips) { mNStrips = numStrips; }
inline void StFttCluster::setSumAdc(int theSumAdc) { mSumAdc = theSumAdc; }
inline void StFttCluster::setX(float x0) { mX = x0; }
inline void StFttCluster::setSigma(float sigma) { mSigma = sigma; }

inline void StFttCluster::setId(int cluid) { mId = cluid; }

inline StPtrVecFttRawHit& StFttCluster::rawHits() { return mRawHits; }
inline const StPtrVecFttRawHit& StFttCluster::rawHits() const { return mRawHits; }
inline StPtrVecFttCluster& StFttCluster::neighbor() { return mNeighbors; }
inline const StPtrVecFttCluster& StFttCluster::neighbor() const { return mNeighbors; }
inline StPtrVecFttPoint& StFttCluster::points() { return mPoints; }
inline const StPtrVecFttPoint& StFttCluster::points() const { return mPoints; }

#endif  // STFTTCLUSTER_H
