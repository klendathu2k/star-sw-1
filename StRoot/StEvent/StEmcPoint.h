/*!
 * \class StEmcPoint 
 * \author Akio Ogawa, Mar 2000
 */
/***************************************************************************
 *
 * $Id: StEmcPoint.h,v 2.11 2016/02/25 17:10:19 ullrich Exp $
 *
 * Author: Akio Ogawa, Mar 2000
 ***************************************************************************
 *
 * Description: Base class for electromagnetic calorimeter Point
 *
 ***************************************************************************
 *
 * $Log: StEmcPoint.h,v $
 * Revision 2.11  2016/02/25 17:10:19  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.10  2012/10/23 20:18:33  fisyak
 * Add/modify print outs
 *
 * Revision 2.9  2005/08/31 18:25:41  perev
 * Class version increased by 1
 *
 * Revision 2.8  2005/07/19 21:32:50  perev
 * Remove clash with IdTruth
 *
 * Revision 2.7  2004/07/20 17:07:49  perev
 * Pavlinov corrs for TBrowser
 *
 * Revision 2.6  2004/07/15 16:36:24  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.5  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.4  2001/04/05 04:00:35  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.3  2001/03/24 03:34:45  perev
 * clone() -> clone() const
 *
 * Revision 2.2  2000/05/22 19:21:54  akio
 * Bug fix, add delta into EMcPoint, wider bits for Eta in RawHit
 *
 * Revision 2.1  2000/03/23 22:24:07  akio
 * Initial version of Emc Point, and Inclusion of track pointers
 *
 *
 **************************************************************************/
#ifndef StEmcPoint_hh
#define StEmcPoint_hh

/// @file StEmcPoint.h
/// @brief Reconstructed energy-deposition point in the STAR EMC, combining clusters from multiple sub-detectors.

#include "StHit.h"
#include "StEnumerations.h"

/// @brief Reconstructed energy-deposition point in the STAR EMC combining clusters from up to four sub-detectors.
class StEmcPoint : public StHit {
public:
    StEmcPoint();
    /// @brief Constructs an EMC point with position, position error, size, hardware id, energy, chi-square, delta-eta, delta-phi, and quality.
    StEmcPoint(const StThreeVectorF&,
	       const StThreeVectorF&,
	       const StThreeVectorF&,
               unsigned int, float,
	       float, float,
	       unsigned char = 0);
    ~StEmcPoint();
    
    /// @brief Returns the total energy of this point [GeV].
    float   energy() const;
    /// @brief Returns the chi-square of the point reconstruction fit.
    float   chiSquare() const;
    /// @brief Sets the total energy of this point [GeV].
    void setEnergy(const float);
    /// @brief Sets the chi-square of the point reconstruction fit.
    void setChiSquare(const float);
    /// @brief Returns the spatial size (extent) of the point as a 3-vector.
    StThreeVectorF size() const;
    /// @brief Sets the spatial size of the point.
    void setSize(const StThreeVectorF&);
    
    /// @brief Returns the energy deposited in the specified sub-detector [GeV].
    float   energyInDetector(const StDetectorId) const;
    /// @brief Returns the transverse size of the point at the specified sub-detector.
    float   sizeAtDetector(const StDetectorId) const;
    /// @brief Sets the energy deposited in the specified sub-detector [GeV].
    void setEnergyInDetector(const StDetectorId, const float);
    /// @brief Sets the transverse size of the point at the specified sub-detector.
    void setSizeAtDetector(const StDetectorId, const float);
    
    /// @brief Returns the residual of the point position in η relative to the matched track.
    float deltaEta() const;
    /// @brief Returns the residual of the point position in φ relative to the matched track [rad].
    float deltaPhi() const;
    /// @brief Returns the residual in the SMD u-strip direction relative to the matched track.
    float deltaU() const;
    /// @brief Returns the residual in the SMD v-strip direction relative to the matched track.
    float deltaV() const;
    /// @brief Sets the residual in η between the point and the matched track.
    void  setDeltaEta(const float);
    /// @brief Sets the residual in φ between the point and the matched track [rad].
    void  setDeltaPhi(const float);
    /// @brief Sets the residual in the SMD u-strip direction.
    void  setDeltaU(const float);
    /// @brief Sets the residual in the SMD v-strip direction.
    void  setDeltaV(const float);
    
    /// @brief Returns the detector identifier for this point.
    StDetectorId detector() const;
    
    /// @brief Returns the clusters in the specified sub-detector associated with this point.
    StPtrVecEmcCluster&       cluster(const StDetectorId);
    const StPtrVecEmcCluster& cluster(const StDetectorId) const;
    /// @brief Returns the clusters in the sub-detector at the given array index.
    StPtrVecEmcCluster&       cluster(Int_t);
    const StPtrVecEmcCluster& cluster(Int_t) const;

    /// @brief Associates a cluster from the specified sub-detector with this point.
    void addCluster(const StDetectorId, const StEmcCluster*);
    
    /// @brief Returns the list of neighboring EMC points.
    StPtrVecEmcPoint&       neighbor();
    const StPtrVecEmcPoint& neighbor() const;

    /// @brief Adds a neighboring EMC point.
    void addNeighbor(const StEmcPoint*);
    
    /// @brief Returns the number of tracks associated with this point.
    int                  nTracks() const;  // *MENU*
    /// @brief Returns the list of tracks associated with this point.
    StPtrVecTrack&       track();
    const StPtrVecTrack& track() const;
    
    /// @brief Associates a track with this point.
    void addTrack(StTrack*);
  // 11-nov-03 by PAI

    /// @brief Prints point information to stdout (ROOT browser menu entry).
    void print();      // *MENU*    
    /// @brief Prints point information to the output stream (ROOT interface).
    void Print(Option_t *option = "") const;

    /// @brief Sets the reconstruction quality flag for this point.
    void setQuality(int qua) {myQuality = qua ;}
    /// @brief Returns the reconstruction quality flag for this point.
    int  quality() const     {return myQuality;}

protected:
    Float_t            mEnergy;              ///< Total energy of the point [GeV].
    Float_t            mChiSquare;           ///< Chi-square of the point reconstruction fit.
    StThreeVectorF     mSize;                ///< Spatial extent of the point.
    Float_t            mEnergyInDetector[4]; ///< Energy deposited per sub-detector [GeV] (indexed by internal detector slot).
    Float_t            mSizeAtDetector[4];   ///< Transverse size at each sub-detector (indexed by internal detector slot).
    Float_t            mDelta[2];            ///< Track-point residuals: [0]=Δη, [1]=Δφ.
    StPtrVecEmcCluster mCluster[4];          ///< Clusters per sub-detector contributing to this point.
    StPtrVecEmcPoint   mNeighbors;           ///< Adjacent EMC points.
    StPtrVecTrack      mTracks;              ///< Tracks associated with this point.
    int                myQuality;            ///< Reconstruction quality flag.
    int getDetId(const StDetectorId) const;
    int getDetId(Int_t /* id */) const;
    ClassDef(StEmcPoint,2)
};

inline StDetectorId StEmcPoint::detector() const {return static_cast<StDetectorId>(StHit::bits(0, 4));}

ostream& operator<<(ostream&, const StEmcPoint&); // Printing operator
#endif


