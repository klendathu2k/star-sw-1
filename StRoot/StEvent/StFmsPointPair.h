/**************************************************************************
 *
 * $Id: StFmsPointPair.h,v 2.4 2017/02/20 16:32:58 ullrich Exp $
 *
 * Author: Akio Ogawa, Sep 2015
 **************************************************************************
 *
 * Description: Declaration of StFmsPointPair, the StEvent FMS pi0/EMJet
 *
 **************************************************************************
 *
 * $Log: StFmsPointPair.h,v $
 * Revision 2.4  2017/02/20 16:32:58  ullrich
 * Changing F to D for StLorentzVector
 *
 * Revision 2.3  2016/06/07 15:51:34  akio
 * Making code better based on Coverity reports
 *
 * Revision 2.2  2015/10/21 14:52:54  ullrich
 * Added methods x() and y()
 *
 * Revision 2.1  2015/09/14 16:15:50  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StFmsPointPair_h
#define StFmsPointPair_h

/// @file StFmsPointPair.h
/// @brief Pair (or group) of FMS photon points for di-photon invariant mass analysis.

#include "StLorentzVectorD.hh"
#include "StThreeVectorD.hh"
#include "StObject.h"
#include "StEnumerations.h"
#include "StFmsPoint.h"

/// @brief A pair (or group) of FMS photon points used for pi0/eta/EMJet reconstruction.
class StFmsPointPair : public StObject {
public:
    /// @brief Default constructor.
    StFmsPointPair();
    /// @brief Constructor with a single photon point.
    StFmsPointPair(StFmsPoint* p);
    /// @brief Constructor from two photon points.
    StFmsPointPair(StFmsPoint* p1, StFmsPoint* p2);
    /// @brief Destructor.
    ~StFmsPointPair();
        
    /// @brief Return the number of photon points in this pair/group.
    int nPoints() const;
    /// @brief Add a photon point to this group.
    void addPoint(StFmsPoint* p);
    /// @brief Return all photon points.
    vector<StFmsPoint*>& points();
    /// @brief Return the photon point at index v.
    StFmsPoint* point(int v);
  
    /// @brief Return the combined four-momentum of all points.
    const StLorentzVectorD& fourMomentum() const;
    /// @brief Return the combined energy.
    float energy() const;
    /// @brief Return the combined transverse momentum.
    float pT() const;
    /// @brief Return the pseudorapidity.
    float eta() const;
    /// @brief Return the azimuthal angle.
    float phi() const;
    /// @brief Return the invariant mass of the point pair (GeV/c^2).
    float mass() const;
    float dgg() const;            ///< Opening distance between the two photons (meaningful for nPoints=2).
    float zgg() const;            ///< Energy asymmetry |E1-E2|/(E1+E2) (meaningful for nPoints=2).
    /// @brief Return the energy-weighted x centroid of the pair.
    float x() const;  
    /// @brief Return the energy-weighted y centroid of the pair.
    float y() const;;
    unsigned int fpsPid() const;  ///< FPS PID word: each decimal digit encodes one point's PID (1=gamma, 2=hadron, 3=electron).
                                  //for pair(npoint=2), 11=gg, 22=hh, 33=ee, 13=ge,etc
                                  //for nPoint>2, LSD is first point and MSD is last 

    enum StFmsPointPairConsts {kFmsPointMaxCone=3}; //!< Maximum number of isolation cone sizes.
    /// @brief Return the radius of isolation cone number @p cone.
    float coneRadius(int cone) const;
    /// @brief Return the FMS energy sum inside isolation cone number @p cone.
    float coneEnergy(int cone) const;
    /// @brief Return the fraction of pair energy inside isolation cone number @p cone.
    float coneEnergyFraction(int cone) const;
    /// @brief Set the energy deposited in isolation cone number @p cone.
    void setConeEnergy(int cone, float energy);

    /// @brief Print point-pair properties.
    void print(int option=0);

private:
    vector<StFmsPoint*> mPoints;    //!< Constituent photon points (transient).
    StLorentzVectorD mFourMomentum; //!< Combined four-momentum (transient).
    UInt_t  mFpsPid;                //!< Packed FPS PID word (transient).
    Float_t mConeRadius[kFmsPointMaxCone]; //!< Isolation cone radii (transient).
    Float_t mConeEnergy[kFmsPointMaxCone]; //!< Energy sums within each isolation cone (transient).

    ClassDef(StFmsPointPair, 1)
};

inline int StFmsPointPair::nPoints() const {return mPoints.size();}
inline vector<StFmsPoint*>& StFmsPointPair::points() {return mPoints;}
inline const StLorentzVectorD& StFmsPointPair::fourMomentum() const { return mFourMomentum; }
inline float StFmsPointPair::energy() const { return (float)mFourMomentum.e(); }
inline float StFmsPointPair::pT() const { return (float)mFourMomentum.perp(); }
inline float StFmsPointPair::eta() const { return (float)mFourMomentum.pseudoRapidity(); }
inline float StFmsPointPair::phi() const { return (float)mFourMomentum.phi(); }
inline float StFmsPointPair::mass() const { return (float)mFourMomentum.m(); }
inline unsigned int  StFmsPointPair::fpsPid() const { return mFpsPid; }
 
#endif  // StFmsPointPair_h

