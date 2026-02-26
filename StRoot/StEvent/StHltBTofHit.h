/***************************************************************************
 *
 * $Id: StHltBTofHit.h,v 2.1 2011/02/01 19:45:47 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltBTofHit.h,v $
 * Revision 2.1  2011/02/01 19:45:47  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltBTofHit_hh
#define StHltBTofHit_hh

/// @file StHltBTofHit.h
/// @brief HLT Barrel Time-of-Flight (BTOF) detector hit.

#include <Stiostream.h>
#include "StObject.h"
#include "StArray.h"

class StHltTrackNode;

/// @brief HLT BTOF hit with timing, time-over-threshold, and channel information.
class StHltBTofHit : public StObject {
public:
    StHltBTofHit();
    ~StHltBTofHit();

    /// @brief BTOF tray identifier.
    short trayId() const;
    short channel() const; ///< Encoded channel index: module * 6 + cell.
    /// @brief Raw TDC time (ns).
    float tdc() const;
    /// @brief Time-over-threshold (ns), used for slewing correction.
    float tot() const;
    /// @brief Calibrated time-of-flight (ns).
    float tof() const;
    /// @brief Reference trigger time (ns).
    float triggerTime() const;
    /// @brief Module number derived from channel.
    short module() const;
    /// @brief Cell number within module derived from channel.
    short cell() const;

    /// @brief Associated HLT track node.
    StHltTrackNode* trackNode();
    const StHltTrackNode* trackNode() const;

    /// @brief Set the tray identifier.
    void setTrayId(short);
    /// @brief Set the encoded channel index (module * 6 + cell).
    void setChannel(short);
    /// @brief Set the raw TDC time (ns).
    void setTdc(float);
    /// @brief Set the time-over-threshold (ns).
    void setTot(float);
    /// @brief Set the calibrated time-of-flight (ns).
    void setTof(float);
    /// @brief Set the reference trigger time (ns).
    void setTriggerTime(float);

    /// @brief Set the associated track node.
    void setTrackNode(StHltTrackNode*);
    
private:
    short mTrayId;       ///< BTOF tray identifier.
    short mChannel;      ///< Encoded channel index: nModule * 6 + nCell.
    float mTdc;          ///< Raw TDC time (ns).
    float mTot;          ///< Time-over-threshold (ns).
    float mTof;          ///< Calibrated time-of-flight (ns).
    float mTriggerTime;  ///< Reference trigger time (ns).
    
#ifdef __CINT__
    StObjLink mTrackNode;
#else
    StLink<StHltTrackNode> mTrackNode;
#endif //__CINT__
    
    ClassDef(StHltBTofHit,1)
};

inline short StHltBTofHit::trayId() const {return mTrayId;}
inline short StHltBTofHit::channel() const {return mChannel;}
inline float StHltBTofHit::tdc() const {return mTdc;}
inline float StHltBTofHit::tot() const {return mTot;}
inline float StHltBTofHit::tof() const {return mTof;}
inline float StHltBTofHit::triggerTime() const {return mTriggerTime;}
inline short StHltBTofHit::module() const {return mChannel/6;}
inline short StHltBTofHit::cell() const {return mChannel%6;}

inline StHltTrackNode* StHltBTofHit::trackNode() {return mTrackNode;}
inline const StHltTrackNode* StHltBTofHit::trackNode() const {return mTrackNode;}

ostream& operator<<(ostream&, const StHltBTofHit&); ///< Printting operator

#endif

