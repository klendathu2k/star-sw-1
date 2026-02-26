// Author: Victor Perev   08/04/01

/*!
 * \file TIdTruUtil.h
 * \brief Monte Carlo truth-identity accumulator: selects the dominant idTruth contributor.
 */

#ifndef ROOT_TIdTruUtil
#define ROOT_TIdTruUtil


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TIdTruUtil                                                           //
// IdTruth utility                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TNamed.h"
#include <map>

/*!
 * \class TIdTruUtil
 * \brief Accumulates Monte Carlo truth-identity (idTruth) contributions and returns the dominant one.
 * \details Accepts multiple (idTru, quality) pairs via Add(), then on GetIdTru()/GetQua() selects the
 *          idTruth with the largest summed quality weight.  Clear() resets the accumulator.
 */
class TIdTruUtil: public TNamed
{
public:
            TIdTruUtil(const char *name="");
           ~TIdTruUtil();
void        Clear(const char* opt=0);
void        Add(int idTru,int qua=100);
int         Size() const {return mSize;}
int         GetIdTru();
double      GetQua();

//		Data members
private:
void Eval();

private:
int mSize;
int mEvalted;
int mIdTru;
double mQua;
std::map      <int,double> mDetWt;
ClassDef(TIdTruUtil,0)

};
#endif //ROOT_TIdTruUtil
