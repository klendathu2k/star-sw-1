/**
 * \class StRunInfo
 * \brief Run-level metadata stored inside StEvent.
 * \author Thomas Ullrich, Sep 2001
 *
 * \details StRunInfo holds configuration and environmental quantities that
 * are constant across all events in a single run: beam species and energy,
 * magnetic field, TPC and SVT drift velocities, RHIC luminosity monitor
 * rates (ZDC, BBC), and reconstruction software version.  One instance is
 * attached to each StEvent and accessed via StEvent::runInfo().
 *
 * \sa StEvent, StEventInfo
 */
/***************************************************************************
 *
 * $Id: StRunInfo.h,v 2.8 2004/10/20 16:06:53 ullrich Exp $
 *
 * Author: Thomas Ullrich, Sep 2001
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StRunInfo.h,v $
 * Revision 2.8  2004/10/20 16:06:53  ullrich
 * Add variables to report on space charge and the correction mode.
 *
 * Revision 2.7  2004/07/06 23:05:26  ullrich
 * Added SVT drift velocity scaler.
 *
 * Revision 2.6  2004/01/22 23:14:07  ullrich
 * Added Rhic scaler methods (BBC).
 *
 * Revision 2.5  2002/02/25 19:32:47  ullrich
 * Added more RHIC related info.
 *
 * Revision 2.4  2002/02/22 22:56:50  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2002/01/31 23:42:36  ullrich
 * Added member to hold BBC coincidence rate.
 *
 * Revision 2.2  2001/12/02 19:27:12  ullrich
 * Added new member and methods.
 *
 * Revision 2.1  2001/09/18 00:14:17  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StRunInfo_hh
#define StRunInfo_hh
/// @file StRunInfo.h
/// @brief Run-level metadata (beam species, energy, magnetic field, luminosity rates) stored inside StEvent.

#include "StObject.h"
#include "TString.h"
#include "StEnumerations.h"
#include <ctime>

class StRunInfo : public StObject {
public:
    StRunInfo();
    // StRunInfo(const StRunInfo&);            use default
    // StRunInfo& operator=(const StRunInfo&); use default
    virtual ~StRunInfo();

    /// \name Run identification
    /// @{
    int      runId() const;                              ///< Run number.
    time_t   productionTime() const;                     ///< Unix timestamp when this run was reconstructed.
    TString  productionVersion() const;                  ///< Reconstruction software version string.
    /// @}

    /// \name Beam and collision parameters
    /// @{
    double   centerOfMassEnergy() const;                 ///< \f$\sqrt{s_{NN}}\f$ in GeV.
    int      beamMassNumber(StBeamDirection) const;      ///< Mass number A of the beam species (east or west).
    float    beamEnergy(StBeamDirection) const;          ///< Beam energy per nucleon (GeV) for the given direction.
    float    initialBeamIntensity(StBeamDirection) const; ///< Initial beam intensity at fill start.
    float    beamLifeTime(StBeamDirection) const;        ///< Beam lifetime (hours).
    float    beamFillNumber(StBeamDirection) const;      ///< RHIC fill number for the given beam direction.
    /// @}

    /// \name Detector parameters
    /// @{
    double   magneticField() const;                      ///< Solenoidal magnetic field (kG, positive = along +z).
    double   tpcDriftVelocity(StBeamDirection) const;    ///< TPC drift velocity for east/west half (cm/µs).
    double   svtDriftVelocityScaler() const;             ///< SVT drift-velocity scale factor.
    /// @}

    /// \name Luminosity monitor rates
    /// @{
    double   zdcWestRate() const;                        ///< ZDC west single-arm rate (Hz).
    double   zdcEastRate() const;                        ///< ZDC east single-arm rate (Hz).
    double   zdcCoincidenceRate() const;                 ///< ZDC east–west coincidence rate (Hz).
    double   bbcCoincidenceRate() const;                 ///< BBC east–west coincidence rate (Hz).
    double   backgroundRate() const;                     ///< Estimated beam-background rate (Hz).
    double   l0RateToRich() const;                       ///< L0 trigger rate delivered to the RICH (Hz).

    double   bbcEastRate() const;                        ///< BBC east single-arm rate (Hz).
    double   bbcWestRate() const;                        ///< BBC west single-arm rate (Hz).
    double   bbcBlueBackgroundRate() const;              ///< BBC blue-beam background rate (Hz).
    double   bbcYellowBackgroundRate() const;            ///< BBC yellow-beam background rate (Hz).
    /// @}

    /// \name Space charge correction
    /// @{
    int      spaceChargeCorrectionMode() const;          ///< TPC space-charge correction mode flag.
    float    spaceCharge() const;                        ///< TPC space-charge correction value.
    /// @}

    void     setRunId(int);
    void     setProductionTime(time_t);                 
    void     setProductionVersion(const char*);   
    void     setCenterOfMassEnergy(double);             
    void     setBeamMassNumber(StBeamDirection, int);  
    void     setBeamEnergy(StBeamDirection, float);
    void     setInitialBeamIntensity(StBeamDirection, float);
    void     setBeamLifeTime(StBeamDirection, float);
    void     setBeamFillNumber(StBeamDirection, float);
    void     setMagneticField(double);                  
    void     setTpcDriftVelocity(StBeamDirection, double);
    void     setSvtDriftVelocityScaler(float);

    void     setZdcWestRate(double);
    void     setZdcEastRate(double);
    void     setZdcCoincidenceRate(double);
    void     setBbcCoincidenceRate(double);
    void     setBackgroundRate(double);
    void     setL0RateToRich(double);

    void     setBbcEastRate(double);	
    void     setBbcWestRate(double);	
    void     setBbcBlueBackgroundRate(double);
    void     setBbcYellowBackgroundRate(double);

    void     setSpaceChargeCorrectionMode(int);
    void     setSpaceCharge(float);

    
protected:
    Int_t	mRunId;                         ///< Run number.
    
    UInt_t      mProductionTime;               ///< Unix timestamp when this data was reconstructed.
    TString     mProductionVersion;            ///< Reconstruction software version string.
    
    Float_t     mCenterOfMassEnergy;           ///< \f$\sqrt{s_{NN}}\f$ in GeV.
    Int_t       mBeamMassNumber[2];            ///< Mass number A of the beam species; [0]=east, [1]=west.
    
    Double_t    mMagneticFieldZ;               ///< Solenoidal magnetic field along z (kG).
    Float_t     mTpcDriftVelocity[2];          ///< TPC drift velocity (cm/µs); [0]=east half, [1]=west half.
    Float_t     mSvtDriftVelocityScaler;       ///< SVT drift-velocity scale factor.

    Double_t    mZdcEastRate;                  ///< ZDC east single-arm rate (Hz).
    Double_t    mZdcWestRate;                  ///< ZDC west single-arm rate (Hz).
    Double_t    mZdcCoincidenceRate;           ///< ZDC east–west coincidence rate (Hz).
    Double_t    mBackgroundRate;               ///< Estimated beam-background rate (Hz).
    Double_t    mL0RateToRich;                 ///< L0 trigger rate delivered to the RICH (Hz).
    Double_t    mBbcCoincidenceRate;           ///< BBC east–west coincidence rate (Hz).

    Float_t     mBeamEnergy[2];                ///< Beam energy per nucleon (GeV); [0]=east, [1]=west.
    Float_t     mInitialBeamIntensity[2];      ///< Initial beam intensity at fill start; [0]=east, [1]=west.
    Float_t     mBeamLifeTime[2];              ///< Beam lifetime (hours); [0]=east, [1]=west.
    Float_t     mBeamFillNumber[2];            ///< RHIC fill number; [0]=east, [1]=west.

    Double_t    mBbcEastRate;                  ///< BBC east single-arm rate (Hz).
    Double_t    mBbcWestRate;                  ///< BBC west single-arm rate (Hz).
    Double_t    mBbcBlueBackgroundRate;        ///< BBC blue-beam background rate (Hz).
    Double_t    mBbcYellowBackgroundRate;      ///< BBC yellow-beam background rate (Hz).

    Int_t       mSpaceChargeCorrectionMode;    ///< TPC space-charge correction mode flag.
    Float_t     mSpaceCharge;                  ///< TPC space-charge correction value.
    
    ClassDef(StRunInfo,7)
};
#endif
