// Author: Victor Perev   08/04/01

/*!
 * \file TTreeHelper.h
 * \brief Thin TTreeIter alias provided for backward compatibility.
 */

#ifndef ROOT_TTreeHelper
#define ROOT_TTreeHelper


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTreeHelper                                                          //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TTreeIter.h"

/*!
 * \class TTreeHelper
 * \brief Thin alias of TTreeIter provided for backward compatibility.
 * \details Adds no new functionality; all methods are inherited from TTreeIter.
 */
class TTreeHelper : public TTreeIter {

public:

    TTreeHelper(TTree *tree):TTreeIter(tree){}
    TTreeHelper(const char *treeName=""):TTreeIter(treeName){}
    virtual ~TTreeHelper(){};

ClassDef(TTreeHelper,0)    
};
#endif //ROOT_TTreeHelper
