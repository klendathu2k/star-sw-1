/*!
 * \file TH1Helper.h
 * \brief Histogram statistics helper that computes mean, RMS, and integral over a sub-range.
 */
#ifndef ROOT_TH1Helper
#define ROOT_TH1Helper


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TH1Helper                                                            //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TH1.h"

/*!
 * \class TH1Helper
 * \brief Computes mean, RMS, and integral of a TH1 over a specified bin or x-value range.
 * \details The range can be set by bin indices or by x-axis values.  Statistics are
 *          recomputed lazily when the setter is called.
 */
class TH1Helper: public TObject
{
public:
   TH1Helper(const TH1 *h=0, int binMin=0, int binMax=0);
   TH1Helper(const TH1 *h, double xMin, double xMax);
  ~TH1Helper();

void   Set(const TH1 *h, int binMin, int binMax);
void   Set(const TH1 *h, double xMin, double xMax);
double GetMean()     ; 
double GetMeanErr()  ; 
double GetRMS    ()  ;
double GetRMSErr ()  ;
int    GetNonZeros() const; 
double GetIntegral() ; 
double GetIntegErr() ;

static void SetCanRebin(TH1 *h,int axis=0);

private:
void   Build();
void   Aver ();

//              Data members
   const TH1* fH1;
   int        fBMin;
   int        fBMax;
   double     fXMin;
   double     fXMax;
   int        fNonZeros;
   double     fMom[5];
   ClassDef(TH1Helper,0)

};
#endif //ROOT_TH1Helper
