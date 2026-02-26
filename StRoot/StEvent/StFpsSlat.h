/***************************************************************************
 *
 * $Id: StFpsSlat.h,v 2.2 2015/10/21 14:45:55 ullrich Exp $
 *
 * Author: Jingguo Ma, Akio Ogawa, Sep 2015
 ***************************************************************************
 *
 * Description: StFpsSlat is data for individual FPS slat 
 *
 ***************************************************************************
 *
 * $Log: StFpsSlat.h,v $
 * Revision 2.2  2015/10/21 14:45:55  ullrich
 * Moved 7 members out of the schema.
 *
 * Revision 2.1  2015/09/01 18:26:45  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StFpsSlat_hh
#define StFpsSlat_hh

/// @file StFpsSlat.h
/// @brief A single scintillator slat in the FPS (FPD Pre-Shower) detector.

#include "Stiostream.h"
#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"

/// @brief Data for a single FPS (FPD Pre-Shower) scintillator slat, including MIP signal and associated FMS photon points.
class StFpsSlat : public StObject {
public:
    /// @brief Default constructor.
    StFpsSlat();
    /// @brief Constructor specifying slat ID and MIP signal.
    StFpsSlat(int slatid, float mip);
    /// @brief Destructor.
    ~StFpsSlat();
    
    /// @brief Return the FPS slat ID.
    int slatId() const;
    /// @brief Return the MIP (minimum ionising particle) signal in this slat.
    float mip() const;
    
    /// @brief Set the slat ID.
    void setSlatId(int);
    /// @brief Set the MIP signal.
    void setMip(float);
    
    unsigned int  nPoint(int type) const;           ///< Return number of associated FMS points of the given proximity rank (0=closest, 1-3=2nd-4th, 4=any).
    StPtrVecFmsPoint& point(int type);              ///< Return FMS points of proximity rank @p type.
    const StPtrVecFmsPoint& point(int type) const; ///< Return FMS points of proximity rank @p type (const).
    /// @brief Add an FMS photon point at proximity rank @p type.
    void addPoint(StFmsPoint* point, int type);
    
    /// @brief Print slat properties.
    void print(int option=0) const;
    
protected:
    Int_t    mSlatId; //!< FPS slat ID (transient).
    Float_t  mMip;    //!< MIP signal in this slat (transient).
    UInt_t   mNPoint[kFpsNCandidate+1];   //!< Number of associated points per proximity rank (transient).
    StPtrVecFmsPoint mPoint1;    //!< FMS points with closest projected distance (transient).
    StPtrVecFmsPoint mPoint2;    //!< FMS points with 2nd closest projected distance (transient).
    StPtrVecFmsPoint mPoint3;    //!< FMS points with 3rd closest projected distance (transient).
    StPtrVecFmsPoint mPoint4;    //!< FMS points with 4th closest projected distance (transient).

    ClassDef(StFpsSlat,2)
};

inline int StFpsSlat::slatId() const {return mSlatId;}
inline float StFpsSlat::mip()  const {return mMip;}
inline void StFpsSlat::setSlatId(int slatid) {mSlatId=slatid;}
inline void StFpsSlat::setMip(float mip) {mMip=mip;} 
inline unsigned int StFpsSlat::nPoint(int type) const {return mNPoint[type];}

#endif
