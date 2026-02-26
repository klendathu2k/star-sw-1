/*!
 * \class StRichSpectra 
 * \author Brian Lasiuk, Dec 14, 2002
 */
/***************************************************************************
 *
 * $Id: StRichSpectra.h,v 2.6 2014/08/06 11:43:08 jeromel Exp $
 *
 * Author: Brian Lasiuk, Dec 14, 2002
 ***************************************************************************
 *
 * Description: Output from StRichSpectraMaker for uDST storage
 *
 ***************************************************************************
 * $Log: StRichSpectra.h,v $
 * Revision 2.6  2014/08/06 11:43:08  jeromel
 * Suffix on literals need to be space (later gcc compiler makes it an error) - first wave of fixes
 *
 * Revision 2.5  2003/09/02 17:58:05  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.4  2002/02/22 22:56:50  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2002/02/19 23:22:30  ullrich
 * Minor modifications.
 *
 * Revision 2.2  2002/02/19 16:54:33  ullrich
 * Minor changes - code not altered.
 *
 * Revision 2.1  2002/02/19 04:24:03  lasiuk
 * addition of StRichSpectra information for uDST purposes
 *
 **************************************************************************/
#ifndef StRichSpectra_hh
#define StRichSpectra_hh

/// @file StRichSpectra.h
/// @brief Summary of RICH Cherenkov spectra and PID quantities for a single track, stored for uDST output.

#include <Stiostream.h>
#include <float.h>
#include "StObject.h"

/// @brief Cherenkov spectra output from StRichSpectraMaker: reconstructed angles, photon counts,
/// mass-squared, and per-hypothesis mean-distance quantities for a single track.
class StRichSpectra : public StObject {
public:
    /// @brief Constructor; @p v sets the production version tag (default -999 = unset).
    StRichSpectra(int v=-999);
    ~StRichSpectra();
    /// @brief Full constructor with all spectra quantities.
    StRichSpectra(float,float,float,float,float,float,
		  float,float,int,float,int,int,
		  float,float,float,float,int,float,
		  float, float, float,
		  int, int, int,
		  int=-999);

    //StRichSpectra(const StRichSpectra&) use default
    //StRichSpectra& operator =(const StRichSpectra&) use default

    /// @brief Sets the extrapolated track impact position on the RICH pad plane (x, y).
    void setExtrapolatedPosition(float, float);
    /// @brief Sets the residual between the extrapolated and MIP-hit positions (dx, dy).
    void setExtrapolatedResidual(float, float);
    /// @brief Sets the corrected extrapolation residual (dx, dy).
    void setCorrectedExtrapolatedResidual(float, float);
    /// @brief Sets the reconstructed mean Cherenkov angle.
    void setCherenkovAngle(float);
    /// @brief Sets the sigma of the Cherenkov angle distribution.
    void setCherenkovSigma(float);
    /// @brief Sets the number of Cherenkov photons used in the angle reconstruction.
    void setCherenkovPhotons(int);
    /// @brief Sets the peak angle in the Cherenkov photon distribution.
    void setPeakAngle(float);
    /// @brief Sets the number of photons at the Cherenkov angle peak.
    void setPeakPhotons(int);
    /// @brief Sets the total number of photon candidates on the ring.
    void setTotalPhotons(int);
    /// @brief Sets the reconstructed mass squared of the particle.
    void setMassSquared(float);
    /// @brief Sets the ratio of the Cherenkov ring line integral.
    void setLineIntegralRatio(float);
    /// @brief Sets the Cherenkov ring line integral.
    void setLineIntegral(float);
    /// @brief Sets the alpha photon-yield correction factor.
    void setAlpha(float);
    /// @brief Sets the status flag word.
    void setFlag(int);
    /// @brief Sets the reserved float field.
    void setReserved(float);

    /// @brief Sets the mean distance from the Cherenkov ring for pion, kaon, and proton hypotheses.
    void setMeanD(float pi=FLT_MAX,  float k=FLT_MAX, float p=FLT_MAX);
    /// @brief Sets the number of photons used in the mean-D calculation for each hypothesis.
    void setNumberOfD(int pi=-100,  int k=-100, int p=-100);
    /// @brief Sets the production version tag.
    void setVersion(int);

