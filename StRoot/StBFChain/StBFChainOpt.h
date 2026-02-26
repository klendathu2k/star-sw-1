#ifndef STAR_StBFChainOpt
#define STAR_StBFChainOpt

//////////////////////////////////////////////////////////////////////////
/*!

 \class  StBFChainOpt
 \author Victor Perev, 
 \date   2006/04/17 

 \brief Adapter exposing StBFChain configuration through the StChainOpt interface.

 Downstream makers that need to query the active BFC chain configuration
 (input/output filenames, TFile handle, geometry tag) use the \c StChainOpt
 abstract interface rather than depending directly on \c StBFChain.
 \c StBFChainOpt bridges the two by delegating each query to the owning
 \c StBFChain instance.

*/
//////////////////////////////////////////////////////////////////////////
#include "StChainOpt.h"
class StBFChain;
class StBFChainOpt : public StChainOpt {
public:
StBFChainOpt(StBFChain *bfc);
virtual ~StBFChainOpt(){};
//Int_t kOpt(const char *Tag)	const;
const TString &GetFileIn()  	const;
const TString &GetFileOut() 	const;
      TFile   *GetTFile()	const;
      TString  GetGeometry()  	const;
private:
StBFChain *fBFChain;

   ClassDef(StBFChainOpt, 0)
};
#endif
