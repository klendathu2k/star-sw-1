/***************************************************************************
 *
 * $Id: StRpsRomanPot.h,v 2.1 2009/11/23 22:18:25 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StRpsRomanPot.h,v $
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRpsRomanPot_hh
#define StRpsRomanPot_hh

/// @file StRpsRomanPot.h
/// @brief A single Roman Pot detector station in the RPS.

#include "StObject.h"
#include "StContainers.h"
#include "StRpsPlane.h"

class StRpsCollection;

/// @brief Single Roman Pot (RP) detector station in the Roman Pot System.
///
/// Each Roman Pot houses four silicon strip detector planes and two PMT-based
/// scintillator detectors used for timing. Stores ADC and TAC values from the
/// PMTs and the array of silicon planes.
class StRpsRomanPot : public StObject {
public:
    /// @brief Default constructor.
    StRpsRomanPot();
    ~StRpsRomanPot();

    /// @brief Returns the Roman Pot identifier (0–7).
    unsigned int romanPotId() const;
    /// @brief Returns the total number of silicon detector planes (always 4).
    unsigned int numberOfPlanes() const;
    /// @brief Returns the number of planes that contain at least one reconstructed cluster.
    unsigned int numberOfPlanesWithClusters() const;
    /// @brief Returns the ADC value of the PMT timing detector at the given index (0–1).
    unsigned int adc(unsigned int) const;
    /// @brief Returns the TAC value of the PMT timing detector at the given index (0–1).
    unsigned int tac(unsigned int) const;
    /// @brief Returns the Roman Pot status flag.
    unsigned char status() const;
    
    /// @brief Returns a const pointer to the silicon detector plane at the given index.
    const StRpsPlane* plane(unsigned int) const;
    /// @brief Returns a pointer to the silicon detector plane at the given index.
    StRpsPlane* plane(unsigned int);
    
    /// @brief Sets the Roman Pot status flag.
    void setStatus(unsigned char);
    /// @brief Sets the ADC value for the PMT timing detector at the given index.
    void setAdc(unsigned int, unsigned int);
    /// @brief Sets the TAC value for the PMT timing detector at the given index.
    void setTac(unsigned int, unsigned int);
    
protected:
    void setRomanPotId(unsigned char);
    friend class StRpsCollection;
    
protected:
    enum {mNumberOfPlanes = 4}; ///< Number of silicon strip detector planes per Roman Pot.
    StRpsPlane mPlanes[mNumberOfPlanes]; ///< Array of silicon strip detector planes.
    UChar_t  mRomanPotId; ///< Roman Pot index (0–7).
    UChar_t  mStatus;     ///< Roman Pot status flag.
    UInt_t   mAdc[2];     ///< ADC values from the two PMT scintillator timing detectors.
    UInt_t   mTac[2];     ///< TAC values from the two PMT scintillator timing detectors.

    ClassDef(StRpsRomanPot,1)
};

#endif
