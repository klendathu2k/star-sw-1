/***************************************************************************
 *
 * $Id: StHltEvent.h,v 2.1 2011/02/01 19:45:47 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltEvent.h,v $
 * Revision 2.1  2011/02/01 19:45:47  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltEvent_hh
#define StHltEvent_hh

/// @file StHltEvent.h
/// @brief Top-level HLT event container holding all online-selected objects.

#include "StObject.h"
#include "StContainers.h"
#include "StThreeVectorF.hh"

class  StHltTrack;
class  StHltTrackNode;
class  StHltBEmcTowerHit; 
class  StHltBTofHit;
class  StHltVpdHit;
class  StHltHighPt;
class  StHltHeavyFragment;
class  StHltDiElectron;
class  StHltTriggerReason;

/// @brief Top-level HLT event container holding all online-selected physics objects.
class StHltEvent : public StObject{
public:
    StHltEvent();
    virtual ~StHltEvent();

    /// @brief Data format version number.
    unsigned int version() const;
    /// @brief Bitwise OR of all StHltTriggerReasonType bits that fired in this event.
    unsigned int triggerReasonBitOred() const;

    /// @brief Primary vertex position from the HLT (cm).
    StThreeVectorF&   vertex();
    const StThreeVectorF& vertex() const;

    /// @brief Vertex position from the low-multiplicity algorithm (cm).
    StThreeVectorF&   lowMultVertex();
    const StThreeVectorF& lowMultVertex() const;

    /// @brief VPD-based vertex z position (cm).
    float vpdVertexZ() const;
    /// @brief Event start time t0 from the VPD (ns).
    float t0() const;
    /// @brief Online dE/dx gain for the TPC inner sectors.
    float innerSecGain() const;
    /// @brief Online dE/dx gain for the TPC outer sectors.
    float outerSecGain() const;

    /// @brief Collection of HLT global tracks.
    StSPtrVecHltTrack& globalTrack();
    const StSPtrVecHltTrack& globalTrack() const;
    /// @brief Collection of HLT primary tracks.
    StSPtrVecHltTrack& primaryTrack();
    const StSPtrVecHltTrack& primaryTrack() const;
    /// @brief Collection of HLT track nodes.
    StSPtrVecHltTrackNode& trackNode();
    const StSPtrVecHltTrackNode& trackNode() const;
    /// @brief Collection of HLT BEMC tower hits.
    StSPtrVecHltBEmcTowerHit& bEmcTowerHits();
    const StSPtrVecHltBEmcTowerHit& bEmcTowerHits() const;
    /// @brief Collection of HLT BTOF hits.
    StSPtrVecHltBTofHit& bTofHit();
    const StSPtrVecHltBTofHit& bTofHit() const;
    /// @brief Collection of HLT VPD hits.
    StSPtrVecHltVpdHit& vpdHit();
    const StSPtrVecHltVpdHit& vpdHit() const;
    /// @brief Collection of HLT high-pT track candidates.
    StSPtrVecHltHighPt& highPt();
    const StSPtrVecHltHighPt& highPt() const;
    /// @brief Collection of HLT heavy nuclear fragment candidates.
    StSPtrVecHltHeavyFragment& heavyFragment();
    const StSPtrVecHltHeavyFragment& heavyFragment() const;
    /// @brief Collection of HLT di-electron pair candidates.
    StSPtrVecHltDiElectron& diElectron();
    const StSPtrVecHltDiElectron& diElectron() const;
    /// @brief Collection of HLT trigger reason records.
    StSPtrVecHltTriggerReason& triggerReason();
    const StSPtrVecHltTriggerReason& triggerReason() const;

    /// @brief Add a global track to the event.
    void addGlobalTrack(const StHltTrack*);
    /// @brief Add a primary track to the event.
    void addPrimaryTrack(const StHltTrack*);
    /// @brief Add a track node to the event.
    void addTrackNode(const StHltTrackNode*);
    /// @brief Add a BEMC tower hit to the event.
    void addBEmcTowerHit(const StHltBEmcTowerHit*);
    /// @brief Add a BTOF hit to the event.
    void addBTofHit(const StHltBTofHit*);
    /// @brief Add a VPD hit to the event.
    void addVpdHit(const StHltVpdHit*);
    /// @brief Add a high-pT candidate to the event.
    void addHighPt(const StHltHighPt*);
    /// @brief Add a heavy fragment candidate to the event.
    void addHeavyFragment(const StHltHeavyFragment*);
    /// @brief Add a di-electron candidate to the event.
    void addDiElectron(const StHltDiElectron*);
    /// @brief Add a trigger reason record to the event.
    void addTriggerReason(const StHltTriggerReason*);

    /// @brief Set the data format version number.
    void setVersion(unsigned int);
    /// @brief Set the bitwise-OR of all trigger reason bits.
    void setTriggerReasonBitOred(unsigned int);
    /// @brief Set the primary vertex position (cm).
    void setVertex(const StThreeVectorF&);
    /// @brief Set the low-multiplicity vertex position (cm).
    void setLowMultVertex(const StThreeVectorF&);
    /// @brief Set the VPD vertex z position (cm).
    void setVpdVertexZ(float);
    /// @brief Set the event start time t0 (ns).
    void setT0(float);
    /// @brief Set the online dE/dx gain for the TPC inner sectors.
    void setInnerSecGain(float);
    /// @brief Set the online dE/dx gain for the TPC outer sectors.
    void setOuterSecGain(float);
    
    
private:
    unsigned int mVersion;                   ///< Data format version number.
    unsigned int mTriggerReasonBitOred;      ///< Bitwise OR of all StHltTriggerReasonType bits that fired.
    StThreeVectorF mVertex;                  ///< Primary vertex position from the HLT (cm).
    StThreeVectorF mLowMultVertex;           ///< Vertex from the low-multiplicity algorithm (cm).
    float mVpdVertexZ;                       ///< VPD-based vertex z position (cm).
    float mT0;                               ///< Event start time t0 from the VPD (ns).
    float mInnerSecGain;                     ///< Online dE/dx gain for the TPC inner sectors.
    float mOuterSecGain;                     ///< Online dE/dx gain for the TPC outer sectors.

    StSPtrVecHltTrack mGlobalTrack;          ///< Collection of HLT global tracks.
    StSPtrVecHltTrack mPrimaryTrack;         ///< Collection of HLT primary tracks.
    StSPtrVecHltTrackNode mTrackNode;        ///< Collection of HLT track nodes.
    StSPtrVecHltBEmcTowerHit mBEmcTowerHits; ///< Collection of HLT BEMC tower hits.
    StSPtrVecHltBTofHit mBTofHit;            ///< Collection of HLT BTOF hits.
    StSPtrVecHltVpdHit  mVpdHit;             ///< Collection of HLT VPD hits.
    StSPtrVecHltHighPt  mHighPt;             ///< Collection of HLT high-pT candidates.
    StSPtrVecHltHeavyFragment  mHeavyFragment; ///< Collection of HLT heavy fragment candidates.
    StSPtrVecHltDiElectron  mDiElectron;     ///< Collection of HLT di-electron candidates.
    StSPtrVecHltTriggerReason  mTriggerReason; ///< Collection of HLT trigger reason records.
    
    ClassDef(StHltEvent,1)
};

inline unsigned int StHltEvent::version() const {return mVersion;}
inline unsigned int StHltEvent::triggerReasonBitOred() const {return mTriggerReasonBitOred;}
inline float StHltEvent::vpdVertexZ() const {return mVpdVertexZ;}
inline float StHltEvent::t0() const {return mT0;}
inline float StHltEvent::innerSecGain() const {return mInnerSecGain;}
inline float StHltEvent::outerSecGain() const {return mOuterSecGain;}

inline StSPtrVecHltTrack& StHltEvent::globalTrack() {return mGlobalTrack;}
inline const StSPtrVecHltTrack& StHltEvent::globalTrack() const {return mGlobalTrack;}
inline StSPtrVecHltTrack& StHltEvent::primaryTrack() {return mPrimaryTrack;}
inline const StSPtrVecHltTrack& StHltEvent::primaryTrack() const {return mPrimaryTrack;}
inline StSPtrVecHltTrackNode& StHltEvent::trackNode() {return mTrackNode;}
inline const StSPtrVecHltTrackNode& StHltEvent::trackNode() const {return mTrackNode;}
inline StSPtrVecHltBEmcTowerHit& StHltEvent::bEmcTowerHits() {return mBEmcTowerHits;}
inline const StSPtrVecHltBEmcTowerHit& StHltEvent::bEmcTowerHits() const {return mBEmcTowerHits;}
inline StSPtrVecHltBTofHit& StHltEvent::bTofHit() {return mBTofHit;}
inline const StSPtrVecHltBTofHit& StHltEvent::bTofHit() const {return mBTofHit;}
inline StSPtrVecHltVpdHit& StHltEvent::vpdHit() {return mVpdHit;}
inline const StSPtrVecHltVpdHit& StHltEvent::vpdHit() const {return mVpdHit;}
inline StSPtrVecHltHighPt& StHltEvent::highPt() {return mHighPt;}
inline const StSPtrVecHltHighPt& StHltEvent::highPt() const {return mHighPt;}
inline StSPtrVecHltHeavyFragment& StHltEvent::heavyFragment() {return mHeavyFragment;}
inline const StSPtrVecHltHeavyFragment& StHltEvent::heavyFragment() const {return mHeavyFragment;}
inline StSPtrVecHltDiElectron& StHltEvent::diElectron() {return mDiElectron;}
inline const StSPtrVecHltDiElectron& StHltEvent::diElectron() const {return mDiElectron;}
inline StSPtrVecHltTriggerReason& StHltEvent::triggerReason() {return mTriggerReason;}
inline const StSPtrVecHltTriggerReason& StHltEvent::triggerReason() const {return mTriggerReason;}


#endif



