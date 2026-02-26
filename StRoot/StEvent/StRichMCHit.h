/*!
 * \class StRichMCHit 
 * \author Brian Lasiuk, May 2000
 */
/***************************************************************************
 *
 * $Id: StRichMCHit.h,v 2.5 2004/07/15 16:36:25 ullrich Exp $
 *
 * Author: Brian Lasiuk, May 2000
 ***************************************************************************
 *
 * Description: Definition of the persistent MC Hit object
 *
 ***************************************************************************
 *
 * $Log: StRichMCHit.h,v $
 * Revision 2.5  2004/07/15 16:36:25  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.4  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:40  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2001/03/24 03:34:55  perev
 * clone() -> clone() const
 *
 * Revision 2.1  2000/05/22 21:44:32  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRichMCHit_hh
#define StRichMCHit_hh

/// @file StRichMCHit.h
/// @brief Persistent Monte Carlo RICH hit, extending StRichHit with simulation truth information.

#include "StRichHit.h"
#include "StRichMCInfo.h"

/// @brief Monte Carlo RICH hit, pairing a reconstructed hit with its generator-level truth record.
class StRichMCHit : public StRichHit {
public:
    /// @brief Default constructor.
    StRichMCHit();
    /// @brief Constructor with global position and position error.
    StRichMCHit(const StThreeVectorF& xg, const StThreeVectorF& dx);
    /// @brief Constructor with position, error, hardware position, charge, max ADC, and track counter.
    StRichMCHit(const StThreeVectorF& xg, const StThreeVectorF& dx,
              unsigned int hp, float q, float maxAdc, unsigned char tc);
    /// @brief Full constructor including Monte Carlo truth information.
    StRichMCHit(const StThreeVectorF& xg, const StThreeVectorF& dx,
                unsigned int hp, float q, float maxAdc, unsigned char tc,
                StRichMCInfo& info);

    ~StRichMCHit();

    //StRichMCHit(const StRichMCHit&){}
    //StRichMCHit& operator=(const StRichMCHit&){}

    /// @brief Sets the Monte Carlo truth record for this hit.
    void setMCInfo(const StRichMCInfo&);
    /// @brief Returns the Monte Carlo truth record for this hit.
    const StRichMCInfo& getMCInfo() const;

protected:
    StRichMCInfo  mInfo;  ///< Monte Carlo truth information for this hit

    ClassDef(StRichMCHit,1)
};

inline const StRichMCInfo& StRichMCHit::getMCInfo() const { return mInfo; }
inline void StRichMCHit::setMCInfo(const StRichMCInfo& info) { mInfo = info;}
#endif
