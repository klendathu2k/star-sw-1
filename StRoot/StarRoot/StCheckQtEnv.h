//*-- Author :    Valery Fine(fine@bnl.gov)   27/10/2006
//
// $Id: StCheckQtEnv.h,v 1.3 2008/02/09 01:17:26 fine Exp $
/*!
 * \file StCheckQtEnv.h
 * \brief Qt/ROOT environment setup utilities: generates .rootrc plugin entries on the fly.
 */
// This class  sets the Qt/Root environment "on fly" and 
// generates the correct ROOT resource ".rootrc" file 
// also
#ifndef STAR_StCheckQtEnv
#define STAR_StCheckQtEnv

#include <stdio.h>
#include "TString.h"

/*!
 * \class StCheckQtEnv
 * \brief Static utilities to configure Qt/ROOT plugin entries in the ROOT resource file.
 * \details Provides helpers to open uniquely-named resource files, write plugin directives,
 *          and set the QTDIR / LD_LIBRARY_PATH environment variables needed for Qt-ROOT integration.
 */
class StCheckQtEnv {

public:
    static FILE *OpeFileName(const char *fileNamePrototype);
    static TString GetNewFileName(const char *fileNamePrototype);
    static Int_t SetRootResource(FILE *file, const char *plugin, 
                       const char *lib,
                       const char *full=0,Bool_t append=kFALSE); 

    static Long_t SetQtEnv(bool checkCoin=true);
};
#endif
