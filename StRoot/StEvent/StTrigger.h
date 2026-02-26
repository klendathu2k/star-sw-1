/*!
 * \class StTrigger
 * \author Thomas Ullrich, Sep 1999
 * \brief Base class representing a STAR trigger with action word and trigger word.
 */
/***************************************************************************
 *
 * $Id: StTrigger.h,v 2.3 2002/02/22 22:56:53 jeromel Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTrigger.h,v $
 * Revision 2.3  2002/02/22 22:56:53  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/08/29 18:53:37  ullrich
 * Changed trigger words to UInt_t (was UShort_t)
 *
 * Revision 2.1  2001/04/05 04:00:46  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.0  1999/10/12 18:43:13  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StTrigger_hh
#define StTrigger_hh

/// @file StTrigger.h
/// @brief Base class for STAR trigger information (trigger word and action word).

#include "StObject.h"

/// @brief Base class for STAR trigger information storing the trigger word and action word.
class StTrigger : public StObject {
public:
    StTrigger();
    /// @brief Construct with explicit action word @p aw and trigger word @p w.
    StTrigger(unsigned int aw, unsigned int w);
    // StTrigger(const StTrigger&);             use default
    // StTrigger& operator=(const StTrigger&);  use default
    virtual ~StTrigger();

    int operator==(const StTrigger&) const;
    int operator!=(const StTrigger&) const;

    /// @brief Return the trigger action word (detector/algorithm decision bits).
    virtual unsigned int triggerActionWord() const;
    /// @brief Return the trigger word (L0 hardware trigger bits).
    virtual unsigned int triggerWord() const;

    /// @brief Set the trigger action word.
    virtual void setTriggerActionWord(unsigned int);
    /// @brief Set the trigger word.
    virtual void setTriggerWord(unsigned int);
    
protected:
    UInt_t mTriggerActionWord; ///< Trigger action word (detector/algorithm decision bits).
    UInt_t mTriggerWord;       ///< L0 hardware trigger word.
    
    ClassDef(StTrigger,2)  //StTrigger structure
};
#endif
