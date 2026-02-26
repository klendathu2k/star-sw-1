/*!
 * \class StTofCell 
 * \author F. Geurts, May 2003
 */
/***************************************************************************
 *
 * $Id: StTofCell.h,v 2.8 2008/03/31 20:09:35 ullrich Exp $
 *
 * Author: F. Geurts, May 2003
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTofCell.h,v $
 * Revision 2.8  2008/03/31 20:09:35  ullrich
 * Changed typr of mLeadingEdgeTime and mTrailingEdgeTime from float to double
 *
 * Revision 2.7  2007/09/19 17:32:18  ullrich
 * New member (mLeadingEdgeTime,  mTrailingEdgeTime) and related functions and updates added.
 *
 * Revision 2.6  2004/06/14 23:54:38  jeromel
 * Corrected typo
 *
 * Revision 2.5  2004/06/11 19:36:48  ullrich
 * Added implementation of zHit().
 *
 * Revision 2.4  2004/02/05 17:59:31  ullrich
 * Changed $LINK to StLink mechanism and add new member.
 *
 * Revision 2.3  2003/08/28 23:24:17  jeromel
 * Modif in class
 *
 * Revision 2.2  2003/08/05 17:12:32  ullrich
 * Added position() methods and member.
 *
 * Revision 2.1  2003/05/21 18:24:20  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTofCell_hh
#define StTofCell_hh

/// @file StTofCell.h
/// @brief Legacy STAR Time of Flight (TOF) detector cell matched to a track.

#include "StObject.h"
#include "StThreeVectorD.hh"
#include "StContainers.h"

class StTrack;

/// @brief A matched cell in the legacy STAR Time of Flight (TOF) detector.
class StTofCell : public StObject {
public:
    /// @brief Default constructor.
    StTofCell();
    /// @brief Constructs with full geometry, electronic, and track information.
    StTofCell(int, int, int, int, int, int, StTrack*,
	    float, int, const StThreeVectorD&);
    /// @brief Constructs with condensed geometry, electronic, and track information.
    StTofCell(int, int, int, int, StTrack*,
	    float, int, const StThreeVectorD&);
    /// @brief Destructor.
    ~StTofCell();
    
    /// @brief Equality comparison operator.
    int operator==(const StTofCell&) const;
    /// @brief Inequality comparison operator.
    int operator!=(const StTofCell&) const;
    
    /// @brief Returns the TOF tray index.
    int                   trayIndex() const;
    /// @brief Returns the module index within the tray.
    int                   moduleIndex() const;
    /// @brief Returns the cell index within the module.
    int                   cellIndex() const;
    /// @brief Returns the DAQ channel index.
    int                   daqIndex() const;
    /// @brief Returns the raw ADC value.
    int                   adc() const;
    /// @brief Returns the raw TDC value.
    int                   tdc() const;
    /// @brief Returns the leading-edge time [ns].
    double                leadingEdgeTime() const;
    /// @brief Returns the trailing-edge time [ns].
    double                trailingEdgeTime() const;
    /// @brief Returns the time-over-threshold (trailing minus leading edge time) [ns].
    float                 tot() const;
    /// @brief Returns a pointer to the associated reconstructed track.
    StTrack*              associatedTrack();
    /// @brief Returns a const pointer to the associated reconstructed track.
    const StTrack*        associatedTrack() const;
    /// @brief Returns the z-coordinate of the hit position [cm].
    float                 zHit() const;
    /// @brief Returns the track-to-cell matching quality flag.
    int                   matchFlag() const;
    /// @brief Returns the 3D hit position [cm].
    const StThreeVectorD& position() const;
    
    /// @brief Sets the TOF tray index.
    void      setTrayIndex(int);
    /// @brief Sets the module index within the tray.
    void      setModuleIndex(int);
    /// @brief Sets the cell index within the module.
    void      setCellIndex(int);
    /// @brief Sets the DAQ channel index.
    void      setDaqIndex(int);
    /// @brief Sets the raw ADC value.
    void      setAdc(int);
    /// @brief Sets the raw TDC value.
    void      setTdc(int);
    /// @brief Sets the leading-edge time [ns].
    void      setLeadingEdgeTime(double);
    /// @brief Sets the trailing-edge time [ns].
    void      setTrailingEdgeTime(double);
    /// @brief Sets the pointer to the associated reconstructed track.
    void      setAssociatedTrack(StTrack*);
    /// @brief Sets the z-coordinate of the hit position [cm].
    void      setZHit(float);
    /// @brief Sets the track-to-cell matching quality flag.
    void      setMatchFlag(int);
    /// @brief Sets the 3D hit position [cm].
    void      setPosition(const StThreeVectorD&);

protected:
    Int_t    mTrayIndex;       ///< TOF tray index
    Int_t    mModuleIndex;     ///< Module index within tray
    Int_t    mCellIndex;       ///< Cell index within module
    Int_t    mDaqIndex;        ///< DAQ channel index
    Int_t    mAdc;             ///< Raw ADC value
    Int_t    mTdc;             ///< Raw TDC value
    Double_t mLeadingEdgeTime;   ///< Leading-edge time [ns]
    Double_t mTrailingEdgeTime;  ///< Trailing-edge time [ns]
    //    StTrack* mAssociatedTrack;   //$LINK
#ifdef __CINT__
    StObjLink        mAssociatedTrack;		
#else
    StLink<StTrack>  mAssociatedTrack;		
#endif //__CINT__
    Float_t  mZhit;            ///< Z coordinate of the hit position [cm]
    Int_t    mMatchFlag;       ///< Track-to-cell matching quality flag
    StThreeVectorD mPosition;  ///< 3D hit position [cm]

    ClassDef(StTofCell,4)
};


inline void
StTofCell::setTrayIndex(int trayId)
{
    mTrayIndex = trayId;
}

inline void
StTofCell::setModuleIndex(int moduleId)
{
    mModuleIndex = moduleId;
}

inline void
StTofCell::setCellIndex(int cellId)
{
    mCellIndex = cellId;
}

inline void
StTofCell::setDaqIndex(int daqId)
{
    mDaqIndex = daqId;
}

inline void
StTofCell::setAdc(int rawAdc)
{
    mAdc = rawAdc;
}

inline void
StTofCell::setTdc(int rawTdc)
{
    mTdc = rawTdc;
}

inline void
StTofCell::setLeadingEdgeTime(double val)
{
    mLeadingEdgeTime = val;
}

inline void
StTofCell::setTrailingEdgeTime(double val)
{
    mTrailingEdgeTime = val;
}

inline void
StTofCell::setZHit(float zhit) {mZhit = zhit;}

inline void
StTofCell::setMatchFlag(int flag)
{
  mMatchFlag = flag;
}

inline int
StTofCell::trayIndex() const
{
    return mTrayIndex;
}

inline int
StTofCell::moduleIndex() const
{
    return mModuleIndex;
}

inline int
StTofCell::cellIndex() const
{
    return mCellIndex;
}

inline int
StTofCell::daqIndex() const
{
    return mDaqIndex;
}

inline int
StTofCell::adc()  const
{
    return mAdc;
}

inline int
StTofCell::tdc()  const
{
    return mTdc;
}

inline double 
StTofCell::leadingEdgeTime()  const
{
    return mLeadingEdgeTime;
}

inline double 
StTofCell::trailingEdgeTime()  const
{
    return mTrailingEdgeTime;
}

inline float
StTofCell::tot()  const
{
    return mTrailingEdgeTime - mLeadingEdgeTime;
}

inline int
StTofCell::matchFlag() const
{
  return mMatchFlag;
}

inline float
StTofCell::zHit() const
{
  return mZhit;
}

#endif
