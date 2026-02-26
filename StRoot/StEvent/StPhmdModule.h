/*!
 * \class StPhmdModule
 * \author Subhasis Chattopadhyay
 */
/********************************************************************
 *
 * $Id: StPhmdModule.h,v 2.1 2002/12/20 22:33:00 ullrich Exp $
 *
 * Author: Subhasis Chattopadhyay, Dec 2002
 ********************************************************************
 *
 * Description: This is the class for each supermodule 
 *              having the hit information.
 *
 ********************************************************************
 *
 * $Log: StPhmdModule.h,v $
 * Revision 2.1  2002/12/20 22:33:00  ullrich
 * Initial Revision.
 *
 ********************************************************************/
#ifndef StPhmdModule_hh
#define StPhmdModule_hh

/// @file StPhmdModule.h
/// @brief Supermodule of the STAR Photon Multiplicity Detector (PMD/PHMD), holding cell hits.

#include "StObject.h"
#include "StContainers.h"
#include "StPhmdHit.h"

/// @brief Represents one PMD supermodule, containing the collection of cell hits within it.
class StPhmdModule : public StObject {
public:
    StPhmdModule();          
    ~StPhmdModule();         
 
    /// @brief Returns the number of hits recorded in this supermodule.
    unsigned int            numberOfHits() const;
    /// @brief Returns a reference to the vector of hits in this supermodule.
    StSPtrVecPhmdHit&       hits();
    /// @brief Returns a const reference to the vector of hits in this supermodule.
    const StSPtrVecPhmdHit& hits() const;    
    
private:
    StSPtrVecPhmdHit mHits;   ///< Collection of PMD cell hits within this supermodule.
    ClassDef(StPhmdModule,1)
};
#endif









