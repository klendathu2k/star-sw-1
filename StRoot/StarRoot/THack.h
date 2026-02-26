/*!
 * \file THack.h
 * \brief Collection of static utility functions for low-level ROOT object manipulation.
 */
#ifndef ROOT_THack
#define ROOT_THack


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// THack                                                                //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
class TClonesArray;
class TPad;
class TDirectory;
class TString;
class TTree;

/*!
 * \class THack
 * \brief Static utility class for low-level ROOT object manipulation not covered by the public API.
 * \details Provides helpers for safe TClonesArray deletion/clearing, pad refresh, histogram
 *          directory release, tokenised line parsing, and TTree writability checks.
 */
class THack 
{
public:
static void DeleteClonesArray(TClonesArray *clone);
static void ClearClonesArray(TClonesArray *clone);
static void PadRefresh(TPad *pad,int flag=0);
static void HistRelease(TDirectory *dir);
static int  LineToD(const char *line, const char **lend,
		    int nItems, double *Items, TString *Names=0);
static bool IsTreeWritable(const TTree *tree, bool fatal=true);
};
#endif //ROOT_THack
