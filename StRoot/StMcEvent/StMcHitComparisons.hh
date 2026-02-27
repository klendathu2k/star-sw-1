/**********************************************
 *
 * $Id: StMcHitComparisons.hh,v 2.5 2005/11/22 21:44:52 fisyak Exp $
 *
 * Author: Manuel Calderon de la Barca Sanchez
 ***********************************************
 * Description: Define the comparisons to be used
 *              in the multimaps
 *              & sorting of containers
 *
 ***********************************************
 * $Log: StMcHitComparisons.hh,v $
 * Revision 2.5  2005/11/22 21:44:52  fisyak
 * Add compress Print for McEvent, add Ssd collections
 *
 * Revision 2.4  2005/07/06 21:49:51  calderon
 * StThreeVector.hh -> StThreeVectorF.hh
 *
 * Revision 2.3  2005/07/06 20:05:28  calderon
 * Remove forward declaration of StThreeVectorF, use #include, and only in
 * StMcHit base class.  StThreeVectorF is not a class anymore, it is now
 * only a typedef, only template version of StThreeVector exists now.
 *
 * Revision 2.2  2000/06/09 19:52:07  calderon
 * No longer use 2 different functions for SVT and TPC that do the same thing,
 * just use one function for the base class
 *
 * Revision 2.1  2000/03/07 15:09:54  calderon
 * Initial Revision.
 * Comparisons used for sorting the hit containers, and
 * for ordering the hits in the multimaps.
 * 
 * 
 **********************************************/
/// @file StMcHitComparisons.hh
/// @brief Functor structs for ordering MC and reconstructed hit pointers in sorted containers.
#ifndef StMcHitComparisons_HH
#define StMcHitComparisons_HH
class StHit;
class StTpcHit;
class StSvtHit;
class StSsdHit;
class StFtpcHit;
class StRichHit;

class StMcHit;
class StMcTpcHit;
class StMcSvtHit;
class StMcSsdHit;
class StMcFtpcHit;
class StMcRichHit;
#include "StThreeVectorF.hh"

#if (defined __SUNPRO_CC && (__SUNPRO_CC < 0x500))
// bool is defined in utility for SUNPRO_CC 4.2
#include <utility> 
#endif
/// @brief Comparison functor for ordering reconstructed StHit pointers (by position).
struct compHit{
    bool operator()(const StHit*, const StHit*) const;
};

/// @brief Comparison functor for ordering StMcHit pointers (by position).
struct compMcHit{
    bool operator()(const StMcHit*, const StMcHit*) const;
};

// struct compTpcHit{
//     bool operator()(const StTpcHit* h1,const StTpcHit* h2) const;
// };

// struct compMcTpcHit{
//     bool operator()(const StMcTpcHit* h1,const StMcTpcHit* h2) const;
// };

// struct compSvtHit{
//     bool operator()(const StSvtHit* h1,const StSvtHit* h2) const;
// };

// struct compMcSvtHit{
//     bool operator()(const StMcSvtHit* h1,const StMcSvtHit* h2) const;
// };

/// @brief Comparison functor ordering 3-vectors by (r, phi) — used for FTPC hit sorting.
struct compRPhi{
    bool operator()(const StThreeVectorF&, const StThreeVectorF&) const;
};
/// @brief Comparison functor for ordering reconstructed StFtpcHit pointers.
struct compFtpcHit{
    bool operator()(const StFtpcHit*,const StFtpcHit*) const;
};
/// @brief Comparison functor for ordering StMcFtpcHit pointers.
struct compMcFtpcHit{
    bool operator()(const StMcFtpcHit*,const StMcFtpcHit*) const;
};

#endif
