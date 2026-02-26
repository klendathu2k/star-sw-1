/*!
 * \file TDirIter.h
 * \brief Recursive directory iterator with optional regular-expression file filtering.
 */
#ifndef ROOT_TDirIter
#define ROOT_TDirIter


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDirIter                                                             //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TString.h"
#include "TRegexp.h"

class TOBjArray;

/*!
 * \class TDirIter
 * \brief Recursive directory iterator with optional regex-based file filtering.
 * \details Traverses a directory tree up to \c maxlev levels deep, yielding one file path
 *          per call to NextFile().  A glob-style pattern embedded in the initial path
 *          string is converted to a TRegexp for file selection.
 */
class TDirIter
{
public:
   TDirIter(const char *path, Int_t maxlev = 99);
  ~TDirIter();
void 	Reset  (const char *path, Int_t maxlev = 99);
const char *NextFile();

private:
TString 	MakeWild(const char *re);
const char 	*NextFileQ();
const char 	*NextFileQQ();
void 		ResetQQ(const char *path);

//              Data members
   Int_t fMaxLev; 
   Int_t fMaxLevQQ; 
   Int_t fLevel; 
   Int_t fState; 
   Int_t fSele; 
   Int_t fSkip; 
   Int_t fTop; 
   TString fFull;
   TString fFile;
   TRegexp fRegx;
   void   *fEntrStk[100];
   Int_t   fLengStk[100];
   TObjArray *fArr;
   Int_t fIter;
   
//   ClassDef(TDirIter,1)

};
#endif //ROOT_TDirIter
