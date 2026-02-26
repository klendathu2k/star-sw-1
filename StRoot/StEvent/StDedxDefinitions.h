#ifndef StDedxDefinitions_hh
#define StDedxDefinitions_hh

/// @file StDedxDefinitions.h
/// @brief Preprocessor constants identifying dE/dx measurement methods (Fortran-compatible).
///
/// These integer codes identify the algorithm used to extract the
/// ionisation energy-loss (dE/dx) signal from TPC or other tracking
/// detectors.  They back the StDedxMethod enumeration in StEnumerations.h.

/* Numbering scheme for dEdx 'methods'
**   Unknown Method          0
**   TruncatedMean           1
**   EnsembleTruncatedMean   2
**   LikelihoodFit           3 
**   WeightedTruncatedMean   4
**   OtherMethod             5        */

/* numbering scheme for Method types */

#define kUndefinedMethodIdentifier           0  ///< Unknown or undefined dE/dx method
#define kTruncatedMeanIdentifier             1  ///< Truncated-mean dE/dx estimator
#define kEnsembleTruncatedMeanIdentifier     2  ///< Ensemble truncated-mean dE/dx estimator
#define kLikelihoodFitIdentifier             3  ///< Likelihood-fit dE/dx estimator
#define kWeightedTruncatedMeanIdentifier     4  ///< Weighted truncated-mean dE/dx estimator
#define kOtherMethodIdentifier               5  ///< Other dE/dx method (first spare)
#define kOtherMethodIdentifier2              6  ///< Other dE/dx method (second spare)
#endif

