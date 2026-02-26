// @(#)root/base:$Name:  $:$Id: TNumDeriv.h,v 1.2 2007/10/24 22:45:01 perev Exp $
// Author: Victor Perev   05/07/03

/*!
 * \file TNumDeriv.h
 * \brief Abstract numerical-derivative base class and test implementations.
 */
#ifndef ROOT_TNumDeriv
#define ROOT_TNumDeriv
#include <math.h>
#include "TNamed.h"

/*!
 * \class TNumDeriv
 * \brief Abstract base for numerical differentiation; override Fcn() to define the function.
 * \details Uses a symmetric finite-difference formula with adaptive step control.
 *          Call DFcn() to obtain the numerical derivative dFcn/dPar at the current argument.
 */
class TNumDeriv : public TNamed {
public:
           TNumDeriv(const char *name):TNamed(name,"") {fStep=0.01;fIArg=0;fOutLim=0;};
  virtual ~TNumDeriv(){}
  /// Pure-virtual function to differentiate; \p add is the perturbation added to the active argument.
  virtual Double_t   Fcn(Double_t  add=0.)=0;
  /// Return the numerical derivative dFcn/dPar at the current argument using the current step size.
  Double_t  DFcn(Double_t  add=0.);
          void      SetOutLimit(int lim=1) 	{fOutLim=lim	;}
  /// Set the finite-difference step size (default 0.01).
  void      SetStep(Double_t step)	{fStep  =step	;}
  /// Select which argument index is differentiated.
  void      SetIArg(Int_t iarg)		{fIArg  =iarg	;}
          Int_t     GetIArg()          		{return fIArg	;}

	  Double_t  GetStep()			{return fStep	;}
  static  Double_t  Tiny();
  static  Double_t  Epsilon();

private:
double numericalDerivative(  double x ,double &delta, double &error);

protected:
Double_t fStep;  
Int_t    fIArg;
Int_t    fOutLim;
static Double_t fgTiny;
static Double_t fgEpsilon;
ClassDef(TNumDeriv,0)
};



/*!
 * \class TNumDeriv1Test
 * \brief Concrete TNumDeriv implementation for unit-testing first derivatives (f(x) = x³).
 */
class TNumDeriv1Test : public TNumDeriv {
public:
           TNumDeriv1Test(double x) :TNumDeriv("DerivTest") {fX=x;SetStep(1e-3);}
  virtual ~TNumDeriv1Test(){};
//  virtual Double_t   Fcn(Double_t  arg){return sin((fX+arg));}  	//Fcn 
    virtual Double_t   Fcn(Double_t  arg){return pow(fX+arg,3);}  	//Fcn 
//
  double fX;
ClassDef(TNumDeriv1Test,0)
};

/*!
 * \class TNumDeriv2Test
 * \brief Concrete TNumDeriv implementation for unit-testing second derivatives via TNumDeriv1Test.
 * \details Wraps a TNumDeriv1Test and returns its DFcn() as Fcn(), thereby computing d²f/dx².
 */
class TNumDeriv2Test : public TNumDeriv {
public:
           TNumDeriv2Test(double x) :TNumDeriv("DerivTest") {fDT=new TNumDeriv1Test(x);fDT->SetStep(0.1);}
  virtual ~TNumDeriv2Test() {delete fDT;};
  virtual Double_t   Fcn(Double_t  arg){return fDT->DFcn(arg);}  	//Fcn 
//
TNumDeriv1Test *fDT;
ClassDef(TNumDeriv2Test,0)
};


#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ class TNumDeriv;
#pragma link C++ class TNumDeriv1Test;
#pragma link C++ class TNumDeriv2Test;
#endif


#endif //ROOT_TNumDeriv












  
