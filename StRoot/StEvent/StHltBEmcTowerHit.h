/***************************************************************************
 *
 * $Id: StHltBEmcTowerHit.h,v 2.1 2011/02/01 19:45:47 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltBEmcTowerHit.h,v $
 * Revision 2.1  2011/02/01 19:45:47  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltBEmcTowerHit_hh
#define StHltBEmcTowerHit_hh

/// @file StHltBEmcTowerHit.h
/// @brief HLT Barrel Electromagnetic Calorimeter (BEMC) tower hit.

#include <Stiostream.h>
#include "StObject.h"
#include "StArray.h"

class StHltTrackNode;

/// @brief HLT BEMC tower hit with geometry, energy deposit, and ADC information.
class StHltBEmcTowerHit : public StObject {
public:
    StHltBEmcTowerHit();
    ~StHltBEmcTowerHit();

    /// @brief Raw ADC value of the tower.
    int adc() const;
    /// @brief Calibrated energy deposit in the tower (GeV).
    float energy() const;
    /// @brief Azimuthal angle φ of the tower center (rad).
    float phi() const;
    /// @brief Pseudorapidity η of the tower center.
    float eta() const;
    /// @brief z coordinate of the tower center (cm).
    float z() const;
    /// @brief Software identifier of the tower.
    float softId() const;
    /// @brief DAQ identifier of the tower.
    float daqId() const;

    /// @brief Associated HLT track node.
    StHltTrackNode* trackNode();
    const StHltTrackNode* trackNode() const;

    /// @brief Set the raw ADC value.
    void setAdc(int);
    /// @brief Set the calibrated energy deposit (GeV).
    void setEnergy(float);
    /// @brief Set the azimuthal angle φ (rad).
    void setPhi(float);
    /// @brief Set the pseudorapidity η.
    void setEta(float);
    /// @brief Set the z coordinate (cm).
    void setZ(float);
    /// @brief Set the software tower identifier.
    void setSoftId(int);
    /// @brief Set the DAQ tower identifier.
    void setDaqId(int);

    /// @brief Set the associated track node.
    void setTrackNode(StHltTrackNode*);
    
private:
    int mAdc;              ///< Raw ADC count of the tower.
    float mEnergy;         ///< Calibrated energy deposit (GeV), using online calibration.
    float mPhi;            ///< Azimuthal angle φ of tower center (rad).
    float mEta;            ///< Pseudorapidity η of tower center.
    float mZ;              ///< z coordinate of tower center (cm).
    int mSoftId;           ///< Software tower identifier.
    int mDaqId;            ///< DAQ tower identifier.
    
#ifdef __CINT__
    StObjLink mTrackNode;
#else
    StLink<StHltTrackNode> mTrackNode;
#endif //__CINT__
    
    ClassDef(StHltBEmcTowerHit,1)
};

inline int StHltBEmcTowerHit::adc() const {return mAdc;}
inline float StHltBEmcTowerHit::energy() const {return mEnergy;}
inline float StHltBEmcTowerHit::phi() const {return mPhi;}
inline float StHltBEmcTowerHit::eta() const {return mEta;}
inline float StHltBEmcTowerHit::z() const {return mZ;}
inline float StHltBEmcTowerHit::softId() const {return mSoftId;}
inline float StHltBEmcTowerHit::daqId() const {return mDaqId;}

inline StHltTrackNode* StHltBEmcTowerHit::trackNode() {return mTrackNode;}
inline const StHltTrackNode* StHltBEmcTowerHit::trackNode() const {return mTrackNode;}

ostream& operator<<(ostream&, const StHltBEmcTowerHit&); ///< Printting operator

#endif


