/*!
 * \class StEmcCollection 
 * \author Akio Ogawa, Jan 2000
 */
/***************************************************************************
 *
 * $Id: StEmcCollection.h,v 2.5 2004/03/26 21:54:15 ullrich Exp $
 *
 * Author: Akio Ogawa, Jan 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEmcCollection.h,v $
 * Revision 2.5  2004/03/26 21:54:15  ullrich
 * Added StEmcRawData to collection.
 *
 * Revision 2.4  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:35  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2000/03/23 22:24:06  akio
 * Initial version of Emc Point, and Inclusion of track pointers
 *
 * Revision 2.1  2000/02/23 17:34:07  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StEmcCollection_hh
#define StEmcCollection_hh

/// @file StEmcCollection.h
/// @brief Top-level container for all STAR Electromagnetic Calorimeter (EMC) data in one event.

#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"
#include "StEmcPoint.h"
#include "StEmcRawData.h"

class StEmcDetector;

/// @brief Top-level event container for all STAR EMC sub-detectors, global points, and raw DAQ data.
class StEmcCollection : public StObject {
public:
    StEmcCollection();
    ~StEmcCollection();
    
    /// @brief Returns the sub-detector object for the given detector identifier.
    StEmcDetector*            detector(StDetectorId);
    const StEmcDetector*      detector(StDetectorId) const;
    
    /// @brief Returns the list of reconstructed points in the Barrel EMC (BEMC).
    StSPtrVecEmcPoint&        barrelPoints();
    const StSPtrVecEmcPoint&  barrelPoints() const;
    /// @brief Returns the list of reconstructed points in the Endcap EMC (EEMC).
    StSPtrVecEmcPoint&        endcapPoints();
    const StSPtrVecEmcPoint&  endcapPoints() const;
    
    /// @brief Returns the raw DAQ data for the Barrel EMC (BEMC).
    StEmcRawData*             bemcRawData();
    /// @brief Returns the raw DAQ data for the Endcap EMC (EEMC).
    StEmcRawData*             eemcRawData();
    
    /// @brief Adds a reconstructed point to the Barrel EMC (BEMC) collection.
    void addBarrelPoint(const StEmcPoint*);
    /// @brief Adds a reconstructed point to the Endcap EMC (EEMC) collection.
    void addEndcapPoint(const StEmcPoint*);
  
    /// @brief Registers an EMC sub-detector object (takes ownership by detector ID slot).
    void setDetector(StEmcDetector*);
    
    /// @brief Sets the raw DAQ data for the Barrel EMC (BEMC).
    void setBemcRawData(StEmcRawData*);
    /// @brief Sets the raw DAQ data for the Endcap EMC (EEMC).
    void setEemcRawData(StEmcRawData*);
    

private:
    StEmcCollection(const StEmcCollection&);
    StEmcCollection& operator=(const StEmcCollection&);
    
private:
    StEmcDetector*            mDetector[8];  ///< Sub-detector objects, indexed by StDetectorId.
    StSPtrVecEmcPoint         mBarrel;       ///< Reconstructed points in the Barrel EMC (BEMC).
    StSPtrVecEmcPoint         mEndcap;       ///< Reconstructed points in the Endcap EMC (EEMC).
    StEmcRawData*             mBemcRawData;  ///< Raw DAQ data banks for the BEMC.
    StEmcRawData*             mEemcRawData;  ///< Raw DAQ data banks for the EEMC.

    ClassDef(StEmcCollection,2)
};
#endif








