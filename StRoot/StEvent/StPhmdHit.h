/*!
 *\class StPhmdHit
 *\author
*/
/********************************************************************
 *
 * $Id: StPhmdHit.h,v 2.4 2003/09/02 17:58:05 perev Exp $
 *
 * Author: Subhasis Chattopadhyay, Dec 2002
 ********************************************************************
 *
 * Description: This is the class for PMD hit objects
 *
 ********************************************************************
 *
 * $Log: StPhmdHit.h,v $
 * Revision 2.4  2003/09/02 17:58:05  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.3  2003/04/22 00:08:14  ullrich
 * Removed //! comments
 *
 * Revision 2.2  2002/12/21 00:32:33  ullrich
 * Corrected typo in module().
 *
 * Revision 2.1  2002/12/20 22:33:00  ullrich
 * Initial Revision.
 *
 ********************************************************************/
#ifndef StPhmdHit_hh
#define StPhmdHit_hh

/// @file StPhmdHit.h
/// @brief Single cell hit in the STAR Photon Multiplicity Detector (PMD/PHMD).

#include <math.h>
#include <Stiostream.h>
#include "StObject.h"

/// @brief Stores raw hit information for a single PMD cell (pad).
class StPhmdHit : public StObject {
public: 
    StPhmdHit();                     
    ~StPhmdHit();                     

    /// @brief Returns the global supermodule number [0,11].
    int             superModule() const;
    /// @brief Returns the module index in range [0,11], or -1 if out of range.
    int             module() ;
    /// @brief Returns the sub-detector identifier (PMD or CPV).
    int             subDetector() const;
    /// @brief Returns the cell row within the supermodule.
    int             row() const;
    /// @brief Returns the cell column within the supermodule.
    int             column() const;
    /// @brief Returns the energy deposition in the cell (GeV).
    float           energy() const;
    /// @brief Returns the raw ADC count for the cell.
    int             adc() const;
    
    /// @brief Sets the global supermodule number.
    void            setSuperModule(int);
    /// @brief Sets the sub-detector identifier (PMD or CPV).
    void            setSubDetector(int);
    /// @brief Sets the cell row within the supermodule.
    void            setRow(int);
    /// @brief Sets the cell column within the supermodule.
    void            setColumn(int);
    /// @brief Sets the energy deposition (GeV).
    void            setEnergy(float);
    /// @brief Sets the raw ADC count.
    void            setAdc(int);
    
private:
    Int_t           mSuperModuleNumber;   ///< Global supermodule number [0,11].
    Int_t           mSubDetector;         ///< Sub-detector identifier (PMD or CPV).
    Int_t           mRow;                 ///< Cell row within the supermodule.
    Int_t           mCol;                 ///< Cell column within the supermodule.
    Float_t         mEnergy;              ///< Energy deposition in the cell (GeV).
    Int_t           mAdc;                 ///< Raw ADC count.
    ClassDef(StPhmdHit,1)
};

inline   int    StPhmdHit::superModule() const {return mSuperModuleNumber;}
inline   int    StPhmdHit::module()
{
    // return module in range 0-11
    if (mSuperModuleNumber < 12) 
	return mSuperModuleNumber;
    else
	return -1;
}
inline int    StPhmdHit::subDetector() const {return mSubDetector;} 
inline int    StPhmdHit::row() const         {return mRow;} 
inline int    StPhmdHit::column() const      {return mCol;} 
inline float  StPhmdHit::energy() const      {return mEnergy;}
inline int    StPhmdHit::adc() const         {return mAdc;}
inline void   StPhmdHit::setSuperModule(int var) {mSuperModuleNumber = var;}
inline void   StPhmdHit::setSubDetector(int var) {mSubDetector = var;}
inline void   StPhmdHit::setRow(int var)     {mRow = var;}
inline void   StPhmdHit::setColumn(int var)  {mCol = var;}
inline void   StPhmdHit::setEnergy(float var){mEnergy = var;}
inline void   StPhmdHit::setAdc(int var)     {mAdc = var;}

ostream&  operator<<(ostream&, const StPhmdHit&);
#endif