    /// @brief Returns the extrapolated x position of the track on the RICH pad plane.
    float getExtrapolatedX() const;
    /// @brief Returns the extrapolated y position of the track on the RICH pad plane.
    float getExtrapolatedY() const;
    /// @brief Returns the x-component of the extrapolation residual.
    float getExtrapolatedXResidual() const;
    /// @brief Returns the y-component of the extrapolation residual.
    float getExtrapolatedYResidual() const;
    /// @brief Returns the x-component of the corrected extrapolation residual.
    float getCorrectedExtrapolatedXResidual() const;
    /// @brief Returns the y-component of the corrected extrapolation residual.
    float getCorrectedExtrapolatedYResidual() const;
    /// @brief Returns the reconstructed mean Cherenkov angle.
    float getCherenkovAngle() const;
    /// @brief Returns the sigma of the Cherenkov angle distribution.
    float getCherenkovSigma() const;
    /// @brief Returns the number of Cherenkov photons used in the angle reconstruction.
    int   getCherenkovPhotons() const;
    /// @brief Returns the peak angle in the Cherenkov photon distribution.
    float getPeakAngle() const;
    /// @brief Returns the number of photons at the Cherenkov angle peak.
    int   getPeakPhotons() const;
    /// @brief Returns the total number of photon candidates on the ring.
    int   getTotalPhotons() const;
    /// @brief Returns the reconstructed mass squared of the particle.
    float getMassSquared() const;
    /// @brief Returns the Cherenkov ring line-integral ratio.
    float getLineIntegralRatio() const;
    /// @brief Returns the Cherenkov ring line integral.
    float getLineIntegral() const;
    /// @brief Returns the alpha photon-yield correction factor.
    float getAlpha() const;
    /// @brief Returns the status flag word.
    int   getFlag() const;
    /// @brief Returns the mean distance from the pion Cherenkov ring hypothesis.
    float getMeanDpi() const;
    /// @brief Returns the mean distance from the kaon Cherenkov ring hypothesis.
    float getMeanDk()  const;
    /// @brief Returns the mean distance from the proton Cherenkov ring hypothesis.
    float getMeanDp()  const;
    /// @brief Returns the number of photons in the pion mean-D calculation.
    int   getMeanDnpi()  const;
    /// @brief Returns the number of photons in the kaon mean-D calculation.
    int   getMeanDnk()  const;
    /// @brief Returns the number of photons in the proton mean-D calculation.
    int   getMeanDnp()  const;

    /// @brief Returns the reserved float field.
    float getReserved() const;
    /// @brief Returns the production version tag.
    int   getVersion() const;

protected:
    Float_t mExtrapolatedX;         ///< Extrapolated track x position on the RICH pad plane
    Float_t mExtrapolatedY;         ///< Extrapolated track y position on the RICH pad plane
    Float_t mDx;                    ///< x-component of extrapolation residual
    Float_t mDy;                    ///< y-component of extrapolation residual
    Float_t mCdx;                   ///< x-component of corrected extrapolation residual
    Float_t mCdy;                   ///< y-component of corrected extrapolation residual
    Float_t mCherenkovAngle;        ///< Reconstructed mean Cherenkov angle (radians)
    Float_t mCherenkovAngleSigma;   ///< Sigma of the Cherenkov angle distribution (radians)
    Int_t   mNumberOfPhotons;       ///< Number of Cherenkov photons used in angle reconstruction
    Float_t mPeakAngle;             ///< Peak angle in the Cherenkov photon distribution (radians)
    Int_t   mPeakPhotons;           ///< Number of photons at the Cherenkov angle peak
    Int_t   mTotalPhotons;          ///< Total number of photon candidates on the ring
    Float_t mMassSquared;           ///< Reconstructed mass squared of the particle (GeV²/c⁴)
    Float_t mLineIntegralRatio;     ///< Ratio of the Cherenkov ring line integral
    Float_t mLineIntegral;          ///< Cherenkov ring line integral
    Float_t mAlpha;                 ///< Photon-yield correction factor alpha
    Int_t   mFlag;                  ///< Status flag word
    Float_t mReserved;              ///< Reserved float (future use)

    Float_t mDpi;   ///< Mean distance from the pion Cherenkov ring hypothesis
    Float_t mDk;    ///< Mean distance from the kaon Cherenkov ring hypothesis
    Float_t mDp;    ///< Mean distance from the proton Cherenkov ring hypothesis
    Int_t   mNDpi;  ///< Number of photons used in pion mean-D calculation
    Int_t   mNDk;   ///< Number of photons used in kaon mean-D calculation
    Int_t   mNDp;   ///< Number of photons used in proton mean-D calculation

