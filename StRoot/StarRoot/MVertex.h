/*!
 * \file MVertex.h
 * \brief Simple vertex data structure with 3D position, covariance matrix, chi2, NDF, and contributor count.
 */
#ifndef MVertex_H
#define MVertex_H

/*!
 * \class MVertex
 * \brief Lightweight vertex representation storing (x, y, z), a 6-element covariance matrix,
 *        chi-square, degrees of freedom, and the number of contributing tracks.
 * \details Holds three position coordinates in \c fP[3] and a packed lower-triangular 3×3
 *          covariance matrix in \c fC[6] (elements C00, C10, C11, C20, C21, C22).
 *          No ROOT persistency; intended for KF-style vertex consumers.
 */
class MVertex
{
 public:
  MVertex() { }
  ~MVertex() { }

/*  MVertex(const MVertex& vVert); 
  MVertex& operator=(const MVertex& vVert);*/

  double GetX() const { return fP[0]; }
  double GetY() const { return fP[1]; }
  double GetZ() const { return fP[2]; }
  
  void   GetXYZ(double *position) const {position[0] = fP[0]; position[1] = fP[1]; position[2] = fP[2];}
  void   GetCovarianceMatrix(double *covmatrix) const
         {
           for (int i=0; i<6; i++)
             covmatrix[i] = fC[i];
         }

  double GetChi2perNDF() const { return fChi2/fNDF; }
  double GetChi2()       const { return fChi2;      }
  int    GetNDF()        const { return fNDF; }
  int    GetNContributors() const { return fNContributors; }
  
  double GetParameter(int i) const { return fP[i]; }
  double GetCovariance(int i) const { return fC[i]; }


  void SetXYZ(double *position) { fP[0] = position[0]; fP[1] = position[1]; fP[2] = position[2]; }
  void SetXYZ(double x, double y, double z) { fP[0] = x; fP[1] = y; fP[2] = z; }
  void SetX(double x)      { fP[0] = x; }
  void SetY(double y)      { fP[1] = y; }
  void SetZ(double z)      { fP[2] = z; }
  void SetChi2(double chi) { fChi2 = chi; }
  void SetNDF(int ndf)     { fNDF = ndf; }
  void SetNContributors(int nc)  { fNContributors = nc; }

  void SetCovarianceMatrix(double *C)
       {
         for (int i=0; i<6; i++)
           fC[i] = C[i];
       }

  void SetCovarianceMatrix(double C00,double C10,double C11,double C20,double C21,double C22)
  {
    fC[0] = C00;
    fC[1] = C10;
    fC[2] = C11;
    fC[3] = C20;
    fC[4] = C21;
    fC[5] = C22;
  }

 private:

  double fP[3];           ///< Vertex position: (x, y, z) in cm
  double fC[6];           ///< Packed lower-triangular 3×3 covariance matrix: C00, C10, C11, C20, C21, C22
  double fChi2;           ///< Chi-square of the vertex fit
  int fNContributors;     ///< Number of tracks used to reconstruct this vertex
  int fNDF;               ///< Number of degrees of freedom of the vertex fit
};

#endif
