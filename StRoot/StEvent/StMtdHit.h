/*!
 * \class StMtdHit 
 */
/***************************************************************************
 *
 * $Id: StMtdHit.h,v 2.3 2018/03/15 22:00:34 smirnovd Exp $
 *
 * Author: Frank Geurts, April 25, 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StMtdHit.h,v $
 * Revision 2.3  2018/03/15 22:00:34  smirnovd
 * Fix linker error by removing declared but undefined functions
 *
 * Revision 2.2  2015/10/09 17:46:14  ullrich
 * Changed type of mIdTruth from ushort to int.
 *
 * Revision 2.1  2011/04/25 21:24:02  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StMtdHit_hh
#define StMtdHit_hh

/// @file StMtdHit.h
/// @brief Reconstructed hit in the STAR Muon Telescope Detector (MTD).

#include <Stiostream.h>
#include "StObject.h"
#include "StContainers.h"

class StTrack;

/// @brief Reconstructed hit in the STAR Muon Telescope Detector (MTD).
class StMtdHit : public StObject {
public:
    /// @brief Default constructor.
    StMtdHit();
    /// @brief Destructor.
    ~StMtdHit();

    /// @brief Returns the backleg number (1–30).
    int             backleg() const;
    /// @brief Returns the module number within the backleg (1–5).
    int             module() const;
    /// @brief Returns the cell number within the module.
    int             cell() const;
    /// @brief Returns the leading-edge times for both readout ends [ns].
    pair<double,double>   leadingEdgeTime() const;
    /// @brief Returns the trailing-edge times for both readout ends [ns].
    pair<double,double>   trailingEdgeTime() const;
    /// @brief Returns the time-over-threshold for both readout ends [ns].
    pair<double,double>   tot() const;
    /// @brief Returns the calibrated time of flight [ns].
    double          tof() const;

    /// @brief Returns a pointer to the associated reconstructed track.
    StTrack*        associatedTrack();
    /// @brief Returns a const pointer to the associated reconstructed track.
    const StTrack*  associatedTrack() const;
    
    /// @brief Returns the Monte Carlo truth track ID.
    int             idTruth() const;
    /// @brief Returns the quality of the MC truth association (% of charge from mIdTruth).
    int             qaTruth() const;

    /// @brief Sets the backleg number.
    void setBackleg(unsigned char);
    /// @brief Sets the module number within the backleg.
    void setModule(unsigned char);
    /// @brief Sets the cell number within the module.
    void setCell(unsigned char);
    /// @brief Sets the leading-edge times for both readout ends [ns].
    void setLeadingEdgeTime(pair<double,double>);
    /// @brief Sets the trailing-edge times for both readout ends [ns].
    void setTrailingEdgeTime(pair<double,double>);
    /// @brief Sets the pointer to the associated reconstructed track.
    void setAssociatedTrack(StTrack*);
    /// @brief Sets the Monte Carlo truth track ID and quality.
    void setIdTruth(int idtru, int qatru=0);

 protected:
    UChar_t   mBackLeg;   ///< Backleg number (1–30)
    UChar_t   mModule;    ///< Module number within backleg (1–5)
    UChar_t   mCell;      ///< Cell number within module
    pair<Double_t,Double_t>  mLeadingEdgeTime;   ///< Leading-edge times for both readout ends [ns]
    pair<Double_t,Double_t>  mTrailingEdgeTime;  ///< Trailing-edge times for both readout ends [ns]
    //    StTrack *mAssociatedTrack;   //$LINK
#ifdef __CINT__
    StObjLink        mAssociatedTrack;		
#else
    StLink<StTrack>  mAssociatedTrack;		
#endif //__CINT__
    Int_t  mIdTruth;     ///< Monte Carlo truth track ID
    UShort_t  mQuality; ///< Quality of MC truth association (% of charge from mIdTruth)

    ClassDef(StMtdHit,2)
};

ostream& operator<<(ostream&, const StMtdHit&); // Printing operator

#endif