    Int_t mVersion; ///< Production version tag
    //
    // Required Track Parameters:
    // innerTrackHelix
    // outerTrackHelix
    // charge
    // firstPointonTrack(x,y,z)
    // lastPointonTrack(x,y,z)
    // chi2
    // numberOfFitPoints

    virtual const char *GetCVS() const	{
	static const char cvs[]=
	    "Tag $Name:  $ $Id $ built " __DATE__ " " __TIME__ ;
	return cvs;
    }
private:
    ClassDef(StRichSpectra, 1)
};

inline void StRichSpectra::setExtrapolatedPosition(float x, float y) {mExtrapolatedX = x;mExtrapolatedY=y;}
inline void StRichSpectra::setExtrapolatedResidual(float dx, float dy) {mDx=dx;mDy=dy;}
inline void StRichSpectra::setCorrectedExtrapolatedResidual(float dx, float dy) {mCdx=dx;mCdy=dy;}
inline void StRichSpectra::setCherenkovAngle(float theta) {mCherenkovAngle=theta;}
inline void StRichSpectra::setCherenkovSigma(float s) {mCherenkovAngleSigma=s;}
inline void StRichSpectra::setCherenkovPhotons(int n) {mNumberOfPhotons=n;}
inline void StRichSpectra::setPeakAngle(float pa) {mPeakAngle=pa;}
inline void StRichSpectra::setPeakPhotons(int np) {mPeakPhotons=np;}
inline void StRichSpectra::setTotalPhotons(int tp) {mTotalPhotons=tp;}
inline void StRichSpectra::setMassSquared(float m2) {mMassSquared=m2;}
inline void StRichSpectra::setLineIntegralRatio(float lir) {mLineIntegralRatio=lir;}
inline void StRichSpectra::setLineIntegral(float li) {mLineIntegral = li;}
inline void StRichSpectra::setAlpha(float alpha) {mAlpha=alpha;}
inline void StRichSpectra::setFlag(int f) {mFlag = f;}
inline void StRichSpectra::setMeanD(float pi,  float k, float p) {mDpi=pi; mDk=k, mDp=p;}
inline void StRichSpectra::setNumberOfD(int pi,  int k, int p) {mNDpi=pi; mNDk=k; mNDp=p;}
inline void StRichSpectra::setReserved(float r) {mReserved = r;}
inline void StRichSpectra::setVersion(int v) {mVersion = v;}

inline float StRichSpectra::getExtrapolatedX() const {return  mExtrapolatedX;}
inline float StRichSpectra::getExtrapolatedY() const {return  mExtrapolatedY;}
inline float StRichSpectra::getExtrapolatedXResidual() const {return  mDx;}
inline float StRichSpectra::getExtrapolatedYResidual() const {return  mDy;}
inline float StRichSpectra::getCorrectedExtrapolatedXResidual() const {return  mCdx;}
inline float StRichSpectra::getCorrectedExtrapolatedYResidual() const {return  mCdy;}
inline float StRichSpectra::getCherenkovAngle() const {return  mCherenkovAngle;}
inline float StRichSpectra::getCherenkovSigma() const {return  mCherenkovAngleSigma;}
inline int   StRichSpectra::getCherenkovPhotons() const {return  mNumberOfPhotons;}
inline float StRichSpectra::getPeakAngle() const {return  mPeakAngle;}
inline int   StRichSpectra::getPeakPhotons() const {return  mPeakPhotons;}
inline int   StRichSpectra::getTotalPhotons() const {return  mTotalPhotons;}
inline float StRichSpectra::getMassSquared() const {return  mMassSquared;}
inline float StRichSpectra::getLineIntegralRatio() const {return  mLineIntegralRatio;}
inline float StRichSpectra::getLineIntegral() const {return  mLineIntegral;}
inline float StRichSpectra::getAlpha() const {return  mAlpha;}
inline int   StRichSpectra::getFlag() const {return  mFlag;}
inline float StRichSpectra::getMeanDpi() const { return mDpi; }
inline float StRichSpectra::getMeanDk()  const { return mDk; }
inline float StRichSpectra::getMeanDp()  const { return mDp; }
inline int   StRichSpectra::getMeanDnpi()  const { return mNDpi; }
inline int   StRichSpectra::getMeanDnk()  const { return mNDk; }
inline int   StRichSpectra::getMeanDnp()  const { return mNDp; }

inline float StRichSpectra::getReserved() const {return  mReserved;}
inline int   StRichSpectra::getVersion() const {return  mVersion;}

//non-members
ostream& operator<<(ostream&, const StRichSpectra&);
#endif
