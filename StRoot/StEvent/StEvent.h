/**
 * \class StEvent
 * \brief Top-level transient event container for all STAR detector data.
 * \author Thomas Ullrich, Sep 1999
 *
 * \details StEvent is the central in-memory store for every piece of
 * reconstructed and raw data associated with a single STAR collision event.
 * It is created and populated by StEventMaker and its sub-makers, then made
 * available to all downstream analysis makers through the STAR framework data
 * set mechanism.
 *
 * The object aggregates detector hit collections, reconstructed tracks and
 * vertices, trigger information, and run/event metadata.  Track and vertex
 * memory is managed through StTrackNode and the primary-vertex list
 * respectively; all detector-collection pointers are owned by StEvent and
 * are deleted in the destructor.
 *
 * Typical read access:
 * \code
 *   StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
 *   if (!event) return;
 *   StTpcHitCollection* tpcHits = event->tpcHitCollection();
 *   const StSPtrVecTrackNode& nodes = event->trackNodes();
 * \endcode
 *
 * \sa StEventMaker, StTrack, StVertex, StHit, StRunInfo, StEventInfo
 */
/***************************************************************************
 *
 * $Id: StEvent.h,v 2.52 2021/01/11 20:27:40 ullrich Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEvent.h,v $
 * Revision 2.52  2021/01/11 20:27:40  ullrich
 * Updated to add FCS elements (Akio).
 *
 * Revision 2.51  2018/12/11 19:53:10  ullrich
 * Added RICHf.
 *
 * Revision 2.50  2018/07/09 16:50:31  ullrich
 * Added EToF hooks.
 *
 * Revision 2.49  2018/02/08 17:36:26  ullrich
 * Changed for new EPD classes.
 *
 * Revision 2.48  2015/05/13 17:06:13  ullrich
 * Added hooks and interfaces to Sst detector (part of HFT).
 *
 * Revision 2.47  2014/04/10 16:00:13  jeromel
 * Changes to inlcude Ist structure (Thomas OK-ed / may revisit some comments)
 *
 * Revision 2.46  2013/03/05 14:42:45  ullrich
 * Added StPxl hits and Containers.
 *
 * Revision 2.45  2012/04/16 20:22:16  ullrich
 * Changes necessary to add Fgt package.
 *
 * Revision 2.44  2012/02/21 18:47:13  perev
 * Add IsMain() related to I/O
 *
 * Revision 2.43  2012/01/24 03:01:24  perev
 * Etr detector added
 *
 * Revision 2.42  2011/10/17 00:13:49  fisyak
 * Add handles for IdTruth info
 *
 * Revision 2.41  2011/04/25 21:25:09  ullrich
 * Modifications to hold MTD data.
 *
 * Revision 2.40  2011/02/01 19:47:36  ullrich
 * Added HLT branch and hooks.
 *
 * Revision 2.39  2010/08/31 19:53:37  fisyak
 * Remove SoftwareMonitors
 *
 * Revision 2.38  2010/01/08 22:43:44  ullrich
 * Updates needed to add StFmsCollection and related classes.
 *
 * Revision 2.37  2009/11/23 22:22:25  ullrich
 * Minor cleanup performed and hooks for RPS added.
 *
 * Revision 2.36  2009/11/23 19:45:42  fisyak
 * remove reference to event_header_st
 *
 * Revision 2.35  2009/11/23 19:31:23  fisyak
 * remove reference to event_header_st
 *
 * Revision 2.33  2008/12/22 20:36:53  ullrich
 * Added hooks for new ToF (BTof)
 *
 * Revision 2.32  2006/04/25 23:21:25  ullrich
 * Modified addPrimaryVertex(). New 2nd arg: StPrimaryVertexOrder.
 *
 * Revision 2.31  2006/01/19 21:48:21  ullrich
 * Add RnD collection.
 *
 * Revision 2.30  2003/04/16 17:48:32  ullrich
 * Added StTriggerData and inherited classe(s).
 *
 * Revision 2.29  2003/01/30 18:36:31  ullrich
 * Added hooks for StTriggerIdCollection.
 *
 * Revision 2.28  2002/12/20 22:41:30  ullrich
 * Added PMD.
 *
 * Revision 2.27  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.26  2002/01/03 20:59:33  ullrich
 * Added BBC and FPD.
 *
 * Revision 2.25  2001/12/01 15:40:48  ullrich
 * Added StDetectorState access function.
 *
 * Revision 2.24  2001/11/10 23:53:23  ullrich
 * Added calibration vertices.
 *
 * Revision 2.23  2001/11/07 21:19:42  ullrich
 * Added L1 trigger.
 *
 * Revision 2.22  2001/09/18 00:15:25  ullrich
 * Added StRunInfo and access functions.
 *
 * Revision 2.21  2001/05/30 17:45:53  perev
 * StEvent branching
 *
 * Revision 2.20  2001/05/17 22:56:33  ullrich
 * Removed all usage of dst_summary_param.
 *
 * Revision 2.19  2001/04/23 19:28:14  ullrich
 * Added StClusteringHints and methods to access it.
 *
 * Revision 2.18  2001/04/05 04:00:36  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.17  2001/03/14 02:35:43  ullrich
 * Added container and methods to handle PSDs.
 *
 * Revision 2.16  2001/03/09 05:24:01  ullrich
 * Added new method statistics().
 *
 * Revision 2.15  2000/12/08 03:53:41  ullrich
 * Prepared hooks for ToF.
 *
 * Revision 2.14  2000/09/25 14:21:30  ullrich
 * Removed enums for content vector. Replaced by lookup function.
 *
 * Revision 2.13  2000/09/06 22:34:17  ullrich
 * Changed mBunchCrossingNumber from scalar to array to hold all 64 bits.
 *
 * Revision 2.12  2000/06/19 01:32:15  perev
 *  Thomas StEvent branches added
 *
 * Revision 2.11  2000/05/24 15:46:10  ullrich
 * Added setSummary() method.
 *
 * Revision 2.10  2000/05/22 21:47:15  ullrich
 * Added RICH collection and related methods.
 *
 * Revision 2.9  2000/05/15 18:35:37  ullrich
 * All data member related to collections and containers are now
 * kept by pointer. The interface (public methods) stays the same.
 * Those methods which returns references were modified to create
 * an empty collection in case the pointer is null.
 *
 * Revision 2.8  2000/04/26 20:33:26  ullrich
 * Removed redundant virtual keywords.
 *
 * Revision 2.7  2000/04/18 17:31:28  perev
 * StEvent::Browse overload of TDataSet:;One
 *
 * Revision 2.6  2000/03/29 16:54:15  ullrich
 * Added L3 trigger.
 *
 * Revision 2.5  2000/02/23 17:36:02  ullrich
 * Changes due to the addition of the EMC to StEvent
 *
 * Revision 2.4  2000/01/13 21:06:22  lasiuk
 * add rich pixel info/containers
 *
 * Revision 2.3  2000/01/05 16:02:28  ullrich
 * SSD hits added to StEvent.
 *
 * Revision 2.2  1999/11/04 13:30:42  ullrich
 * Added constructor without summary table
 *
 * Revision 2.1  1999/10/28 22:25:10  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.0  1999/10/12 18:41:58  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StEvent_hh
#define StEvent_hh
/// @file StEvent.h
/// @brief Top-level transient event container for all STAR detector data.

#include "St_DataSet.h"
#include "TString.h"
#include "StContainers.h"
#include "StEnumerations.h"

class StCalibrationVertex;
class StDetectorState;
class StEventClusteringHints;
class StEventInfo;
class StEventSummary;
class StFpdCollection;
class StPhmdCollection;
class StTpcHitCollection;
class StFtpcHitCollection;
class StSvtHitCollection;
class StSsdHitCollection;
class StSstHitCollection;
class StEtrHitCollection;
class StEmcCollection;
class StEpdCollection;
class StFcsCollection;
class StFttCollection;
class StFmsCollection;
class StRHICfCollection;
class StRichCollection;
class StRpsCollection;
class StTofCollection;
class StBTofCollection;
class StETofCollection;
class StMtdCollection;
class StTriggerDetectorCollection;
class StTriggerIdCollection;
class StTriggerData;
class StL0Trigger;
class StL1Trigger;
class StL3Trigger;
class StPrimaryVertex;
class StRunInfo;
class StV0Vertex;
class StXiVertex;
class StKinkVertex;
class StPsd;
class StRnDHitCollection;
class StHltEvent;
class StFgtCollection;
class StPxlHitCollection;
class StIstHitCollection;
class StFstEvtCollection;
class StFstHitCollection;
class StFwdTrackCollection;
class StGmtCollection;

class StEvent : public StXRefMain {
public:
    StEvent();
    virtual ~StEvent();

    /// \name Event identification and metadata
    /// @{
    /// CVS tag of the StEvent library build.
    static const TString&               cvsTag();
    /// Event type string (e.g. "physics", "laser", "pedestal").
    TString                             type() const;
    /// Unique event number within the run.
    int                                 id() const;
    /// Run number.
    int                                 runId() const;
    /// Unix timestamp of the event trigger.
    int                                 time() const;
    /// Bitmask of fired L0 triggers.
    unsigned int                        triggerMask() const;
    /// RHIC bunch-crossing number; \a index selects the 32-bit word (0 or 1).
    unsigned int                        bunchCrossingNumber(unsigned int) const;
    /// @}

    /// \name Event and run information objects
    /// @{
    /// Per-event metadata (event/run numbers, timestamp, trigger mask).
    StEventInfo*                        info();
    const StEventInfo*                  info() const;

    /// Run-level metadata (beam parameters, drift velocities, luminosity).
    StRunInfo*                          runInfo();
    const StRunInfo*                    runInfo() const;

    /// Global event summary (multiplicity, centrality, energy sums).
    StEventSummary*                     summary();
    const StEventSummary*               summary() const;
    /// @}
    
    /// \name TPC and FTPC hit collections
    /// @{
    /// Main Time Projection Chamber hit collection.
    StTpcHitCollection*                 tpcHitCollection();
    const StTpcHitCollection*           tpcHitCollection() const;
    /// Forward Time Projection Chamber hit collection.
    StFtpcHitCollection*                ftpcHitCollection();
    const StFtpcHitCollection*          ftpcHitCollection() const;
    /// @}

    /// \name SVT, SSD, and SST silicon hit collections
    /// @{
    /// Silicon Vertex Tracker hit collection.
    StSvtHitCollection*                 svtHitCollection();
    const StSvtHitCollection*           svtHitCollection() const;
    /// Silicon Strip Detector hit collection.
    StSsdHitCollection*                 ssdHitCollection();
    const StSsdHitCollection*           ssdHitCollection() const;
    /// Silicon Strip Tracker (SST/SSD upgrade) hit collection.
    StSstHitCollection*                 sstHitCollection();
    const StSstHitCollection*           sstHitCollection() const;
    /// @}

    /// \name Electromagnetic and forward calorimeter collections
    /// @{
    /// Barrel and Endcap EMC collection (towers, clusters, points).
    StEmcCollection*                    emcCollection();
    const StEmcCollection*              emcCollection() const;
    /// Forward Calorimeter System (FCS) collection.
    StFcsCollection*                    fcsCollection();
    const StFcsCollection*              fcsCollection() const;
    /// Forward Tracking Telescope (FTT) collection.
    StFttCollection*                    fttCollection();
    const StFttCollection*              fttCollection() const;
    /// Forward Meson Spectrometer (FMS) collection.
    StFmsCollection*                    fmsCollection();
    const StFmsCollection*              fmsCollection() const;
    /// RHICf calorimeter collection.
    StRHICfCollection*                  rhicfCollection();
    const StRHICfCollection*            rhicfCollection() const;
    /// RICH detector collection.
    StRichCollection*                   richCollection();
    const StRichCollection*             richCollection() const;
    /// Roman Pot System (pp2pp/RPS) collection.
    StRpsCollection*                    rpsCollection();
    const StRpsCollection*              rpsCollection() const;
    /// @}

    /// \name Time-of-flight and timing detector collections
    /// @{
    /// Legacy TOF (pVPD/TOFr) collection.
    StTofCollection*                    tofCollection();
    const StTofCollection*              tofCollection() const;
    /// Barrel TOF (BTOF) collection.
    StBTofCollection*                   btofCollection();
    const StBTofCollection*             btofCollection() const;
    /// Endcap TOF (ETOF) collection.
    StETofCollection*                   etofCollection();
    const StETofCollection*             etofCollection() const;
    /// Muon Telescope Detector (MTD) collection.
    StMtdCollection*                    mtdCollection();
    const StMtdCollection*              mtdCollection() const;
    /// @}

    /// \name Other detector collections
    /// @{
    /// Forward Pion Detector (FPD) collection.
    StFpdCollection*                    fpdCollection();
    const StFpdCollection*              fpdCollection() const;
    /// Photon Multiplicity Detector (PMD/PHMD) collection.
    StPhmdCollection*                   phmdCollection();
    const StPhmdCollection*             phmdCollection() const;
    /// R&D generic hit collection.
    StRnDHitCollection*                 rndHitCollection();
    const StRnDHitCollection*           rndHitCollection() const;
    /// Endcap tracking detector (ETr) hit collection.
    StEtrHitCollection*                 etrHitCollection();
    const StEtrHitCollection*           etrHitCollection() const;
    /// High-Level Trigger event data.
    StHltEvent*                         hltEvent();
    const StHltEvent*                   hltEvent() const;
    /// Forward GEM Tracker (FGT) collection.
    StFgtCollection*                    fgtCollection();
    const StFgtCollection*              fgtCollection() const;
    /// @}

    /// \name HFT pixel and strip detector collections (PXL, IST, FST)
    /// @{
    /// Pixel detector (PXL) hit collection.
    StPxlHitCollection*                 pxlHitCollection();
    const StPxlHitCollection*           pxlHitCollection() const;
    /// Intermediate Silicon Tracker (IST) hit collection.
    StIstHitCollection*                 istHitCollection();
    const StIstHitCollection*           istHitCollection() const;
    /// Forward Silicon Tracker (FST) event collection.
    StFstEvtCollection*                 fstEvtCollection();
    const StFstEvtCollection*           fstEvtCollection() const;
    /// Forward Silicon Tracker (FST) hit collection.
    StFstHitCollection*                 fstHitCollection();
    const StFstHitCollection*           fstHitCollection() const;
    /// Event Plane Detector (EPD) collection.
    StEpdCollection*                    epdCollection();
    const StEpdCollection*              epdCollection() const;
    /// @}

    /// \name Trigger objects
    /// @{
    /// Level-0 trigger data.
    StL0Trigger*                        l0Trigger();
    const StL0Trigger*                  l0Trigger() const;
    /// Level-1 trigger data.
    StL1Trigger*                        l1Trigger();
    const StL1Trigger*                  l1Trigger() const;
    /// Level-3 (online) trigger data.
    StL3Trigger*                        l3Trigger();
    const StL3Trigger*                  l3Trigger() const;
    /// Trigger detector (CTB, ZDC, BBC, …) collection.
    StTriggerDetectorCollection*        triggerDetectorCollection();
    const StTriggerDetectorCollection*  triggerDetectorCollection() const;
    /// Collection of trigger ID objects for this event.
    StTriggerIdCollection*              triggerIdCollection();
    const StTriggerIdCollection*        triggerIdCollection() const;
    /// Raw trigger data bank.
    StTriggerData*                      triggerData();
    const StTriggerData*                triggerData() const;
    /// Global Muon Trigger (GMT) collection.
    StGmtCollection*                    gmtCollection();
    const StGmtCollection*              gmtCollection() const;
    /// @}
    
    /// \name Track and track-detector info containers
    /// @{
    /// Collection of StTrackDetectorInfo objects shared among tracks.
    StSPtrVecTrackDetectorInfo&         trackDetectorInfo();
    const StSPtrVecTrackDetectorInfo&   trackDetectorInfo() const;
    
    /// Ordered collection of StTrackNode objects; each node groups one
    /// global track with its associated primary track(s).
    StSPtrVecTrackNode&                 trackNodes();
    const StSPtrVecTrackNode&           trackNodes() const;
    /// @}

    /// \name Primary and calibration vertices
    /// @{
    /// Number of primary-vertex candidates reconstructed in this event.
    unsigned int                        numberOfPrimaryVertices() const;
    /// Primary vertex by rank index (default 0 = highest-ranked vertex).
    StPrimaryVertex*                    primaryVertex(unsigned int = 0);
    const StPrimaryVertex*              primaryVertex(unsigned int = 0) const;

    /// Number of calibration vertices stored in this event.
    unsigned int                        numberOfCalibrationVertices() const;
    /// Calibration vertex by index.
    StCalibrationVertex*                calibrationVertex(unsigned int);
    const StCalibrationVertex*          calibrationVertex(unsigned int) const;
    /// @}

    /// \name Secondary vertex collections (V0, Xi, Kink)
    /// @{
    /// Collection of V0 (two-prong neutral) decay vertices.
    StSPtrVecV0Vertex&                  v0Vertices();
    const StSPtrVecV0Vertex&            v0Vertices() const;
    /// Collection of Xi (cascade) decay vertices.
    StSPtrVecXiVertex&                  xiVertices();
    const StSPtrVecXiVertex&            xiVertices() const;
    /// Collection of kink decay vertices.
    StSPtrVecKinkVertex&                kinkVertices();
    const StSPtrVecKinkVertex&          kinkVertices() const;
    /// @}

    /// \name Forward track collection
    /// @{
    /// Collection of forward (FWD) reconstructed tracks.
    StFwdTrackCollection*               fwdTrackCollection();
    const StFwdTrackCollection*         fwdTrackCollection() const;
    /// @}

    /// \name Detector state and PSDs
    /// @{
    /// Detector status object for the given detector identifier.
    StDetectorState*                    detectorState(StDetectorId);
    const StDetectorState*              detectorState(StDetectorId) const;
    
    /// Pseudo-Slow Control data for the given physics working group and index.
    StPsd*                              psd(StPwg, int);
    const StPsd*                        psd(StPwg, int) const;
    /// Total number of PSDs stored.
    unsigned int                        numberOfPsds() const;
    /// Number of PSDs for a specific physics working group.
    unsigned int                        numberOfPsds(StPwg) const;
    /// @}

    /// \name Generic hit collection access and I/O utilities
    /// @{
    /// Return named hit collection (used for non-standard detectors).
    StSPtrVecHit*                       hitCollection(const Char_t *name);
    
    StSPtrVecObject&                    content();               // for IO purposes only

    const StEventClusteringHints*       clusteringHints() const; // for IO purposes only
    StEventClusteringHints*             clusteringHints();       // for IO purposes only
    
    void                                statistics();            // *MENU*
    /// @}

    /// \name Setters — event identification and metadata
    /// @{
    void setType(const char*);           ///< Set event type string.
    void setRunId(int);                  ///< Set run number.
    void setId(int);                     ///< Set event number.
    void setTime(int);                   ///< Set Unix timestamp.
    void setTriggerMask(unsigned int);   ///< Set L0 trigger bitmask.
    void setBunchCrossingNumber(unsigned int, unsigned int); ///< Set RHIC bunch-crossing word \a index to \a value.
    void setInfo(StEventInfo*);          ///< Attach an StEventInfo object (takes ownership).
    void setRunInfo(StRunInfo*);         ///< Attach an StRunInfo object (takes ownership).
    void setSummary(StEventSummary*);    ///< Attach an StEventSummary object (takes ownership).
    void setIdTruth();                   ///< Propagate MC truth IDs from hits to tracks and vertices.
    /// @}

    /// \name Setters — detector hit and data collections
    /// @{
    void setTpcHitCollection(StTpcHitCollection*);      ///< Attach the TPC hit collection.
    void setRnDHitCollection(StRnDHitCollection*);      ///< Attach the R&D hit collection.
    void setEtrHitCollection(StEtrHitCollection*);      ///< Attach the ETr hit collection.
    void setFtpcHitCollection(StFtpcHitCollection*);    ///< Attach the FTPC hit collection.
    void setSvtHitCollection(StSvtHitCollection*);      ///< Attach the SVT hit collection.
    void setSsdHitCollection(StSsdHitCollection*);      ///< Attach the SSD hit collection.
    void setSstHitCollection(StSstHitCollection*);      ///< Attach the SST hit collection.
    void setPxlHitCollection(StPxlHitCollection*);      ///< Attach the PXL hit collection.
    void setIstHitCollection(StIstHitCollection*);      ///< Attach the IST hit collection.
    void setFstEvtCollection(StFstEvtCollection*);      ///< Attach the FST event collection.
    void setFstHitCollection(StFstHitCollection*);      ///< Attach the FST hit collection.
    void setEmcCollection(StEmcCollection*);            ///< Attach the EMC collection.
    void setEpdCollection(StEpdCollection*);            ///< Attach the EPD collection.
    void setFcsCollection(StFcsCollection*);            ///< Attach the FCS collection.
    void setFttCollection(StFttCollection*);            ///< Attach the FTT collection.
    void setFmsCollection(StFmsCollection*);            ///< Attach the FMS collection.
    void setRHICfCollection(StRHICfCollection*);        ///< Attach the RHICf collection.
    void setRichCollection(StRichCollection*);          ///< Attach the RICH collection.
    void setRpsCollection(StRpsCollection*);            ///< Attach the RPS collection.
    void setTofCollection(StTofCollection*);            ///< Attach the legacy TOF collection.
    void setBTofCollection(StBTofCollection*);          ///< Attach the BTOF collection.
    void setETofCollection(StETofCollection*);          ///< Attach the ETOF collection.
    void setMtdCollection(StMtdCollection*);            ///< Attach the MTD collection.
    void setFpdCollection(StFpdCollection*);            ///< Attach the FPD collection.
    void setPhmdCollection(StPhmdCollection*);          ///< Attach the PMD/PHMD collection.
    void setTriggerDetectorCollection(StTriggerDetectorCollection*); ///< Attach the trigger-detector collection.
    void setTriggerIdCollection(StTriggerIdCollection*);             ///< Attach the trigger-ID collection.
    void setTriggerData(StTriggerData*);                ///< Attach the raw trigger data bank.
    void setL0Trigger(StL0Trigger*);                    ///< Attach L0 trigger data.
    void setL1Trigger(StL1Trigger*);                    ///< Attach L1 trigger data.
    void setL3Trigger(StL3Trigger*);                    ///< Attach L3 trigger data.
    void setHltEvent(StHltEvent*);                      ///< Attach the HLT event object.
    void setFgtCollection(StFgtCollection*);            ///< Attach the FGT collection.
    void setFwdTrackCollection(StFwdTrackCollection*);  ///< Attach the forward-track collection.
    /// @}

    /// \name Adders — vertices, detector states, and dynamic collections
    /// @{
    /// Add a primary vertex candidate; \a order controls ranking criterion.
    void addPrimaryVertex(StPrimaryVertex*, StPrimaryVertexOrder = orderByNumberOfDaughters);
    void addCalibrationVertex(StCalibrationVertex*); ///< Add a calibration vertex.
    void addDetectorState(StDetectorState*);          ///< Add a detector state record.
    void addPsd(StPsd*);                              ///< Add a pseudo-slow-control data object.
    void removePsd(StPsd*);                           ///< Remove and delete a PSD object.
    void addHitCollection(StSPtrVecHit* p, const Char_t *name);    ///< Register a named hit collection.
    void removeHitCollection(const Char_t *name);                   ///< Unregister a named hit collection.
    void setGmtCollection(StGmtCollection*);          ///< Attach the GMT collection.
    /// @}

    /// @brief ROOT notification hook (used for I/O branch activation).
    virtual Bool_t Notify();
    
protected:
    mutable StSPtrVecObject  mContent;  ///< Master container of all owned sub-objects (for I/O).
    static  TString          mCvsTag;   ///< CVS tag of the StEvent library used to create this event.
    /// @brief Split this event into separate ROOT branches for I/O.
    void    Split();
    /// @brief Returns 1; indicates this is the main event branch (used by I/O framework).
     int    IsMain() const 	{return 1;}
    
private:
    StEvent& operator=(const StEvent&);
    StEvent(const StEvent&);
    void initToZero();
    
    ClassDef(StEvent,6)
};
#endif







