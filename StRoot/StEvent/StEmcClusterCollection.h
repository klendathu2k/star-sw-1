/*!
 * \class StEmcClusterCollection 
 * \author Akio Ogawa, Jan 2000
 */
/***************************************************************************
 *
 * $Id: StEmcClusterCollection.h,v 2.3 2002/02/22 22:56:47 jeromel Exp $
 *
 * Author: Akio Ogawa, Jan 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEmcClusterCollection.h,v $
 * Revision 2.3  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:34  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  2000/02/23 17:34:04  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StEmcClusterCollection_hh
#define StEmcClusterCollection_hh

/// @file StEmcClusterCollection.h
/// @brief Collection of EMC clusters for a single calorimeter detector.

#include "StContainers.h"
#include "StObject.h"
#include "StEnumerations.h"

/// @brief Owns the set of reconstructed EMC clusters for one sub-detector and records the cluster-finder provenance.
class StEmcClusterCollection : public StObject {
public:
    StEmcClusterCollection();
    // StEmcClusterCollection(const StEmcClusterCollection&);            use default
    // StEmcClusterCollection& operator=(const StEmcClusterCollection&); use default
    virtual ~StEmcClusterCollection();
    
    /// @brief Returns the identifier of the sub-detector these clusters belong to.
    StDetectorId detector() const;
    /// @brief Sets the sub-detector identifier.
    void setDetector(StDetectorId);
    
    /// @brief Returns the number of clusters in this collection.
    int numberOfClusters() const;
    /// @brief Returns the list of clusters in this collection.
    StSPtrVecEmcCluster&       clusters();
    const StSPtrVecEmcCluster& clusters() const;

    /// @brief Adds a cluster to this collection (takes ownership).
    void addCluster(StEmcCluster*);
    
    /// @brief Returns the identifier of the cluster-finder algorithm used.
    int  clusterFinderId() const;
    /// @brief Returns the version number of the cluster-finder parameter set.
    int  clusterFinderParamVersion() const;
    /// @brief Sets the cluster-finder algorithm identifier.
    void setClusterFinderId(int);
    /// @brief Sets the cluster-finder parameter version number.
    void setClusterFinderParamVersion(int);
    
private:
    StDetectorId        mDetector;              ///< Identifier of the sub-detector owning these clusters.
    StSPtrVecEmcCluster mClusters;              ///< Owned list of reconstructed clusters.
    
    Int_t        mClusterFinderId;              ///< Identifier of the cluster-finder algorithm.
    Int_t        mClusterFinderParamVersion;    ///< Version of the cluster-finder parameter set.
    
    ClassDef(StEmcClusterCollection,1)
};

#endif




