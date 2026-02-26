// $Id: StFcsClusterMaker.h,v 1.1 2021/03/30 13:40:02 akio Exp $
// $Log: StFcsClusterMaker.h,v $
// Revision 1.1  2021/03/30 13:40:02  akio
// FCS code after peer review and moved from $CVSROOT/offline/upgrades/akio
//
// Revision 1.13  2021/02/25 21:52:57  akio
// Int_t -> int
//
// Revision 1.12  2021/02/25 19:24:15  akio
// Modified for STAR code review (Hongwei)
//
// Revision 1.11  2020/12/17 21:18:01  akio
// Separate RATIO2SPLIT for ecal/hcal
//
// Revision 1.10  2020/09/03 19:42:24  akio
// moving sum & fit to StFcsWaveformFitMaker
//
// Revision 1.9  2019/11/22 17:26:17  akio
// fix typo
//
// Revision 1.8  2019/11/22 15:51:16  akio
// adding categorization
//
// Revision 1.7  2019/07/03 16:13:29  akio
// separate neighbor distance for ecal and hcal
//
// Revision 1.6  2019/06/27 16:12:42  akio
// Using getLocalXYinCell for cell/cluster distance measure and in moment analysis
//
// Revision 1.5  2019/06/26 18:00:34  akio
// added some adjustable parameters and way to select how to get energy
//
// Revision 1.4  2019/06/25 16:38:17  akio
// Added TOWER_E_RATIO2SPLIT, neighbor clusters
//
// Revision 1.3  2019/06/21 16:32:42  akio
// Added neighbor cluster and factor threshold for cluster splitting at valley
//
// Revision 1.2  2019/06/07 18:17:27  akio
// added summing adc
//
// Revision 1.1  2018/11/14 16:50:11  akio
// FCS codes in offline/upgrade/akio
//

#ifndef STROOT_STFCSCLUSTERMAKER_STFCSCLUSTERMAKER_H_
#define STROOT_STFCSCLUSTERMAKER_STFCSCLUSTERMAKER_H_

#include <map>
#include <vector>

#include "StMaker.h"

class StFcsCollection;
class StFcsHit;
class StFcsCluster;
class StFcsPoint;
class StFcsDb;
class StMuDst;

/**
 * \file  StFcsClusterMaker.h
 * \brief Declaration of StFcsClusterMaker, the maker that clusters FCS
 *        (Forward Calorimeter System) hits into StFcsCluster objects stored
 *        in StFcsCollection within StEvent.
 */

/**
 * \class StFcsClusterMaker
 * \brief Maker that clusters FCS calorimeter hits into StFcsCollection.
 *
 * \details StFcsClusterMaker groups individual StFcsHit tower-energy deposits
 * in the Forward Calorimeter System (ECal and HCal) into spatial clusters.
 * Clusters are stored as StFcsCluster objects inside StFcsCollection in StEvent.
 * Moment analysis is performed to characterise cluster shape, and clusters may
 * be split at energy valleys when two overlapping showers are detected.
 *
 * \par Inputs
 * - StFcsHit objects in StFcsCollection (filled by the upstream waveform-fit maker).
 * - StFcsDb geometry/calibration database.
 *
 * \par Output
 * - StFcsCluster objects appended to StFcsCollection in StEvent.
 *
 * \note Clustering parameters (seed threshold, neighbor distance, split ratio)
 *       can be tuned via the setter methods before Init().
 *
 * \sa StFcsHit, StFcsCluster, StFcsCollection, StFcsDb
 */
class StFcsClusterMaker : public StMaker {
public:
    
    StFcsClusterMaker(const char* name = "StFcsClusterMaker"); ///< Constructor. \param name Maker name passed to StMaker.
    ~StFcsClusterMaker();
    int InitRun(int runNumber); ///< Loads geometry and calibration from StFcsDb. \param runNumber Run number. \return kStOK on success.
    int Make();                 ///< Clusters FCS hits and fills StFcsCollection in StEvent. \return kStOK on success.
    void Clear(Option_t* option = ""); ///< Clears transient per-event state. \param option Passed to StMaker::Clear().
    
    void setDebug(int v) {SetDebug(v);}  ///< Set debug verbosity level. \param v Debug level (0 = off).

