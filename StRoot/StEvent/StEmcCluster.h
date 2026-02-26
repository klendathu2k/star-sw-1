/*!
 * \class StEmcCluster 
 * \author Akio Ogawa, Jan 2000
 */
/***************************************************************************
 *
 * $Id: StEmcCluster.h,v 2.11 2016/02/25 17:10:19 ullrich Exp $
 *
 * Author: Akio Ogawa, Jan 2000
 ***************************************************************************
 *
 * Description: Base class for electromagnetic calorimeter cluster
 *
 ***************************************************************************
 *
 * $Log: StEmcCluster.h,v $
 * Revision 2.11  2016/02/25 17:10:19  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.10  2012/09/16 21:33:33  fisyak
 * Make one line print out
 *
 * Revision 2.9  2012/05/07 14:42:57  fisyak
 * Add handilings for Track to Fast Detectors Matching
 *
 * Revision 2.8  2004/07/15 16:36:24  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.7  2003/09/02 17:58:05  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.6  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.5  2001/04/05 04:00:34  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.4  2001/03/24 03:34:44  perev
 * clone() -> clone() const
 *
 * Revision 2.3  2000/07/31 22:12:23  akio
 * eliminate print() for L3(?)
 *
 * Revision 2.2  2000/03/23 22:24:06  akio
 * Initial version of Emc Point, and Inclusion of track pointers
 *
 * Revision 2.1  2000/02/23 17:55:43  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StEmcCluster_hh
#define StEmcCluster_hh

/// @file StEmcCluster.h
/// @brief Reconstructed cluster in the STAR Electromagnetic Calorimeter (EMC).

#include <Stiostream.h>
#include "StHit.h"
#include "StContainers.h"

/// @brief Reconstructed cluster of adjacent hits in one STAR EMC sub-detector.
class StEmcCluster : public StHit {
public:
    StEmcCluster();
    ~StEmcCluster();
    // StEmcCluster(const StEmcCluster&);            use default
    // StEmcCluster& operator=(const StEmcCluster&); use default
    
    /// @brief Returns the pseudorapidity (η) of the cluster centroid.
    float eta() const;
    /// @brief Returns the azimuthal angle (φ) of the cluster centroid [rad].
    float phi() const;
    /// @brief Returns the RMS spread of the cluster in η.
    float sigmaEta() const;
    /// @brief Returns the RMS spread of the cluster in φ [rad].
    float sigmaPhi() const;
    /// @brief Returns the total energy deposited in the cluster [GeV].
    float energy() const;
    /// @brief Returns the number of raw hits belonging to this cluster.
    int   nHits() const;
    /// @brief Returns the number of neighboring clusters.
    int   nNeighbors() const;
    /// @brief Returns the number of tracks associated with this cluster.
    int   nTracks() const;
    
    /// @brief Returns the list of raw hits belonging to this cluster.
    StPtrVecEmcRawHit&        hit();
    const StPtrVecEmcRawHit&  hit() const;
    /// @brief Returns the list of neighboring clusters.
    StPtrVecEmcCluster&       neighbor();
    const StPtrVecEmcCluster& neighbor() const;
    /// @brief Returns the list of tracks associated with this cluster.
    StPtrVecTrack&            track();
    const StPtrVecTrack&      track() const;
    
    /// @brief Sets the pseudorapidity (η) centroid of the cluster.
    void setEta(float);
    /// @brief Sets the azimuthal angle (φ) centroid of the cluster [rad].
    void setPhi(float);
    /// @brief Sets the RMS spread of the cluster in η.
    void setSigmaEta(float);
    /// @brief Sets the RMS spread of the cluster in φ [rad].
    void setSigmaPhi(float);
    /// @brief Sets the total energy of the cluster [GeV].
    void setEnergy(float);
    
    /// @brief Returns the detector identifier for this cluster.
    StDetectorId   detector() const;
    
    /// @brief Adds a raw hit to this cluster.
    void addHit(StEmcRawHit*);
    /// @brief Adds a neighboring cluster.
    void addNeighbor(StEmcCluster*);
    /// @brief Adds a track associated with this cluster.
    void addTrack(StTrack*);
    /// @brief Prints cluster information to the output stream (ROOT interface).
    void Print(Option_t *option = "") const;
    
private:
    Float_t mEta;                  ///< Pseudorapidity (η) centroid of the cluster.
    Float_t mPhi;                  ///< Azimuthal angle (φ) centroid of the cluster [rad].
    Float_t mSigmaEta;             ///< RMS spread of the cluster in η.
    Float_t mSigmaPhi;             ///< RMS spread of the cluster in φ [rad].
    Float_t mEnergy;               ///< Total energy deposited in the cluster [GeV].
    StPtrVecEmcRawHit  mHits;      ///< Raw hits belonging to this cluster.
    StPtrVecEmcCluster mNeighbors; ///< Adjacent clusters sharing a boundary.
    StPtrVecTrack      mTracks;    ///< Tracks associated with this cluster.
    
    ClassDef(StEmcCluster,2)
};

inline StDetectorId StEmcCluster::detector() const {return static_cast<StDetectorId>(StHit::bits(0, 4));}

ostream& operator<<(ostream&, const StEmcCluster&); // Printing operator
#endif






