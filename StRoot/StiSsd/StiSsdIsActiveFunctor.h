// $Id: StiSsdIsActiveFunctor.h,v 1.2 2005/06/21 15:31:48 lmartin Exp $
// 
// $Log: StiSsdIsActiveFunctor.h,v $
// Revision 1.2  2005/06/21 15:31:48  lmartin
// CVS tags added
//
/**
 * file StiSsdIsActiveFunctor.h
 * class StiSsdIsActiveFunctor
 * brief function object for determine a SSD ladder's active regions
 */

/// @file StiSsdIsActiveFunctor.h
/// @brief SSD acceptance functor.
///
/// @ingroup StiDetectorGeometry
#ifndef STI_SSD_IS_ACTIVE_FUNCTOR
#define STI_SSD_IS_ACTIVE_FUNCTOR

#include "Sti/StiIsActiveFunctor.h"

/// @class StiSsdIsActiveFunctor
/// @brief SSD acceptance functor returning true if a position is within an active SSD strip.
///
/// Returns true if the (y,z) position falls within the active region of an SSD ladder.
/// @ingroup StiDetectorGeometry
struct StiSsdIsActiveFunctor : public StiIsActiveFunctor
{
    StiSsdIsActiveFunctor();
    virtual ~StiSsdIsActiveFunctor();
    virtual bool operator()(double dYlocal, double dZlocal) const;
};

#endif // defined STI_SSD_IS_ACTIVE_FUNCTOR
