/*!
 * \class StProbPidTraits 
 * \author Yuri Fisyak, Oct 2002
 */
/***************************************************************************
 *
 * $Id: StProbPidTraits.h,v 2.6 2013/12/17 15:07:34 fisyak Exp $
 *
 * Author: 
 ***************************************************************************
 *
 * Description: Yuri Fisyak, Oct 2002
 *    keep track Likelihood for each mass hypothesis
 *
 **************************************************************************/
#ifndef StProbPidTraits_hh
#define StProbPidTraits_hh

/// @file StProbPidTraits.h
/// @brief PID traits storing per-hypothesis likelihood and probability arrays for a track.

#include "StDetectorId.h"
#include "StTrackPidTraits.h"
#include "TArrayF.h"
#include "StParticleDefinition.hh"
#include "StPidParticleDefinition.h"

/// @brief PID traits storing per-hypothesis log-likelihoods and probabilities for a reconstructed track.
///
/// Holds the results of a likelihood-based PID fit, providing the probability and chi-squared
/// probability for each particle mass hypothesis (e, mu, pi, K, p, ...).
class StProbPidTraits : public StTrackPidTraits {
public:
    /// @brief Construct from the number of degrees of freedom, detector ID, particle count, and PID arrays.
    StProbPidTraits(const Int_t NDF=0, const StDetectorId Id=kUnknownId, const StPidParticle N=KPidParticles,
		    const Float_t *PidArray = 0, Double_t *Fractions = 0);
    /// @brief Destructor.
    virtual ~StProbPidTraits();
    /// @brief Returns the number of degrees of freedom used in the PID likelihood fit.
    Int_t    GetNDF()      {return mNDF;}
    /// @brief Returns the array of raw PID values (typically −2 log L) for each particle hypothesis.
    TArrayF *GetPidArray() {return mPidArray;}
    /// @brief Returns the normalised probability for the given particle hypothesis index.
    Double_t GetProbability(Int_t PartId) ;
    /// @brief Returns the chi-squared probability for the given particle hypothesis index.
    Double_t GetChi2Prob(Int_t PartId) const;
    /// @brief Returns the normalisation sum used when computing probabilities; transient.
    Double_t GetSum() { return mSum;}
    /// @brief Sets the particle abundance fractions used as priors in probability normalisation; transient.
    void     SetFractions(Double_t *Fractions) {mFractions = Fractions; mSum = 0;}
    /// @brief Sets the number of degrees of freedom for the PID fit.
    void     SetNDF(Int_t ndf)                 {mNDF = ndf;}
    /// @brief Prints PID probabilities for all particle hypotheses.
    void     Print(Option_t *opt = "") const;
    /// @brief Array of particle definitions corresponding to each hypothesis index; transient.
    static   StParticleDefinition  *mPidParticleDefinitions[KPidParticles]; //!

 protected:
    Int_t     mNDF;                         ///< Number of degrees of freedom in the PID likelihood fit.
    TArrayF  *mPidArray;                    ///< Raw PID values (−2 log L) for each particle hypothesis.
    Double_t  mSum;                         //!  ///< Normalisation sum; transient.
    Double_t *mFractions;                   //!  ///< Prior particle abundance fractions; transient.
    Double_t  mProbability[KPidParticles];  //!  ///< Cached normalised probabilities for each hypothesis; transient.
    
    ClassDef(StProbPidTraits,1)
};
#endif
