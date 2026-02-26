/*!
 * \file xTCL.h
 * \brief Static linear-algebra utilities complementing TCL/TCernLib.
 */
/*!
 * \class xTCL
 * \brief Static linear-algebra utilities complementing TCL/TCernLib: matrix operations,
 *        eigenvalues, Simpson integration, and quadratic programming.
 * \details All methods are static; xTCL is never instantiated.
 */
/***************************************************************************
 *
 * $Id: xTCL.h,v 1.4 2014/02/18 19:45:49 perev Exp $
 *
 * Author: Victor Perev, Feb 2007
 ***************************************************************************
 *
 * Description:
 *
 * Set of useful routines in style of TCL, but not in TCL
 *
 *
 **************************************************************************/
#ifndef xTCL_h
#define xTCL_h
#include "TMath.h"
#if ROOT_VERSION_CODE < 331013
#include "TCL.h"
#else
#include "TCernLib.h"
#endif
#include "TMatrixD.h"
#include "TVectorD.h"


class xTCL {
public:
  /// Return the maximum absolute value in array \p a of length \p na.
  static double vmaxa  (const double   *a, int na);
  /// Return the maximum absolute value in TVectorD \p a.
  static double vmaxa  (const TVectorD &a);
  /// Return the index of the element with the largest absolute value in \p v (length \p n).
  static int    lvmaxa (const double *v,int n);
  /// Return the index of the element with the smallest absolute value in \p v (length \p n).
  static int    lvmina (const double *v,int n);
  /// Fill array \p a (length \p na) with constant value \p f.
  static void   vfill  (      double *a,double f,int na);
  /// Compute X = A * B * A^T (TMatrixD version).
  static void   mxmlrt (const TMatrixD &A,const TMatrixD &B,TMatrixD &X);
  /// Compute X = A * B * A^T assuming B is symmetric (TMatrixD version).
  static void   mxmlrtS(const TMatrixD &A,const TMatrixD &B,TMatrixD &X);
  /// Compute X = A * B * A^T assuming B is symmetric (raw array version; nra×nca).
  static void   mxmlrtS(const double *A,const double *B,double *X,int nra,int nca);
  /// Return the transpose of matrix \p mx.
  static TMatrixD T(const TMatrixD &mx);
  /// Compute the two eigenvalues \p lam[2] and corresponding eigenvectors \p eig[2] of a 2×2 symmetric matrix \p err[3].
  static void   eigen2 (const double err[3], double lam[2], double eig[2]);
  /// Integrate array \p F of \p NP+1 evenly-spaced values over [A, B] using Simpson's rule.
  static double simpson(const double *F,double A,double B,int NP);
  /// Return the sum of absolute values of array \p a of length \p na.
  static double vasum  (const double *a, int na);
  /// Return the sum of absolute values of TVectorD \p a.
  static double vasum  (const TVectorD &a);
  /// Solve a simple quadratic program: minimise 0.5 x^T G x + g^T x subject to Min ≤ x ≤ Max.
  static int SqProgSimple(      TVectorD &x
                         ,const TVectorD &g,const TMatrixD &G
                         ,const TVectorD &Min
                         ,const TVectorD &Max,int iAkt);
  /// Allocate a 2D matrix of doubles with dimensions \p m × \p n.
  static double **makeMatrixD(int m,int n);
  /// Extract Euler angles (phi, theta, psi and their cosines/sines) from a 3×3 rotation matrix.
  static void toEuler(const double TT[3][3],double PhiThePsi[6]);


};
#endif// xTCL_h
