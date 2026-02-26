/***************************************************************************
 *
 * $Id: StHltTrackNode.h,v 2.1 2011/02/01 19:45:47 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltTrackNode.h,v $
 * Revision 2.1  2011/02/01 19:45:47  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltTrackNode_hh
#define StHltTrackNode_hh

/// @file StHltTrackNode.h
/// @brief HLT track node grouping a global track, primary track, and associated detector hits.

#include <Stiostream.h>
#include "StObject.h"
#include "StArray.h"

#include "StHltTriggerReasonCapable.h"

class StHltTrack;
class StHltBTofHit;
class StHltBEmcTowerHit;


/// @brief HLT track node grouping a global track, primary track, BTOF hit, and BEMC tower hit for one particle.
class StHltTrackNode : public StHltTriggerReasonCapable {
public:
    StHltTrackNode();
    ~StHltTrackNode();

    /// @brief Associated HLT global track.
    StHltTrack* globalTrack();
    const StHltTrack* globalTrack() const;

    /// @brief Associated HLT primary track.
    StHltTrack* primaryTrack();
    const StHltTrack* primaryTrack() const;

    /// @brief Associated BTOF hit.
    StHltBTofHit* bTofHit();
    const StHltBTofHit* bTofHit() const;

    /// @brief Associated BEMC tower hit.
    StHltBEmcTowerHit* bEmcTowerHit();
    const StHltBEmcTowerHit* bEmcTowerHit() const;

    /// @brief Serial number of the associated global track.
    int globalTrackSN() const;
    /// @brief Serial number of the associated primary track.
    int primaryTrackSN() const;
    /// @brief Serial number of the associated TOF hit.
    int tofHitSN() const;
    /// @brief Serial number of the associated EMC tower hit.
    int emcTowerSN() const;

    /// @brief φ difference between track projection and matched BEMC tower (rad).
    double bEmcMatchPhiDiff() const;
    /// @brief z-edge distance for BEMC tower match (cm).
    double bEmcMatchZEdge() const;

    /// @brief Projected BTOF channel for this track node.
    float bTofProjChannel() const;
    /// @brief Local y position within the matched BTOF cell (cm).
    float bTofCellLocalY() const;
    /// @brief Local z position within the matched BTOF cell (cm).
    float bTofCellLocalZ() const;
    /// @brief Track path length to the matched BTOF hit (cm).
    float bTofPathLength() const;
    /// @brief Velocity β = v/c of the track.
    float beta() const;
    /// @brief Measured time-of-flight (ns).
    float tof() const;

    /// @brief Set the associated global track.
    void setGlobalTrack(StHltTrack*);
    /// @brief Set the associated primary track.
    void setPrimaryTrack(StHltTrack*);
    /// @brief Set the associated BTOF hit.
    void setBTofHit(StHltBTofHit*);
    /// @brief Set the associated BEMC tower hit.
    void setBEmcTowerHit(StHltBEmcTowerHit*);
    /// @brief Set the global track serial number.
    void setGlobalTrackSN(int);
    /// @brief Set the primary track serial number.
    void setPrimaryTrackSN(int);
    /// @brief Set the TOF hit serial number.
    void setTofHitSN(int);
    /// @brief Set the EMC tower hit serial number.
    void setEmcTowerSN(int);
    /// @brief Set the BEMC φ match difference (rad).
    void setBEmcMatchPhiDiff(double);
    /// @brief Set the BEMC z-edge match distance (cm).
    void setBEmcMatchZEdge(double);
    /// @brief Set the projected BTOF channel.
    void setBTofProjChannel(float);
    /// @brief Set the BTOF cell local y (cm).
    void setBTofCellLocalY(float);
    /// @brief Set the BTOF cell local z (cm).
    void setBTofCellLocalZ(float);
    /// @brief Set the BTOF path length (cm).
    void setBTofPathLength(float);
    /// @brief Set the velocity β.
    void setBeta(float);
    /// @brief Set the measured time-of-flight (ns).
    void setTof(float);
    
private:
    
#ifdef __CINT__
    StObjLink mGlobalTrack;
    StObjLink mPrimaryTrack;
    StObjLink mBTofHit;
    StObjLink mBEmcTowerHit;
#else
    StLink<StHltTrack> mGlobalTrack;
    StLink<StHltTrack> mPrimaryTrack;
    StLink<StHltBTofHit> mBTofHit;
    StLink<StHltBEmcTowerHit> mBEmcTowerHit;
#endif //__CINT__
    
    double mBEmcMatchPhiDiff;  ///< φ difference for BEMC tower match (rad).
    double mBEmcMatchZEdge;    ///< z-edge distance for BEMC tower match (cm).

    int mGlobalTrackSN;   ///< Serial number of the associated global track.
    int mPrimaryTrackSN;  ///< Serial number of the associated primary track.
    int mTofHitSN;        ///< Serial number of the associated TOF hit.
    int mEmcTowerSN;      ///< Serial number of the associated EMC tower hit.

    float mBTofProjChannel;  ///< Projected BTOF channel.
    float mBTofCellLocalY;   ///< Local y in matched BTOF cell (cm).
    float mBTofCellLocalZ;   ///< Local z in matched BTOF cell (cm).
    float mBTofPathLength;   ///< Track path length to the BTOF hit (cm).
    float mBeta;             ///< Velocity β = v/c of the track.
    float mTof;              ///< Measured time-of-flight (ns).
    
    ClassDef(StHltTrackNode,1)
};

ostream& operator<<(ostream&, const StHltTrackNode&);


#endif






