/*!
 * \class StEmcDetector 
 * \author Akio Ogawa, Jan 2000
 */
/***************************************************************************
 *
 * $Id: StEmcDetector.h,v 2.8 2004/10/15 18:17:58 ullrich Exp $
 *
 * Author: Akio Ogawa, Jan 2000`
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEmcDetector.h,v $
 * Revision 2.8  2004/10/15 18:17:58  ullrich
 * Increased mMaxNumberOfCrates to 36.
 *
 * Revision 2.7  2004/10/14 20:00:18  ullrich
 * Added member and methods to deal with crate status flags.
 *
 * Revision 2.6  2004/07/20 17:07:49  perev
 * Pavlinov corrs for TBrowser
 *
 * Revision 2.5  2003/09/12 21:54:53  jeromel
 * Zeroing
 *
 * Revision 2.4  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:35  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2000/10/26 00:02:24  ullrich
 * Fixed various problems causing I/O failures.
 *
 * Revision 2.1  2000/02/23 17:33:59  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StEmcDetector_hh
#define StEmcDetector_hh

/// @file StEmcDetector.h
/// @brief One sub-detector of the STAR Electromagnetic Calorimeter (EMC).

#include "StObject.h"
#include "StEnumerations.h"

class StEmcRawHit;
class StEmcModule;
class StEmcClusterCollection;

/// @brief One sub-detector of the STAR EMC (e.g. BEMC towers, BPRS, BSMDE, BSMDP), holding modules, hits, and clusters.
class StEmcDetector : public StObject {
public:
    StEmcDetector();
    /// @brief Constructs a detector object with a given ID and number of modules.
    StEmcDetector(StDetectorId, unsigned int);
    ~StEmcDetector();
    
    /// @brief Returns the detector identifier (e.g. kBarrelEmcTowerId).
    StDetectorId  detectorId() const;
    /// @brief Returns the number of modules in this detector.
    unsigned int  numberOfModules() const;
    
    /// @brief Adds a raw hit to the appropriate module; returns false if the hit cannot be placed.
    bool          addHit(StEmcRawHit*);
    /// @brief Returns the total number of raw hits across all modules.
    unsigned int  numberOfHits() const;
    /// @brief Prints the hit count per module (ROOT browser menu entry).
    void          printNumberOfHits() const; // *MENU*
    /// @brief Returns the total energy deposited in this detector [GeV].
    double        getEnergy(const int pri=0) const; // *MENU*
    
    /// @brief Returns the module at the given index (1-based).
    StEmcModule*       module(unsigned int);
    const StEmcModule* module(unsigned int) const;
    
    /// @brief Returns the cluster collection for this detector.
    StEmcClusterCollection*         cluster();
    const StEmcClusterCollection*   cluster() const;
    
    /// @brief Returns the readout-crate status flag for the given crate number.
    StEmcCrateStatus crateStatus(int) const;

    /// @brief Sets the cluster collection for this detector (takes ownership).
    void setCluster(StEmcClusterCollection*);
    /// @brief Sets the module at the given slot index.
    void setModule(StEmcModule*, int);
    /// @brief Sets the readout-crate status flag for the given crate number.
    void setCrateStatus(int, StEmcCrateStatus);

    virtual bool  IsFolder() const;    

protected:
    /// @brief Deletes and nullifies all module pointers.
    void clear();
    
private:
    enum { mMaxNumberOfCrates = 36 };
    StDetectorId            mDetectorId;                          ///< Identifier of this EMC sub-detector.
    UInt_t                  mNumberOfModules;                     ///< Number of readout modules in this sub-detector.
    StEmcModule             *mModules[120];                       ///< Array of module objects (1-based, up to 120).
    StEmcClusterCollection  *mClusters;                          ///< Owned cluster collection for this sub-detector.
    StEmcCrateStatus        mCrateStatusFlag[mMaxNumberOfCrates]; ///< Status flags for each readout crate.
    
    ClassDef(StEmcDetector,2)
};
#endif
