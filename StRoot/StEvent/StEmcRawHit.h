/*!
 * \class StEmcRawHit 
 * \author Akio Ogawa, Jan 2000
 */
/***************************************************************************
 *
 * $Id: StEmcRawHit.h,v 2.9 2012/09/16 21:33:57 fisyak Exp $
 *
 * Author: Akio Ogawa, Jan 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEmcRawHit.h,v $
 * Revision 2.9  2012/09/16 21:33:57  fisyak
 * Add print out
 *
 * Revision 2.8  2004/07/20 17:07:49  perev
 * Pavlinov corrs for TBrowser
 *
 * Revision 2.7  2004/07/15 16:36:24  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.6  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.5  2001/04/05 04:00:35  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.4  2001/03/24 03:34:46  perev
 * clone() -> clone() const
 *
 * Revision 2.3  2000/07/28 19:49:28  akio
 * Change in Detector Id for Endcap SMD
 *
 * Revision 2.2  2000/05/22 19:21:54  akio
 * Bug fix, add delta into EMcPoint, wider bits for Eta in RawHit
 *
 * Revision 2.1  2000/02/23 17:34:14  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StEmcRawHit_hh
#define StEmcRawHit_hh

/// @file StEmcRawHit.h
/// @brief Raw ADC hit in one cell of the STAR Electromagnetic Calorimeter (EMC).

#include "StObject.h"
#include "StEnumerations.h"

class StEmcGeom;

/// @brief Raw ADC hit in a single cell of one STAR EMC sub-detector, identified by module, eta bin, and sub-division.
class StEmcRawHit : public StObject {
public:
    StEmcRawHit();
    /// @brief Constructs a raw hit with detector id, module, eta bin, sub-division, and ADC value.
    StEmcRawHit(StDetectorId, unsigned int, unsigned int, unsigned int, unsigned int);
    /// @brief Constructs a raw hit with detector id, module, eta bin, sub-division, ADC value, and calibrated energy.
    StEmcRawHit(StDetectorId, unsigned int, unsigned int, unsigned int, unsigned int, float);
    /// @brief Copy constructor.
    StEmcRawHit(const StEmcRawHit&);
    // StEmcRawHit& operator=(const StEmcRawHit&); use default
    ~StEmcRawHit();
    
    /// @brief Returns the sub-detector identifier for this hit.
    StDetectorId    detector() const;
    /// @brief Unpacks the packed cell id into module, eta bin, and sub-division indices.
    void            modEtaSub(int &m, int &e, int &s) const;
    /// @brief Returns the software cell id as used by StEmcGeom for the given detector.
    unsigned int    softId(int det) const; // as in StEmcGeom;

    /// @brief Returns the module index of the hit cell.
    unsigned int    module() const;
    /// @brief Returns the η-bin index of the hit cell within the module.
    unsigned int    eta() const;
    /// @brief Returns the φ sub-division (sub) index of the hit cell.
    int             sub() const;
    /// @brief Returns the calibration type flag applied to this hit.
    unsigned int    calibrationType() const;
    /// @brief Returns the raw ADC value of this hit.
    unsigned int    adc() const;
    /// @brief Returns the calibrated energy of this hit [GeV].
    float           energy() const;
    
    /// @brief Sets the packed cell identifier from detector id, module, eta bin, and sub-division.
    void setId(StDetectorId, unsigned int, unsigned int, unsigned int);
    /// @brief Sets the calibration type flag.
    void setCalibrationType(const unsigned int);
    /// @brief Sets the raw ADC value.
    void setAdc(const unsigned int);
    /// @brief Sets the calibrated energy [GeV].
    void setEnergy(const float);
    /// @brief Prints hit information to the output stream (ROOT interface).
    void     Print(Option_t *option="") const;
protected:
    /// @brief Extracts a bit field from @a mId starting at bit @a pos with @a len bits.
    unsigned int    bits(unsigned int, unsigned int) const;
    
protected:
    UInt_t    mId;     ///< Packed cell identifier encoding detector, module, eta bin, and sub-division.
    UInt_t    mAdc;    ///< Raw ADC value and calibration type flag (packed).
    Float_t   mEnergy; ///< Calibrated energy deposited in this cell [GeV].
    
    static StEmcGeom* mGeom; ///< Pointer to the shared EMC geometry object (used for softId conversion).

    ClassDef(StEmcRawHit,1)
}; 
ostream&              operator<<(ostream& os, StEmcRawHit const & v);
#endif


