/// @file StiNodePars.h
/// @brief Kalman track-state parameter vector and covariance matrix for one track node.
///
/// @ingroup StiHitsAndTracks

#ifndef __StiNodePars_h_
#define __StiNodePars_h_
#include "Rtypes.h"

#define kEC       2.99792458e-4
#define kZEROHZ   2e-6			//minimal/zero mag field
#define kZEROPTI  1e-3			//minimal/zero 1/pt
#define kZEROCURV (kZEROHZ*kZEROPTI) 	//minimal/zero curvature
#define kBIGPT    10                    //Pt when energy loss ignored



/// @class StiNodePars
/// @brief Five-parameter Kalman track state at one detector surface.
///
/// Stores the track state in the local frame of the associated StiDetector, together
/// with the 5×5 covariance matrix `G` (stored as a symmetric lower-triangle in the
/// derived class StiNodeMatrix).
///
/// **Coordinate convention** (cm, GeV/c):
/// - `P[kX]`    = X position in local detector frame (normal to surface; redundant — always zero at a plane)
/// - `P[kY]`    = Y position in local frame (transverse, tangential)
/// - `P[kZ]`    = Z position in local frame (beam-axis direction)
/// - `P[kPhi]`  = η = sin(φ_local) — sine of local azimuthal crossing angle
/// - `P[kPtin]` = q/p_t [GeV⁻¹] with sign = sign(−qB); reciprocal transverse momentum
/// - `P[kTan]`  = tan λ — tangent of the dip angle (λ = polar angle from transverse plane)
/// - `P[kCurv]` = signed curvature κ = Hz × (q/p_t)
/// - `P[kHz]`   = B_z field component in special units: p_t [GeV/c] = Hz × R [cm]
///
/// The reset() member zeroes all parameters and sets `_cosCA = 1` (straight-line seed state).
/// @ingroup StiHitsAndTracks
class StiNodePars {
 public:	
  enum {kX=0,kY=1,kZ=2,kPhi=3,kPtin=4,kTan=5,kCurv=6,kHz=7};
  void reset(){memset(this,0,sizeof(StiNodePars));_cosCA=1;}
  void ready();
   int isZeroH() const { return fabs(P[kHz]) <= kZEROHZ;}
  StiNodePars &merge(double wt,StiNodePars &other);
  StiNodePars &operator=(const StiNodePars& fr);
  void rotate(double alfa);
  int  nan() const;
    /// accessors
  double  operator[](Int_t idx) const {return P[idx];}
  double &operator[](Int_t idx)       {return P[idx];}
  double x()    const 	{return P[kX];} 
  double y()    const 	{return P[kY];}//  local Y-coordinate of this track (reference plane)           		     
  double z()    const 	{return P[kZ];}//  local Z-coordinate of this track (reference plane)			     
  double eta()  const 	{return P[kPhi];}//  phi angle
  double phi()  const 	{return P[kPhi];}//  phi angle again
  double ptin() const 	{return P[kPtin];}//  signed invert pt [sign = sign(-qB)]					     
  double tanl() const 	{return P[kTan];}//  tangent of the track momentum dip angle
  double curv() const 	{return P[kCurv];}//  signed curvature [sign = sign(-qB)]					     
  double rxy2() const 	{return     (P[kX]*P[kX]+P[kY]*P[kY]);}
  double rxy()  const 	{return sqrt(P[kX]*P[kX]+P[kY]*P[kY]);}
  double hz()   const 	{return P[kHz];}//  Z component magnetic field in units Pt(Gev) = Hz * RCurv(cm)                  
  double &x()    	{return P[kX];}
  double &y()    	{return P[kY];}
  double &z()    	{return P[kZ];}
  double &eta()  	{return P[kPhi];} 
  double &ptin() 	{return P[kPtin];}
  double &tanl() 	{return P[kTan];}
  double &curv() 	{return P[kCurv];}
  double &hz()   	{return P[kHz];}
  double *A(Int_t i)    {return &P[i];}
  
  Int_t     check(const char *pri=0) const;
  void      print() const;

    /// sine and cosine of cross angle
  double _cosCA;
  double _sinCA;
  double P[8]; // array of parameters. see below
};

class StiELoss 
{
public:
float mELoss,mLen,mDens,mX0;
};

inline void StiNodePars::ready()
{
_cosCA=cos(P[kPhi]);_sinCA=sin(P[kPhi]); 
 if (fabs(P[kHz])<= kZEROHZ) {
   P[kCurv] =  kZEROCURV; P[kPtin]= kZEROPTI;
 } else {
   P[kCurv]  = P[kHz]*P[kPtin];
 }
}


#endif
