/***************************************************************************
 *
 * $Id: StHltDiElectron.h,v 2.1 2011/02/01 19:45:47 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltDiElectron.h,v $
 * Revision 2.1  2011/02/01 19:45:47  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltDiElectron_hh
#define StHltDiElectron_hh

/// @file StHltDiElectron.h
/// @brief HLT di-electron pair candidate selected online by the High Level Trigger.

#include <Stiostream.h>
#include "StObject.h"
#include "StArray.h"

#include "StHltTrack.h"
#include "StHltBTofHit.h"
#include "StHltBEmcTowerHit.h"
#include "StHltTriggerReasonCapable.h"

/// @brief HLT di-electron pair candidate built from two electron tracks with optional BTOF and BEMC matching.
class StHltDiElectron : public StHltTriggerReasonCapable {
public:
    StHltDiElectron();               ///< default constructor
    ~StHltDiElectron();

    float invariantMass() const;  ///< Invariant mass of the di-electron pair (GeV/c²).
    /// @brief Transverse momentum of the di-electron pair (GeV/c).
    float pt() const;
    /// @brief Azimuthal angle ψ of the pair momentum (rad).
    float psi() const;
    /// @brief Tangent of the dip angle of the pair momentum.
    float tanl() const;

    StHltTrack& daughter1primaryTrack();                     ///< Primary track of daughter 1.
    const StHltTrack& daughter1primaryTrack() const;
    StHltTrack& daughter1globalTrack();                      ///< Global track of daughter 1.
    const StHltTrack& daughter1globalTrack() const;
    StHltBTofHit& daughter1bTofHit();                        ///< BTOF hit matched to daughter 1.
    const StHltBTofHit& daughter1bTofHit() const;
    StHltBEmcTowerHit& daughter1bEmcTowerHit();              ///< BEMC tower hit matched to daughter 1.
    const StHltBEmcTowerHit& daughter1bEmcTowerHit() const;
    /// @brief Selection bit flags for daughter 1.
    int daughter1SelectionBit() const;
    /// @brief Serial number of the global track of daughter 1.
    int daughter1globalTrackSN() const;
    /// @brief Serial number of the primary track of daughter 1.
    int daughter1primaryTrackSN() const;
    /// @brief Serial number of the TOF hit of daughter 1.
    int daughter1tofHitSN() const;
    /// @brief Serial number of the EMC tower hit of daughter 1.
    int daughter1emcTowerSN() const;
    /// @brief φ difference between track projection and BEMC tower for daughter 1 (rad).
    double daughter1bEmcMatchPhiDiff() const;
    /// @brief z-edge distance for BEMC tower match of daughter 1 (cm).
    double daughter1bEmcMatchZEdge() const;
    /// @brief Projected BTOF channel for daughter 1.
    float daughter1bTofProjChannel() const;
    /// @brief Local y position within BTOF cell for daughter 1 (cm).
    float daughter1bTofCellLocalY() const;
    /// @brief Local z position within BTOF cell for daughter 1 (cm).
    float daughter1bTofCellLocalZ() const;
    /// @brief Track path length to BTOF hit for daughter 1 (cm).
    float daughter1bTofPathLength() const;
    /// @brief Velocity β = v/c for daughter 1.
    float daughter1beta() const;
    /// @brief Time-of-flight for daughter 1 (ns).
    float daughter1tof() const;

    StHltTrack& daughter2primaryTrack();                      ///< Primary track of daughter 2.
    const StHltTrack& daughter2primaryTrack() const;
    StHltTrack& daughter2globalTrack();                       ///< Global track of daughter 2.
    const StHltTrack& daughter2globalTrack() const;
    StHltBTofHit& daughter2bTofHit();                         ///< BTOF hit matched to daughter 2.
    const StHltBTofHit& daughter2bTofHit() const;
    StHltBEmcTowerHit& daughter2bEmcTowerHit();               ///< BEMC tower hit matched to daughter 2.
    const StHltBEmcTowerHit& daughter2bEmcTowerHit() const;
    /// @brief Selection bit flags for daughter 2.
    int daughter2SelectionBit() const;
    /// @brief Serial number of the global track of daughter 2.
    int daughter2globalTrackSN() const;
    /// @brief Serial number of the primary track of daughter 2.
    int daughter2primaryTrackSN() const;
    /// @brief Serial number of the TOF hit of daughter 2.
    int daughter2tofHitSN() const;
    /// @brief Serial number of the EMC tower hit of daughter 2.
    int daughter2emcTowerSN() const;
    /// @brief φ difference between track projection and BEMC tower for daughter 2 (rad).
    double daughter2bEmcMatchPhiDiff() const;
    /// @brief z-edge distance for BEMC tower match of daughter 2 (cm).
    double daughter2bEmcMatchZEdge() const;
    /// @brief Projected BTOF channel for daughter 2.
    float daughter2bTofProjChannel() const;
    /// @brief Local y position within BTOF cell for daughter 2 (cm).
    float daughter2bTofCellLocalY() const;
    /// @brief Local z position within BTOF cell for daughter 2 (cm).
    float daughter2bTofCellLocalZ() const;
    /// @brief Track path length to BTOF hit for daughter 2 (cm).
    float daughter2bTofPathLength() const;
    /// @brief Velocity β = v/c for daughter 2.
    float daughter2beta() const;
    /// @brief Time-of-flight for daughter 2 (ns).
    float daughter2tof() const;

    /// @brief Set the invariant mass of the pair (GeV/c²).
    void setInvariantMass(float);
    /// @brief Set the transverse momentum of the pair (GeV/c).
    void setPt(float);
    /// @brief Set the azimuthal angle ψ of the pair (rad).
    void setPsi(float);
    /// @brief Set the tangent of the dip angle of the pair.
    void setTanl(float);

    /// @brief Set the global track of daughter 1.
    void setDaughter1GlobalTrack(const StHltTrack &);
    /// @brief Set the primary track of daughter 1.
    void setDaughter1PrimaryTrack(const StHltTrack &);
    /// @brief Set the BTOF hit of daughter 1.
    void setDaughter1BTofHit(const StHltBTofHit &);
    /// @brief Set the BEMC tower hit of daughter 1.
    void setDaughter1BEmcTowerHit(const StHltBEmcTowerHit &);
    /// @brief Set the selection bit flags for daughter 1.
    void setDaughter1SelectionBit(int);
    /// @brief Set the global track serial number for daughter 1.
    void setDaughter1GlobalTrackSN(int);
    /// @brief Set the primary track serial number for daughter 1.
    void setDaughter1PrimaryTrackSN(int);
    /// @brief Set the TOF hit serial number for daughter 1.
    void setDaughter1TofHitSN(int);
    /// @brief Set the EMC tower serial number for daughter 1.
    void setDaughter1EmcTowerSN(int);
    /// @brief Set the BEMC φ match difference for daughter 1 (rad).
    void setDaughter1BEmcMatchPhiDiff(double);
    /// @brief Set the BEMC z-edge match distance for daughter 1 (cm).
    void setDaughter1BEmcMatchZEdge(double);
    /// @brief Set the projected BTOF channel for daughter 1.
    void setDaughter1BTofProjChannel(float);
    /// @brief Set the BTOF cell local y for daughter 1 (cm).
    void setDaughter1BTofCellLocalY(float);
    /// @brief Set the BTOF cell local z for daughter 1 (cm).
    void setDaughter1BTofCellLocalZ(float);
    /// @brief Set the BTOF path length for daughter 1 (cm).
    void setDaughter1BTofPathLength(float);
    /// @brief Set the velocity β for daughter 1.
    void setDaughter1Beta(float);
    /// @brief Set the time-of-flight for daughter 1 (ns).
    void setDaughter1Tof(float);

    /// @brief Set the global track of daughter 2.
    void setDaughter2GlobalTrack(const StHltTrack &);
    /// @brief Set the primary track of daughter 2.
    void setDaughter2PrimaryTrack(const StHltTrack &);
    /// @brief Set the BTOF hit of daughter 2.
    void setDaughter2BTofHit(const StHltBTofHit &);
    /// @brief Set the BEMC tower hit of daughter 2.
    void setDaughter2BEmcTowerHit(const StHltBEmcTowerHit &);
    /// @brief Set the selection bit flags for daughter 2.
    void setDaughter2SelectionBit(int);
    /// @brief Set the global track serial number for daughter 2.
    void setDaughter2GlobalTrackSN(int);
    /// @brief Set the primary track serial number for daughter 2.
    void setDaughter2PrimaryTrackSN(int);
    /// @brief Set the TOF hit serial number for daughter 2.
    void setDaughter2TofHitSN(int);
    /// @brief Set the EMC tower serial number for daughter 2.
    void setDaughter2EmcTowerSN(int);
    /// @brief Set the BEMC φ match difference for daughter 2 (rad).
    void setDaughter2BEmcMatchPhiDiff(double);
    /// @brief Set the BEMC z-edge match distance for daughter 2 (cm).
    void setDaughter2BEmcMatchZEdge(double);
    /// @brief Set the projected BTOF channel for daughter 2.
    void setDaughter2BTofProjChannel(float);
    /// @brief Set the BTOF cell local y for daughter 2 (cm).
    void setDaughter2BTofCellLocalY(float);
    /// @brief Set the BTOF cell local z for daughter 2 (cm).
    void setDaughter2BTofCellLocalZ(float);
    /// @brief Set the BTOF path length for daughter 2 (cm).
    void setDaughter2BTofPathLength(float);
    /// @brief Set the velocity β for daughter 2.
    void setDaughter2Beta(float);
    /// @brief Set the time-of-flight for daughter 2 (ns).
    void setDaughter2Tof(float);
    
private:
    
    float mInvariantMass;  ///< Invariant mass of the di-electron pair (GeV/c²).
    float mPt;             ///< Transverse momentum of the pair (GeV/c).
    float mPsi;            ///< Azimuthal angle ψ of the pair momentum (rad).
    float mTanl;           ///< Tangent of the dip angle of the pair momentum.

    StHltTrack mDaughter1PrimaryTrack;         ///< Primary track of daughter 1.
    StHltTrack mDaughter1GlobalTrack;          ///< Global track of daughter 1.
    StHltBTofHit mDaughter1BTofHit;            ///< BTOF hit matched to daughter 1.
    StHltBEmcTowerHit mDaughter1BEmcTowerHit;  ///< BEMC tower hit matched to daughter 1.
    double mDaughter1BEmcMatchPhiDiff;         ///< φ difference for BEMC match of daughter 1 (rad).
    double mDaughter1BEmcMatchZEdge;           ///< z-edge distance for BEMC match of daughter 1 (cm).
    int mDaughter1SelectionBit;                ///< Selection bit flags for daughter 1.
    int mDaughter1GlobalTrackSN;               ///< Serial number of the global track of daughter 1.
    int mDaughter1PrimaryTrackSN;              ///< Serial number of the primary track of daughter 1.
    int mDaughter1TofHitSN;                    ///< Serial number of the TOF hit of daughter 1.
    int mDaughter1EmcTowerSN;                  ///< Serial number of the EMC tower hit of daughter 1.
    float mDaughter1BTofProjChannel;           ///< Projected BTOF channel for daughter 1.
    float mDaughter1BTofCellLocalY;            ///< Local y in BTOF cell for daughter 1 (cm).
    float mDaughter1BTofCellLocalZ;            ///< Local z in BTOF cell for daughter 1 (cm).
    float mDaughter1BTofPathLength;            ///< Track path length to BTOF hit for daughter 1 (cm).
    float mDaughter1Beta;                      ///< Velocity β = v/c for daughter 1.
    float mDaughter1Tof;                       ///< Time-of-flight for daughter 1 (ns).

    StHltTrack mDaughter2PrimaryTrack;         ///< Primary track of daughter 2.
    StHltTrack mDaughter2GlobalTrack;          ///< Global track of daughter 2.
    StHltBTofHit mDaughter2BTofHit;            ///< BTOF hit matched to daughter 2.
    StHltBEmcTowerHit mDaughter2BEmcTowerHit;  ///< BEMC tower hit matched to daughter 2.
    double mDaughter2BEmcMatchPhiDiff;         ///< φ difference for BEMC match of daughter 2 (rad).
    double mDaughter2BEmcMatchZEdge;           ///< z-edge distance for BEMC match of daughter 2 (cm).
    int mDaughter2SelectionBit;                ///< Selection bit flags for daughter 2.
    int mDaughter2GlobalTrackSN;               ///< Serial number of the global track of daughter 2.
    int mDaughter2PrimaryTrackSN;              ///< Serial number of the primary track of daughter 2.
    int mDaughter2TofHitSN;                    ///< Serial number of the TOF hit of daughter 2.
    int mDaughter2EmcTowerSN;                  ///< Serial number of the EMC tower hit of daughter 2.
    float mDaughter2BTofProjChannel;           ///< Projected BTOF channel for daughter 2.
    float mDaughter2BTofCellLocalY;            ///< Local y in BTOF cell for daughter 2 (cm).
    float mDaughter2BTofCellLocalZ;            ///< Local z in BTOF cell for daughter 2 (cm).
    float mDaughter2BTofPathLength;            ///< Track path length to BTOF hit for daughter 2 (cm).
    float mDaughter2Beta;                      ///< Velocity β = v/c for daughter 2.
    float mDaughter2Tof;                       ///< Time-of-flight for daughter 2 (ns).
    
    ClassDef(StHltDiElectron,1);
    
};

inline float StHltDiElectron::invariantMass() const {return mInvariantMass;}
inline float StHltDiElectron::pt() const {return mPt; }
inline float StHltDiElectron::psi() const {return mPsi;}
inline float StHltDiElectron::tanl() const {return mTanl;}


ostream& operator<<(ostream&, const StHltDiElectron&);///< print operator

#endif




