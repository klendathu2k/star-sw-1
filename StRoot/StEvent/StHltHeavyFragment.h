/***************************************************************************
 *
 * $Id: StHltHeavyFragment.h,v 2.1 2011/02/01 19:45:47 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltHeavyFragment.h,v $
 * Revision 2.1  2011/02/01 19:45:47  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltHeavyFragment_hh
#define StHltHeavyFragment_hh

/// @file StHltHeavyFragment.h
/// @brief HLT heavy nuclear fragment candidate selected online by the High Level Trigger.

#include <Stiostream.h>
#include "StObject.h"
#include "StArray.h"

#include "StHltTrack.h"
#include "StHltBTofHit.h"
#include "StHltBEmcTowerHit.h"
#include "StHltTriggerReasonCapable.h"

/// @brief HLT heavy nuclear fragment candidate (e.g. 3He, 4He) selected online by the High Level Trigger.
class StHltHeavyFragment : public StHltTriggerReasonCapable {

public:

    /// @brief Default constructor.
    StHltHeavyFragment();
    /// @brief Destructor.
    ~StHltHeavyFragment();

    /// @brief Primary track of the heavy fragment candidate.
    StHltTrack& primaryTrack();
    const StHltTrack& primaryTrack() const;
    /// @brief Global track of the heavy fragment candidate.
    StHltTrack& globalTrack();
    const StHltTrack& globalTrack() const;
    /// @brief BTOF hit matched to the heavy fragment candidate.
    StHltBTofHit& bTofHit();
    const StHltBTofHit& bTofHit() const;
    /// @brief BEMC tower hit matched to the heavy fragment candidate.
    StHltBEmcTowerHit& bEmcTowerHit();
    const StHltBEmcTowerHit& bEmcTowerHit() const;

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

    /// @brief Projected BTOF channel for this candidate.
    float bTofProjChannel() const;
    /// @brief Local y position within the matched BTOF cell (cm).
    float bTofCellLocalY() const;
    /// @brief Local z position within the matched BTOF cell (cm).
    float bTofCellLocalZ() const;
    /// @brief Track path length to the matched BTOF hit (cm).
    float bTofPathLength() const;
    /// @brief Velocity β = v/c of the candidate.
    float beta() const;
    /// @brief Measured time-of-flight (ns).
    float tof() const;

    /// @brief Set the global track.
    void setGlobalTrack(const StHltTrack &);
    /// @brief Set the primary track.
    void setPrimaryTrack(const StHltTrack &);
    /// @brief Set the matched BTOF hit.
    void setBTofHit(const StHltBTofHit &);
    /// @brief Set the matched BEMC tower hit.
    void setBEmcTowerHit(const StHltBEmcTowerHit &);
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
    
    StHltTrack mPrimaryTrack;          ///< Primary track of the heavy fragment candidate.
    StHltTrack mGlobalTrack;           ///< Global track of the heavy fragment candidate.
    StHltBTofHit mBTofHit;             ///< BTOF hit matched to the heavy fragment candidate.
    StHltBEmcTowerHit mBEmcTowerHit;   ///< BEMC tower hit matched to the heavy fragment candidate.

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
    float mBeta;             ///< Velocity β = v/c of the candidate.
    float mTof;              ///< Measured time-of-flight (ns).
    
    ClassDef(StHltHeavyFragment,1)
};

ostream& operator<<(ostream&, const StHltHeavyFragment&);


#endif


