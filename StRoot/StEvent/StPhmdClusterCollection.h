/*!
 * \class StPhmdClusterCollection
 * \author  Subhasis Chattopadhaya
 */
/********************************************************************
 *
 * $Id: StPhmdClusterCollection.h,v 2.1 2002/12/20 22:33:00 ullrich Exp $
 *
 * Author: Subhasis Chattopadhyay, Dec 2002
 ********************************************************************
 *
 * Description: Base class for PMD cluster collection
 *
 ********************************************************************
 *
 * $Log: StPhmdClusterCollection.h,v $
 * Revision 2.1  2002/12/20 22:33:00  ullrich
 * Initial Revision.
 *
 ********************************************************************/
#ifndef STAR_StPhmdClusterCollection
#define STAR_StPhmdClusterCollection

/// @file StPhmdClusterCollection.h
/// @brief Collection of reconstructed clusters for one PMD detector plane.

#include "StContainers.h"
#include "StObject.h"
#include "StPhmdDetector.h"
#include "StPhmdCluster.h"


/// @brief Container for all reconstructed PMD clusters associated with one detector plane.
class StPhmdClusterCollection : public StObject {
public: 
    StPhmdClusterCollection();  
    ~StPhmdClusterCollection();

    /// @brief Returns the number of clusters in the collection.
    int                          numberOfclusters() const; 
    /// @brief Returns a reference to the vector of clusters.
    StSPtrVecPhmdCluster&        clusters();
    /// @brief Returns a const reference to the vector of clusters.
    const StSPtrVecPhmdCluster&  clusters() const;
    
    /// @brief Adds a cluster to the collection.
    void   addCluster(StPhmdCluster* );
    /// @brief Removes a specific cluster from the collection.
    void   deleteCluster(StPhmdCluster* );
    /// @brief Removes all clusters from the collection.
    void   deleteClusters();
    /// @brief Returns the cluster-finder algorithm identifier.
    int    clusterFinderId() const;
    /// @brief Returns the version number of the cluster-finder parameter set.
    int    clusterFinderParamVersion() const;
    
    /// @brief Sets the cluster-finder algorithm identifier.
    void   setClusterFinderId(int);
    /// @brief Sets the version number of the cluster-finder parameter set.
    void   setClusterFinderParamVersion(int);

private:
    StDetectorId         mDetector;                  ///< Identifier of the associated PMD detector plane.
    StSPtrVecPhmdCluster mClusters;                  ///< Vector of clusters owned by this collection.
    
    Int_t        mClusterFinderId;                   ///< Cluster-finder algorithm identifier.
    Int_t        mClusterFinderParamVersion;         ///< Version of the cluster-finder parameter set.
    
    ClassDef(StPhmdClusterCollection,1)
};

#endif




