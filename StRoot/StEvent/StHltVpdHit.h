/***************************************************************************
 *
 * $Id: StHltVpdHit.h,v 2.1 2011/02/01 19:45:48 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltVpdHit.h,v $
 * Revision 2.1  2011/02/01 19:45:48  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltVpdHit_hh
#define StHltVpdHit_hh

/// @file StHltVpdHit.h
/// @brief HLT Vertex Position Detector (VPD) hit used for online vertex-z determination.

#include <Stiostream.h>
#include "StObject.h"
#include "StArray.h"
#include "StEnumerations.h"

/// @brief VPD hit recorded by the HLT, carrying timing and channel information for vertex-z reconstruction.
class StHltVpdHit : public StObject {
public:
    /// @brief Default constructor.
    StHltVpdHit();
    /// @brief Destructor.
    ~StHltVpdHit();
    
    /// @brief Beam direction of this hit (east = 0, west = 1).
    StBeamDirection direction() const;
    /// @brief Returns the VPD channel index (0–18, total 19 channels per side).
    short channel() const; 
    /// @brief Returns the raw TDC value (ns).
    float tdc() const;
    /// @brief Returns the time-over-threshold (TOT) value (ns).
    float tot() const;
    /// @brief Returns the calibrated time-of-flight (ns).
    float tof() const;
    /// @brief Returns the trigger time reference (ns).
    float triggerTime() const;
    /// @brief Returns the VPD module number (channel / 6).
    short module() const;
    /// @brief Returns the cell number within the module (channel % 6).
    short cell() const;
    
    
    /// @brief Sets the beam direction of this hit.
    void setDirection(StBeamDirection);
    /// @brief Sets the VPD channel index.
    void setChannel(short);
    /// @brief Sets the raw TDC value (ns).
    void setTdc(float);
    /// @brief Sets the time-over-threshold (TOT) value (ns).
    void setTot(float);
    /// @brief Sets the calibrated time-of-flight (ns).
    void setTof(float);
    /// @brief Sets the trigger time reference (ns).
    void setTriggerTime(float);
    
private:
    StBeamDirection mDirection;  ///< Beam direction: east (0) or west (1).
    short mChannel;              ///< VPD channel index (0–18 per side, 19 channels total).
    float mTdc;                  ///< Raw TDC value (ns).
    float mTot;                  ///< Time-over-threshold (ns).
    float mTof;                  ///< Calibrated time-of-flight (ns).
    float mTriggerTime;          ///< Trigger time reference (ns).
    
    ClassDef(StHltVpdHit,1)
};

inline StBeamDirection StHltVpdHit::direction() const {return mDirection;}
inline short StHltVpdHit::channel() const {return mChannel;}
inline float StHltVpdHit::tdc() const {return mTdc;}
inline float StHltVpdHit::tot() const {return mTot;}
inline float StHltVpdHit::tof() const {return mTof;}
inline float StHltVpdHit::triggerTime() const {return mTriggerTime;}
inline short StHltVpdHit::module() const {return mChannel/6;}
inline short StHltVpdHit::cell() const {return mChannel%6;}


ostream& operator<<(ostream&, const StHltVpdHit&); ///< Stream-insertion operator for printout.

#endif

