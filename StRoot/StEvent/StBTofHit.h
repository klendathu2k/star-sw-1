/*!
 * \class StBTofHit
 * \author Xin Dong, Nov 2008
 */
/***************************************************************************
 *
 * $Id: StBTofHit.h,v 2.5 2016/02/25 17:10:19 ullrich Exp $
 *
 * Author: Xin Dong, Nov 2008
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StBTofHit.h,v $
 * Revision 2.5  2016/02/25 17:10:19  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.4  2012/05/07 14:42:57  fisyak
 * Add handilings for Track to Fast Detectors Matching
 *
 * Revision 2.3  2009/02/13 22:29:03  ullrich
 * Fixed typo in ostream<< operator.
 *
 * Revision 2.2  2009/01/15 00:46:26  ullrich
 * tray() now returns int.
 *
 * Revision 2.1  2008/12/22 20:30:58  ullrich
 * Initial Revision.
 *
 *
 **************************************************************************/
#ifndef StBTofHit_hh
#define StBTofHit_hh

/// @file StBTofHit.h
/// @brief Reconstructed hit in the STAR Barrel Time of Flight (BTOF) detector.

#include <Stiostream.h>
#include "StHit.h"
#include "StContainers.h"

class StTrack;

/// @brief Reconstructed hit in the STAR Barrel Time of Flight (BTOF) detector.
class StBTofHit : public StHit {
public:
    enum {
        kNTray   = 120,  //! 120 TOF trays
        kNModule =  32,  //! 32 modules per tray
        kNCell     = 6   //! 6 cells per module
    };
    /// @brief Default constructor.
    StBTofHit();
    /// @brief Destructor.
    ~StBTofHit() {}
    /// @brief Returns the tray number (1–120).
    Int_t             tray()             const { return mTray; }
    /// @brief Returns the module number within the tray (1–32).
    Int_t             module()           const { return mModule; }
    /// @brief Returns the cell number within the module (1–6).
    Int_t             cell()             const { return mCell; }
    /// @brief Returns a unique module ID across all trays.
    Int_t             ID()               const { return kNModule*(tray()-1) + module() - 1;}
    /// @brief Returns the leading-edge time [ns].
    Double_t          leadingEdgeTime()  const { return mLeadingEdgeTime; }
    /// @brief Returns the trailing-edge time [ns].
    Double_t          trailingEdgeTime() const { return mTrailingEdgeTime; }
    /// @brief Returns the time-over-threshold (trailing minus leading edge time) [ns].
    Double_t          tot()              const { return mTrailingEdgeTime - mLeadingEdgeTime; }
    /// @brief Returns a pointer to the associated reconstructed track.
    StTrack*          associatedTrack();
    /// @brief Returns a const pointer to the associated reconstructed track.
    const StTrack*    associatedTrack() const;
    /// @brief Sets the tray number.
    void setTray(UChar_t trayId)            { mTray = trayId; }
    /// @brief Sets the module number within the tray.
    void setModule(UChar_t moduleId)        { mModule = moduleId; }
    /// @brief Sets the cell number within the module.
    void setCell(UChar_t cellId)            { mCell = cellId; }
    /// @brief Sets the leading-edge time [ns].
    void setLeadingEdgeTime(Double_t time)  { mLeadingEdgeTime = time; }
    /// @brief Sets the trailing-edge time [ns].
    void setTrailingEdgeTime(Double_t time) { mTrailingEdgeTime = time; }
    /// @brief Sets the pointer to the associated reconstructed track.
    void setAssociatedTrack(StTrack*);
    /// @brief Returns the 3D hit position [cm].
    const StThreeVectorF& position() const;
    /// @brief Returns the pad width of the BTOF detector [cm].
    static Float_t    padWidth()            { return mBTofPadWidth;}
    /// @brief Returns the detector identifier for this hit.
    StDetectorId   detector() const;

protected:
    UChar_t   mTray;               ///< Tray number (1–120)
    UChar_t   mModule;             ///< Module number within tray (1–32)
    UChar_t   mCell;               ///< Cell number within module (1–6)
    Double_t  mLeadingEdgeTime;    ///< Leading-edge time [ns]
    Double_t  mTrailingEdgeTime;   ///< Trailing-edge time [ns]
    const static Float_t mBTofPadWidth;  ///< Pad width of the BTOF detector [cm]
    //    StTrack *mAssociatedTrack;   //$LINK
#ifdef __CINT__
    StObjLink        mAssociatedTrack;
#else
    StLink<StTrack>  mAssociatedTrack;
#endif //__CINT__
    ClassDef(StBTofHit,2)
};

inline StDetectorId StBTofHit::detector() const {return kBTofId;}

ostream& operator<<(ostream&, const StBTofHit&); // Printing operator
#endif
