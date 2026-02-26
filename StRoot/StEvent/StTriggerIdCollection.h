/***************************************************************************
 *
 * $Id: StTriggerIdCollection.h,v 2.3 2006/05/04 19:07:49 ullrich Exp $
 *
 * Author: Thomas Ullrich, January 2003
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTriggerIdCollection.h,v $
 * Revision 2.3  2006/05/04 19:07:49  ullrich
 * Added L3 trigger expansion.
 *
 * Revision 2.2  2003/02/19 16:35:05  jeromel
 * $LINK mechanism removed
 *
 * Revision 2.1  2003/01/30 18:14:15  ullrich
 * Initial Revision.
 *
 **************************************************************************/
/*!
 *                                                                     
 * \class  StTriggerIdCollection
 * \author Thomas Ullrich
 * \date   2003/01/30
 * \brief  Collection of trigger ids as stored in StEvent
 *
 * This class stores the trigger ids for every event in StEvent.
 * Most users should only access the nominal(); everything else
 * is for detailed understanding of the triggers.
 *
 */    
#ifndef StTriggerIdCollection_hh
#define StTriggerIdCollection_hh

/// @file StTriggerIdCollection.h
/// @brief Collection of trigger ID sets (L1, L2, L3, nominal) for a single event.

#include "StTriggerId.h"

class StTriggerIdCollection : public StObject {
public:
    StTriggerIdCollection();
    ~StTriggerIdCollection();

    const StTriggerId* nominal() const; ///< Most-used accessor: trigger IDs agreed upon by all trigger levels.
    /// @brief Return the L1 trigger ID set.
    const StTriggerId* l1() const;
    /// @brief Return the L2 trigger ID set.
    const StTriggerId* l2() const;
    /// @brief Return the L3 trigger ID set.
    const StTriggerId* l3() const;
    /// @brief Return the expanded L3 trigger ID set.
    const StTriggerId* l3Expanded() const;

    /// @brief Set the L1 trigger ID object (takes ownership).
    void setL1(StTriggerId*);
    /// @brief Set the L2 trigger ID object (takes ownership).
    void setL2(StTriggerId*);
    /// @brief Set the L3 trigger ID object (takes ownership).
    void setL3(StTriggerId*);
    /// @brief Set the expanded L3 trigger ID object (takes ownership).
    void setL3Expanded(StTriggerId*);
    /// @brief Set the nominal trigger ID object (takes ownership).
    void setNominal(StTriggerId*);
    
private:
    StTriggerId  *mL1TriggerId;         ///< L1 trigger ID set.
    StTriggerId  *mL2TriggerId;         ///< L2 trigger ID set.
    StTriggerId  *mL3TriggerId;         ///< L3 trigger ID set.
    StTriggerId  *mL3ExpandedTriggerId; ///< Expanded L3 trigger ID set.
    StTriggerId  *mNominalTriggerId;    ///< Nominal (consensus) trigger ID set.
    
    ClassDef(StTriggerIdCollection,2)
};

#endif 
