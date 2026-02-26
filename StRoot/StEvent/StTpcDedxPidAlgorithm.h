/*!
 * \class StTpcDedxPidAlgorithm 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StTpcDedxPidAlgorithm.h,v 2.10 2014/01/15 21:01:42 fisyak Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTpcDedxPidAlgorithm.h,v $
 * Revision 2.10  2014/01/15 21:01:42  fisyak
 * change default from kTruncatedMeanId => kLikelihoodFitId
 *
 * Revision 2.9  2010/08/31 20:15:11  fisyak
 * Clean up
 *
 * Revision 2.8  2002/03/26 23:09:17  ullrich
 * Added destructor.
 *
 * Revision 2.7  2002/02/22 22:56:52  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.6  2000/03/02 12:43:51  ullrich
 * Method can be passed as argument to constructor. Default
 * method is truncated mean.
 *
 * Revision 2.5  1999/12/21 15:09:14  ullrich
 * Modified to cope with new compiler version on Sun (CC5.0).
 *
 * Revision 2.4  1999/12/02 16:35:40  ullrich
 * Added method to return the stored dE/dx traits
 *
 * Revision 2.3  1999/11/29 15:33:34  ullrich
 * Changed Macro for SUN CC
 *
 * Revision 2.2  1999/10/28 22:27:04  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:43:55  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StTpcDedxPidAlgorithm_hh
#define StTpcDedxPidAlgorithm_hh

/// @file StTpcDedxPidAlgorithm.h
/// @brief PID algorithm that identifies particles via TPC dE/dx measurements.

#include <vector>
#include "StFunctional.h"
#include "StEnumerations.h"
#include "Rtypes.h"
#if !defined(ST_NO_NAMESPACES)
using std::vector;
#endif

class StDedxPidTraits;

/// @brief PID algorithm that identifies particles using TPC dE/dx measurements.
///
/// Selects the best-matching particle hypothesis for a track by comparing the
/// measured dE/dx to Bethe-Bloch expectations using the configured dE/dx method.
class StTpcDedxPidAlgorithm : public StPidAlgorithm {
public:
    /// @brief Construct the algorithm with the specified dE/dx estimation method.
    StTpcDedxPidAlgorithm(StDedxMethod = kLikelihoodFitId);
    /// @brief Destructor.
    ~StTpcDedxPidAlgorithm() {}

    /// @brief Evaluates the track PID traits and returns the best-matching particle definition.
    StParticleDefinition*  operator() (const StTrack&, const StSPtrVecTrackPidTraits&);

    /// @brief Returns the dE/dx PID traits selected during the last operator() call; transient.
    const  StDedxPidTraits* traits() const { return mTraits; }
    /// @brief Returns the number of sigma deviation from the expected dE/dx for the given particle hypothesis.
    Double_t numberOfSigma(const StParticleDefinition*) const;

private:
    /// @brief Last matched dE/dx PID traits; transient.
    const StDedxPidTraits*        mTraits;       //!
    /// @brief Track evaluated during the last operator() call; transient.
    const StTrack*                mTrack;        //!
    /// @brief dE/dx estimation method used for PID (e.g. truncated mean, likelihood fit); transient.
    StDedxMethod                  mDedxMethod;   //!
    /// @brief Candidate particle definitions evaluated by the algorithm; transient.
#if defined(ST_NO_TEMPLATE_DEF_ARGS)
    vector<StParticleDefinition*,
           allocator<StParticleDefinition*> > mParticles; //!
#else
    vector<StParticleDefinition*> mParticles;    //!
#endif
};
#endif


