/*!
 * \class StRichCluster 
 * \author bl
 */
/***************************************************************************
 *
 * $Id: StRichCluster.h,v 2.5 2002/02/22 22:56:49 jeromel Exp $
 *
 * Author: bl
 ***************************************************************************
 *
 * Description: Definition of the Cluster  object (Persistent)
 *
 ***************************************************************************
 *
 * $Log: StRichCluster.h,v $
 * Revision 2.5  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.4  2001/04/05 04:00:39  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.3  2000/08/09 14:11:51  perev
 * ClassDef must be without ;, (???)
 *
 * Revision 2.2  2000/08/08 14:42:14  ullrich
 * Added missing ClassDef and ClassImp macros.
 *
 * Revision 2.1  2000/05/22 21:44:56  ullrich
 * Initial Revision
 *
 ***************************************************************************/
#ifndef StRichCluster_hh
#define StRichCluster_hh

/// @file StRichCluster.h
/// @brief Persistent RICH pad-cluster object used in Cherenkov ring reconstruction.
#include "StObject.h"

/// @brief Cluster of adjacent RICH pads grouped during hit reconstruction.
class StRichCluster : public StObject {
public:
    /// @brief Default constructor.
    StRichCluster();
    ~StRichCluster();
    /// @brief Constructor with full cluster parameters.
    StRichCluster(int nPads, int nLocMax, int fPad, float ampSum, float amp2Sum, float rms2);
    //StRichCluster(const StRichCluster&) {}
    //StRichCluster& operator=(const StRichCluster&) {}

    int    operator==(const StRichCluster&) const;

    /// @brief Returns the number of pads associated with this cluster.
    int    numberOfPads()        const;
    /// @brief Returns the number of local maxima in this cluster.
    int    numberOfLocalMax()    const;
    /// @brief Returns the index of the first pad in this cluster.
    int    firstPad()            const;
    /// @brief Returns the minimum amplitude among all local maxima.
    float  minimumAmplitudeOfLocalMax() const;
    /// @brief Returns the sum of all pad amplitudes in the cluster.
    float  amplitudeSum()        const;
    /// @brief Returns the sum of squares of all pad amplitudes.
    float  amplitude2Sum()       const;
    /// @brief Returns the stored square RMS of pad amplitudes.
    float  rms2()                const;
    /// @brief Computes and stores the square RMS of pad amplitudes.
    float  rms2Calc();

    /// @brief Increments the local-maximum count by one.
    void   increaseNumberOfLocalMax();
    /// @brief Increments the pad count by one.
    void   increaseNumberOfPads();
    /// @brief Sets the index of the first pad in the cluster.
    void   setFirstPad(int index);
    /// @brief Sets the total number of pads in the cluster.
    void   setNumberOfPads(int newNPads);
    /// @brief Adds a pad amplitude to the running amplitude sums.
    void   updateAmplitude(float newamp);
    /// @brief Sets the minimum local-maximum amplitude threshold.
    void   setMinimumAmplitudeOfLocalMax(float newLocMax);

private:
    Int_t    mNumberOfPads;                 ///< Number of associated pads
    Int_t    mNumberOfLocalMax;             ///< Number of local maxima in the cluster
    Int_t    mFirstPad;                     ///< Index of first cluster pad
    Float_t  mMinimumAmplitudeOfLocalMax;   ///< Lowest amplitude among all associated local maxima
    Float_t  mAmplitudeSum;                 ///< Sum of all pad amplitudes
    Float_t  mAmplitude2Sum;                ///< Sum of squares of all pad amplitudes
    Float_t  mRms2;                         ///< Square RMS of pad amplitudes
    ClassDef(StRichCluster,1)
};

inline int StRichCluster::numberOfPads() const { return mNumberOfPads; }
inline int StRichCluster::numberOfLocalMax() const { return mNumberOfLocalMax; }
inline int StRichCluster::firstPad() const { return mFirstPad; }
inline float StRichCluster::minimumAmplitudeOfLocalMax() const { return mMinimumAmplitudeOfLocalMax; }
inline float StRichCluster::amplitudeSum() const { return mAmplitudeSum; }
inline float StRichCluster::amplitude2Sum() const { return mAmplitude2Sum; }
inline float StRichCluster::rms2() const { return mRms2; }
inline void StRichCluster::increaseNumberOfLocalMax() { mNumberOfLocalMax++; }
inline void StRichCluster::increaseNumberOfPads() { mNumberOfPads++; }
inline void StRichCluster::setFirstPad(int index) { mFirstPad=index; }
inline void StRichCluster::setNumberOfPads(int newNPads)  { mNumberOfPads=newNPads; }
inline void StRichCluster::updateAmplitude(float newamp)
{
    mAmplitudeSum += newamp;
    mAmplitude2Sum += newamp*newamp;
}
inline float StRichCluster::rms2Calc()
{ return (mRms2 = mAmplitude2Sum/mNumberOfPads - (mAmplitudeSum*mAmplitudeSum/mNumberOfPads/mNumberOfPads)); }
inline void StRichCluster::setMinimumAmplitudeOfLocalMax(float newLocMax)
{ mMinimumAmplitudeOfLocalMax = newLocMax; }
#endif