    /// Set neighbor-search distance for ECal and HCal. \param e ECal distance in cell units. \param h HCal distance in cell units.
    void setNeighborDistance(float e, float h){mNeighborDistance_Ecal=e; mNeighborDistance_Hcal=h;}
    /// Set distance advantage factor favouring higher-energy clusters. \param e ECal factor. \param h HCal factor.
    void setDistanceAdvantage(float e, float h){mDistanceAdvantage_Ecal=e; mDistanceAdvantage_Hcal=h;}
    /// Set tower energy seed threshold for new cluster formation. \param e ECal threshold [GeV]. \param h HCal threshold [GeV].
    void setTowerEThreSeed(float e, float h){mTowerEThreSeed_Ecal=e; mTowerEThreSeed_Hcal=h;}
    /// Set minimum tower energy to be included in a cluster. \param e ECal threshold [GeV]. \param h HCal threshold [GeV].
    void setTowerEThreshold(float e, float h){mTowerEThreshold_Ecal=e; mTowerEThreshold_Hcal=h;}
    /// Set tower energy threshold used in moment analysis. \param e ECal threshold [GeV]. \param h HCal threshold [GeV].
    void setTowerEThreMoment(float e, float h){mTowerEThreMoment_Ecal=e; mTowerEThreMoment_Hcal=h;}
    /// Set energy ratio threshold to decide cluster splitting vs merging. \param e ECal ratio. \param h HCal ratio.
    void setTowerERatio2Split(float e, float h){mTowerERatio2Split_Ecal=e; mTowerERatio2Split_Hcal=h;}
    /// Set hit-sorting mode. \param v 1 = sort by ID (cosmic tracking), 0 = sort by energy (default).
    void sortById(int v=1){mSortById=v;}

 private:
    int makeCluster(int det);

    float isNeighbor(StFcsHit* hit,  StFcsCluster* clu);   //! checks if a hit is touching a cluster
    float distance(StFcsHit* hit1, StFcsHit* hit2);        //! distance between 2 hits [cell unit]
    float distance(StFcsHit* hit, StFcsCluster* clu);      //! distance between a hit to cluster center [cell unit]
    void  updateCluster(StFcsCluster* clu);                //! update cluster infos after hits added 
    int   clusterMomentAnalysis(StFcsCluster* clu, float ecut);      //! cluster moment analysis with a threshold
    float getSigma(StFcsCluster* clu, double thetam, float ecut);    //! calculate sigma 
    void  categorization(StFcsCluster* clu);                         //! categorize cluster based on moment analysis
	
    StFcsDb* mDb=0;                       //! pointer to StFcsDb
    StFcsCollection* mFcsCollection=0;    //! pointer to StFcsCollection in StEvent

    float mNeighborDistance      = 1.01;  //! Distance to make it neignbor
    float mNeighborDistance_Ecal = 1.01;  //! 1.01 for 4 towers around
    float mNeighborDistance_Hcal = 2.01;  //! 1.42 for 8 surroundings

    float mDistanceAdvantage      = 1.2;  //! if similar distanced cluster found, higher E cluster takes it
    float mDistanceAdvantage_Ecal = 1.2;  //! for Ecal
    float mDistanceAdvantage_Hcal = 1.2;  //! for Hcal

    float mTowerEThreSeed      = 1.0;     //! Tower E threshold for cluster seed [GeV]
    float mTowerEThreSeed_Ecal = 1.0;     //! for Ecal
    float mTowerEThreSeed_Hcal = 1.0;     //! for Hcal

    float mTowerEThreshold      = 0.01;   //! Tower E threshold for clustering [GeV] 
    float mTowerEThreshold_Ecal = 0.01;   //! for Ecal
    float mTowerEThreshold_Hcal = 0.01;   //! for Hcal

    float mTowerEThreMoment      = 0.1;   //! Tower E threshold for moment analysis [GeV]
    float mTowerEThreMoment_Ecal = 0.1;   //! for Ecal
    float mTowerEThreMoment_Hcal = 0.1;   //! for Hcal

    float mTowerERatio2Split      = 1.5;  //! Neighbor E * threshold > tower E to merge cluster, otherwise split
    float mTowerERatio2Split_Ecal = 1.5;  //! for Ecal
    float mTowerERatio2Split_Hcal = 2.0;  //! for Hcal

    int mSortById=0;  //! set this for cosmic tracking (default is sort by energy)

    virtual const Char_t *GetCVS() const {static const Char_t cvs[]="Tag " __DATE__ " " __TIME__ ; return cvs;}
    ClassDef(StFcsClusterMaker, 1)
};
#endif  // STROOT_STFCSCLUSTERMAKER_STFCSCLUSTERMAKER_H_
