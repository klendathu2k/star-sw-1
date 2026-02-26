/***************************************************************************
 *
 * $Id: StFmsCollection.h,v 2.9 2019/06/25 15:56:33 ullrich Exp $
 *
 * Author: Jingguo Ma, Dec 2009
 ***************************************************************************
 *
 * Description:
 * Collection of all hits (towers), clusters and points (photons) in the FMS.
 * This collection owns all these objects, and is itself owned by StEvent.
 * It is therefore vital to *not* delete any of the objects stored in this
 * container yourself - the collection will handle freeing memory.
 * Similarly, any object added to the collection via an add() method must be
 * allocated with new, and not be owned anywhere else.
 *
 ***************************************************************************
 *
 * $Log: StFmsCollection.h,v $
 * Revision 2.9  2019/06/25 15:56:33  ullrich
 * FMS shower shape scaling in StFmsCollection (Akio)
 *
 * Revision 2.8  2015/11/05 19:00:39  ullrich
 * Added 4 new inline functions.
 *
 * Revision 2.7  2015/10/21 14:53:59  ullrich
 * Added new member and methods.
 *
 * Revision 2.6  2015/09/14 16:59:53  ullrich
 * Added StFmsPointPair collection.
 *
 * Revision 2.5  2015/09/01 21:01:47  ullrich
 * Minor changes to format of print statments and \nchange to naming of data member.
 *
 * Revision 2.4  2015/09/01 18:29:01  ullrich
 * Changes due to adding StFpsSlat and interconnection between slats and points.
 *
 * Revision 2.3  2015/08/26 16:51:59  ullrich
 * Added print out fct and operator.
 *
 * Revision 2.2  2015/02/14 18:57:25  ullrich
 * Big upgrade after adding StFmPoint and StFmsCluster.
 *
 * Revision 2.1  2010/01/08 22:42:31  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StFmsCollection_hh
#define StFmsCollection_hh

/// @file StFmsCollection.h
/// @brief Event-level collection of all FMS hits, clusters, points, and point pairs.

#include "Stiostream.h"
#include "StObject.h"
#include "StContainers.h"

class StFmsHit;
class StFmsCluster;
class StFmsPoint;
class StFmsPointPair;
class StFpsSlat;

/// @brief Container owning all FMS hits, clusters, photon points, and point pairs for one event.
class StFmsCollection : public StObject {
public:
    /// @brief Default constructor.
    StFmsCollection();
    /// @brief Destructor; frees all owned objects.
    ~StFmsCollection();
    
    void addHit(StFmsHit*);           ///< Add a hit to the collection.
    void addCluster(StFmsCluster*);   ///< Add a cluster to the collection.
    void addPoint(StFmsPoint*);       ///< Add a photon point to the collection.
    
    unsigned int numberOfHits() const;   ///< Return the total number of hits.
    unsigned int numberOfClusters() const; ///< Return the total number of clusters.
    unsigned int numberOfPoints() const;   ///< Return the total number of photon points.
    
    StSPtrVecFmsHit& hits();   ///< Return the list of FMS hits.
    const StSPtrVecFmsHit& hits() const;
    
    StSPtrVecFmsCluster& clusters();   ///< Return the list of FMS clusters.
    const StSPtrVecFmsCluster& clusters() const;
    
    StSPtrVecFmsPoint& points();    ///< Return the list of photon points.
    const StSPtrVecFmsPoint& points() const;
   
    /// @brief Return the packed FMS reconstruction flag word.
    int fmsReconstructionFlag()      const;
    /// @brief Return 1 if small-cell clusters are merged into large-cell clusters.
    int isMergeSmallToLarge()        const;
    /// @brief Return 1 if a global cluster refit was performed.
    int isGlobalRefit()              const;
    /// @brief Return 1 if a 1-photon fit retry was attempted for poor 2-photon fits.
    int isTry1PhotonFit()            const;
    /// @brief Return 1 if the new cluster categorization algorithm was used.
    int isNewClusterCategorization() const;
    /// @brief Return 1 if shower shape scaling was applied.
    int isScaleShowerShape()         const;
    /// @brief Return the shower shape scale factor for the large-cell FMS.
    float scaleShowerShapeLarge()    const;
    /// @brief Return the shower shape scale factor for the small-cell FMS.
    float scaleShowerShapeSmall()    const;
    /// @brief Set the packed FMS reconstruction flag word.
    void setFmsReconstructionFlag(int v);
    void setMergeSmallToLarge(int v);
    void setGlobalRefit(int v);
    void setTry1PhotonFit(int v);
    void setNewClusterCategorization(int v);
    void setScaleShowerShape(int v);
    void setScaleShowerShape(float l, float s);

    void fillFpsSlat();            ///< Populate FPS slat information from FMS hits.
    void fillFpsAssociation();     ///< Update FPS–FMS spatial association from photon points.
    StSPtrVecFpsSlat& fpsSlats();  ///< Return the array of FPS (FPD Pre-Shower) slats.
    StFpsSlat* fps(int slatid);    ///< Return the FPS slat for a given slat ID.

    /// @brief Build all photon point pairs from the current point list.
    void fillFmsPointPair();
    /// @brief Return the number of point pairs.
    unsigned int numberOfPointPairs();    
    /// @brief Return all point pairs sorted by descending E1 then E2.
    vector<StFmsPointPair*>& pointPairs();    
    /// @brief Return point pairs sorted by descending total energy.
    vector<StFmsPointPair*>& pointPairsEnergySorted();    
    /// @brief Return point pairs sorted by descending total transverse energy.
    vector<StFmsPointPair*>& pointPairsETSorted();    
    /// @brief Return point pairs sorted by proximity to the pi0 mass.
    vector<StFmsPointPair*>& pointPairsPi0MassSorted();    
    
    /// @brief Sort photon points by descending energy.
    void sortPointsByEnergy();
    /// @brief Sort photon points by descending transverse energy.
    void sortPointsByET();

    /// @brief Print collection summary.
    void print(int option=1);
    
private:
    StSPtrVecFmsHit     mHits;      ///< Owns all FMS tower hits.
    StSPtrVecFmsCluster mClusters;  ///< Owns all FMS clusters.
    StSPtrVecFmsPoint   mPoints;    ///< Owns all FMS photon points.
    StSPtrVecFpsSlat    mFpsSlats;  //!< Owns FPS slats; transient (auto-generated, not persisted).

    vector<StFmsPointPair*> mPointPairs;              //!< All point pairs sorted by descending E1 then E2.
    vector<StFmsPointPair*> mPointPairsEnergySorted;  //!< Point pairs sorted by descending total energy.
    vector<StFmsPointPair*> mPointPairsETSorted;      //!< Point pairs sorted by descending total ET.
    vector<StFmsPointPair*> mPointPairsPi0MassSorted; //!< Point pairs sorted by proximity to pi0 mass.

    Int_t mFmsReconstructionFlag;   ///< Packed reconstruction flags (see setter methods for bit layout).
                                    // 2nd LSB=(0=No global refit, 1=performe global refit)
                                    // 3rd LSB=(0=No 1photon fit retry, 1=performe 1 photon fit if 2 photon fit is bad)
    Float_t mScaleShowerShapeLarge=1.0; 
    Float_t mScaleShowerShapeSmall=1.0; 


    bool mFpsSlatFilled;            //!
    bool mFpsAssociationFilled;     //!
    bool mFmsPointPairFilled;       //!

    ClassDef(StFmsCollection, 4)
};

inline int StFmsCollection::fmsReconstructionFlag()      const {return mFmsReconstructionFlag;}
inline int StFmsCollection::isMergeSmallToLarge()        const {return  (mFmsReconstructionFlag &  0x1);}
inline int StFmsCollection::isGlobalRefit()              const {return ((mFmsReconstructionFlag &  0x2)>>1);}
inline int StFmsCollection::isTry1PhotonFit()            const {return ((mFmsReconstructionFlag &  0x4)>>2);}
inline int StFmsCollection::isNewClusterCategorization() const {return ((mFmsReconstructionFlag &  0x8)>>3);}
inline int StFmsCollection::isScaleShowerShape()         const {return ((mFmsReconstructionFlag & 0x10)>>4);}
inline float StFmsCollection::scaleShowerShapeLarge()    const {return mScaleShowerShapeLarge;}
inline float StFmsCollection::scaleShowerShapeSmall()    const {return mScaleShowerShapeSmall;}
inline void StFmsCollection::setFmsReconstructionFlag(int v)    {mFmsReconstructionFlag=v;}
inline void StFmsCollection::setMergeSmallToLarge(int v)        {mFmsReconstructionFlag=(mFmsReconstructionFlag & 0xfffffe) | (v & 0x1);   }
inline void StFmsCollection::setGlobalRefit(int v)              {mFmsReconstructionFlag=(mFmsReconstructionFlag & 0xfffffd) | (v & 0x1)<<1;}
inline void StFmsCollection::setTry1PhotonFit(int v)            {mFmsReconstructionFlag=(mFmsReconstructionFlag & 0xfffffb) | (v & 0x1)<<2;}
inline void StFmsCollection::setNewClusterCategorization(int v) {mFmsReconstructionFlag=(mFmsReconstructionFlag & 0xfffff7) | (v & 0x1)<<3;}
inline void StFmsCollection::setScaleShowerShape(int v)         {mFmsReconstructionFlag=(mFmsReconstructionFlag & 0xffffef) | (v & 0x1)<<4;}
inline void StFmsCollection::setScaleShowerShape(float l, float s) {mScaleShowerShapeLarge=l; mScaleShowerShapeSmall=s;};
#endif
