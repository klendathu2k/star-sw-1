/***************************************************************************
 *
 * $Id: StFcsHit.h,v 2.1 2021/01/11 20:25:37 ullrich Exp $
 *
 * Author: Akio Ogawaa, Aug 2018
 ***************************************************************************
 *
 * Description: StFcsHit is data for individual cell 
 *
 ***************************************************************************
 *
 * $Log: StFcsHit.h,v $
 * Revision 2.1  2021/01/11 20:25:37  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StFcsHit_hh
#define StFcsHit_hh

/// @file StFcsHit.h
/// @brief Raw hit from a single FCS (Forward Calorimeter System) tower channel.

#include "Stiostream.h"
#include "StObject.h"
#include "TArrayS.h"
class StFcsCluster;

/// @brief A single FCS tower hit storing detector address, time-bin ADC data, and calibrated energy.
class StFcsHit : public StObject {
public:
    /// @brief Default constructor.
    StFcsHit();
    /// @brief Constructor with time-bin ADC data.
    StFcsHit(unsigned short zs, unsigned short det, unsigned short id,
             unsigned short ns, unsigned short ehp, unsigned short dep, unsigned short ch, 
             int ntimebin, unsigned short* data);
    /// @brief Constructor with pre-computed energy.
    StFcsHit(unsigned short zs, unsigned short det, unsigned short id,
             unsigned short ns, unsigned short ehp, unsigned short dep, unsigned short ch, 
             float e);
    /// @brief Destructor.
    ~StFcsHit();
    
    unsigned short zs() const;         ///< Return 1 if zero-suppressed, 0 otherwise.
    unsigned short detectorId() const; ///< Return the FCS sub-detector ID.
    unsigned short id() const;         ///< Return the tower channel ID within the detector.
    unsigned short ns() const;         ///< Return North/South flag (from DEP).
    unsigned short ehp() const;        ///< Return ECal/HCal/Pre-shower flag (from DEP).
    unsigned short dep() const;        ///< Return DEP board number.
    unsigned short channel() const;    ///< Return channel number within the DEP board.
    unsigned int   nTimeBin() const;   ///< Return the number of time bins in the stored waveform.
    unsigned short timebin(int i) const; ///< Return the time-bin index for waveform sample @p i.
    unsigned short data(int i) const;  ///< Return the raw data word (ADC + flag) for time-bin @p i.
    unsigned short adc(int i) const;   ///< Return the 12-bit ADC value for time-bin @p i.
    unsigned short flag(int i) const;  ///< Return the flag bits for time-bin @p i.
    int   adcSum() const;              ///< Return the sum of ADC values over all time bins.
    float fitPeak() const;             ///< Return the fitted peak position in the waveform.
    float fitSigma() const;            ///< Return the fitted Gaussian sigma of the waveform peak.
    float fitChi2() const;             ///< Return the chi^2 of the waveform fit.
    int   nPeak() const;               ///< Return the number of peaks found in the waveform.
    float energy() const;              ///< Return the calibrated energy deposit (GeV).
    
    /// @brief Set all DEP address fields at once.
    void setDepCh(unsigned short ns, unsigned short ehp, unsigned short dep, unsigned short ch);
    /// @brief Set the North/South flag.
    void setNS(unsigned short val);
    /// @brief Set the ECal/HCal/Pre-shower flag.
    void setEHP(unsigned short val);
    /// @brief Set the DEP board number.
    void setDep(unsigned short val);
    /// @brief Set the DEP channel number.
    void setChannel(unsigned short val);

    /// @brief Set detector identification fields.
    void setDetId(unsigned short zs, unsigned short det, unsigned short id);
    /// @brief Set the zero-suppression flag.
    void setZS(unsigned short val);
    /// @brief Set the packed detector ID word.
    void setDetId(unsigned short val);
    /// @brief Set the sub-detector ID.
    void setDetectorId(unsigned short val);
    /// @brief Set the tower channel ID.
    void setId(unsigned short val);

    /// @brief Set the waveform data array (n samples).
    void setData(int n, const unsigned short* d);
    /// @brief Set a single waveform sample at time bin @p tb.
    void setDataAt(int tb, unsigned short val);
    /// @brief Set ADC and flag for a single time bin.
    void setAdcFlag(int tb, unsigned short adc, unsigned short flag);
    /// @brief Set the ADC value at time bin @p tb.
    void setAdc(int tb, unsigned short val);
    /// @brief Set the flag bits at time bin @p tb.
    void setFlag(int tb, unsigned short val);

    /// @brief Set the ADC sum.
    void setAdcSum(int v);
    /// @brief Set the fitted waveform peak position.
    void setFitPeak(float v);
    /// @brief Set the fitted waveform sigma.
    void setFitSigma(float v);
    /// @brief Set the waveform fit chi^2.
    void setFitChi2(float v);
    /// @brief Set the number of peaks in the waveform.
    void setNPeak(int v);
    /// @brief Set the calibrated energy deposit.
    void setEnergy(float v);

    /// @brief Set all hit fields from time-bin ADC data.
    void setFcsHit(unsigned short zs, unsigned short det, unsigned short id,
                   unsigned short ns, unsigned short ehp, unsigned short dep, unsigned short ch, 
		   int ntimebin, unsigned short* data);
    /// @brief Set all hit fields using a pre-computed energy.
    void setFcsHit(unsigned short zs, unsigned short det, unsigned short id,
                   unsigned short ns, unsigned short ehp, unsigned short dep, unsigned short ch, 
		   float e);
    
    /// @brief Associate this hit with a cluster.
    void setCluster(StFcsCluster* clu) {mCluster = clu;}
    StFcsCluster *cluster() {return mCluster;} ///< Return the cluster this hit belongs to.

    /// @brief Return the list of Geant4 parent tracks contributing to this hit (track ID, dE).
    const vector<pair<unsigned int, float>>& getGeantTracks() const {return mGeantTracks;}
    /// @brief Add a Geant4 parent track contribution (track ID and energy deposit).
    void addGeantTrack(unsigned int id, float e);

    /// @brief Print hit properties.
    void print(Option_t *option="") const;

protected:
    UShort_t mDetId=0;        ///< Packed detector ID: bit15=ZS flag, bits14-12=DetectorId, bits11-0=channel id.
    UShort_t mDepCh=0;        ///< Packed DEP address: bit15=NS, bits14-13=EHP, bits12-8=DEP, bits7-0=channel.
    UInt_t   mAdcSum=0;       ///< Sum of ADC values over all time bins.
    Float_t  mFitPeak=0.0;    ///< Fitted waveform peak position (time bin).
    Float_t  mFitSigma=0.0;   ///< Fitted waveform Gaussian sigma (time bins).
    Float_t  mFitChi2=0.0;    ///< Chi^2 of the waveform fit.
    UInt_t   mNPeak=0;        ///< Number of peaks identified in the waveform.
    Float_t  mEnergy=0.0;     ///< Calibrated energy deposit (GeV).
    StFcsCluster* mCluster=0; ///< Pointer to the cluster this hit belongs to (not owned).
    TArrayS* mData=0;         ///< Waveform data: 12-bit ADC value + 4 flag bits per time bin.

    vector<pair<unsigned int, float>> mGeantTracks; ///< Geant4 parent track contributions (track ID, dE).

    ClassDef(StFcsHit,6)
};

#endif
