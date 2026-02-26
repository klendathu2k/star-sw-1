/*!
 * \class StEtrHit 
 * \author Ming Shao, Jan 2012
 */
/***************************************************************************
 *
 * $Id: StEtrHit.h,v 2.2 2012/03/22 00:07:28 perev Exp $
 *
 * Author: Ming Shao, Jan 2012
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEtrHit.h,v $
 * Revision 2.2  2012/03/22 00:07:28  perev
 * Section added
 *
 * Revision 2.1  2012/01/24 03:06:12  perev
 * Add Etr
 *
 *
 * Revision 1.0  2012/01/05 Ming
 * Initial Version
 *
 **************************************************************************/
#ifndef StEtrHit_hh
#define StEtrHit_hh

/// @file StEtrHit.h
/// @brief Reconstructed hit class for the ETR (Endcap Tracking prototype) detector.

#include "StHit.h"
#include "StMemoryPool.hh"

class StEtrPoint;

/// @brief Reconstructed hit in the Endcap Tracking (ETR) prototype detector.
class StEtrHit : public StHit {
public:
    /// @brief Default constructor.
    StEtrHit();
    /// @brief Construct with hit position, geometry indices, and deposited charge.
    StEtrHit(const StThreeVectorF&  position,
              int sector, int layer, int section, float charge);
    /// @brief Destructor.
    ~StEtrHit();
    /// @brief Returns the detector identifier (kEtrId).
    StDetectorId detector() const {return kEtrId;}

    /// @brief Returns the pad section index (0–29).
    int section() const;
    /// @brief Returns the azimuthal sector index (0–11).
    int sector()  const;
    /// @brief Returns the radial layer index (0–2).
    int layer()   const;
protected:
    ClassDef(StEtrHit,1)
};
#endif
