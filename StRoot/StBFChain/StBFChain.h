#ifndef STAR_StBFChain
#define STAR_StBFChain

//////////////////////////////////////////////////////////////////////////
/*!

 \class  StBFChain
 \author Yuri Fisyak, Jerome LAURET
 \date   1999/07/29 , 2001-2011
 @(#)StRoot/StBFChain:$Name:  $:$Id: StBFChain.h,v 1.59 2019/11/19 17:26:17 jeromel Exp $

 \brief STAR Big Full Chain (BFC) controller.

 StBFChain translates a user-supplied comma/space-separated option string into
 a fully configured STAR maker chain.  The available options (~1,062 entries)
 are defined in \c BigFullChain.h and loaded at runtime from the ROOT macro
 \c BFC.C.

 Typical usage:
 \code
   StBFChain *chain = new StBFChain("bfc");
   chain->Setup();
   chain->SetFlags("P2014a,btof,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D");
   chain->Set_IO_Files(inputFile, outputFile);
   chain->Load();        // load required shared libraries
   chain->Instantiate(); // construct and configure makers
   chain->Init();
   while (chain->Make() == kStOk) {}
   chain->Finish();
 \endcode

 \section dep Dependency resolution

 Each BFC option entry has an \c Opts field listing prerequisite option keys
 (a `-` prefix disables an option).  When an option is activated via
 SetOption(), its dependencies are recursively enabled depth-first, ensuring
 all prerequisites are satisfied before the option itself is marked active.
 See doc/README.md for a detailed description.

 This class:
   - Parses the option string and resolves transitive option dependencies
   - Loads shared libraries required by active options
   - Instantiates and configures the appropriate maker objects
   - Provides API to query and modify option state at runtime

 \sa StBFChainOpt, Bfc_st, BigFullChain.h, doc/README.md

*/
//////////////////////////////////////////////////////////////////////////
#include "StChain.h"
#include "TFile.h"
#include "TTable.h"
#include "Ttypes.h"
#include "Bfc.h"
//_____________________________________________________________________

class StFileI;
class TObjArray;

class StBFChain : public StChain {
 private:
  Bfc_st              *fBFC;      // Private chain
  StFileI             *fSetFiles; //
  TString             fInFile;    //
  TString             fFileOut;   //
  TFile              *fTFile;
  Int_t               FDate;      // floating timestamp date (MaxDateTime)
  Int_t               FTime;      // floating timestamp time (unused)
  Int_t               FDateS;     // floating timestamp date (DateTime)
  Int_t               FTimeS;     // floating timestamp time (DateTime)
  TString             fFiltTrg;   // trigger filtering properties (flavor, inc/exc)
  Int_t               fRunG;      // on fly simulation run no. & set for RDNM
  TString             fFmsGainCorrTag; // FMS GainCorrection Flavor Tag
  Int_t               fNoChainOptions;
  vector<TString>     Gproperty;  // a global property name
  vector<TString>     Gpattern;   // a global pattern
  vector<TString>     Gvalue;     // a global value
#ifdef USE_BFCTIMESTAMP
  StVecBFCTS          GTSOptions; // global set of detector specific timestamps
#endif /*  USE_BFCTIMESTAMP */
  St_Bfc             *fchainOpt;
  Int_t               fkChain;    // Master chain option

 public:
  /// Constructor. Call Setup() after construction to load the option table.
  StBFChain(const char *name="bfc", const Bool_t UseOwnHeader = kFALSE) :
           StChain(name,UseOwnHeader)
	     ,fBFC(0), fSetFiles(0),fInFile(""),fFileOut(""),fTFile(0)
	     ,FDate(0),FTime(0),FDateS(0),FTimeS(0),fFiltTrg(""),fRunG(0)
	     ,fNoChainOptions(0), fchainOpt(0), fkChain(-1) {}

#if 0
    StBFChain(Int_t /* mode */, const char *name="bfc",const Bool_t UseOwnHeader = kFALSE) :
            StChain(name,UseOwnHeader)
           ,fSetFiles(0),fInFile(""),fFileOut(""),fTFile(0)
	   ,fNoChainOptions(0), fchainOpt(0), fkChain(-1) {}
#endif
  /// Load the BFC option table from BFC.C and initialise internal state.
  /// Must be called once after construction before SetFlags().
  void Setup(Int_t mode=1);
   virtual            ~StBFChain();
   virtual Int_t       Make(int number){ SetIventNumber(number); return StChain::Make(number);};
   virtual Int_t       Make(){return StChain::Make();};
   /// Load shared libraries required by all active BFC options.
   virtual Int_t       Load();
   /// Instantiate and configure maker objects for all active BFC options.
   virtual Int_t       Instantiate();
   virtual Int_t       Init();
   /// Insert \a maker into the chain at the position relative to \a after.
   /// \a Opt=1 inserts after; \a Opt=-1 inserts before.
   virtual Int_t       AddAB (const Char_t *after="",const StMaker *maker=0,const Int_t Opt=1);
   virtual Int_t       AddAfter  (const Char_t *after, const StMaker *maker) {return AddAB (after,maker);}
   virtual Int_t       AddBefore (const Char_t *before,const StMaker *maker) {return AddAB (before,maker,-1);}
   /// Tokenise a comma/space-separated option string into a TObjArray.
   /// When \a Sort=kTRUE the tokens are ordered by their position in the BFC
   /// option table so that dependencies are processed before dependents.
   static  Int_t       ParseString (const TString &tChain, TObjArray &Opt, Bool_t Sort=kFALSE);
   /// Activate a set of BFC options from a space-separated string.
   /// A token prefixed with \c - disables the named option.
   /// Example: \c "P2014a,btof,BEmcChkStat,-tofDat"
   void                SetFlags(const Char_t *Chain="gstar tfs"); // *MENU*
   void                Set_IO_Files(const Char_t *infile=0, const Char_t *outfile=0); // *MENU
   void                SetInputFile(const Char_t *infile=0);                          // *MENU
   void                SetOutputFile(const Char_t *outfile=0);                        // *MENU
   void                SetTFile(TFile *tf)			{fTFile=tf;}
   TFile              *GetTFile() const			        {return fTFile;}
   /// Look up an option by key and return its index in the BFC table.
   /// Returns a positive index on match, negative for the \c -tag form,
   /// or 0 if not found.  Matching is case-insensitive against both the
   /// \c Key and \c Maker fields.  When \a Check=kTRUE (default) an unknown
   /// key triggers a warning.
   virtual Int_t       kOpt(const TString *Tag, Bool_t Check = kTRUE) const;
   /// \overload
   virtual Int_t       kOpt(const Char_t  *Tag, Bool_t Check = kTRUE) const;
   /// Apply database-related SetAttr and SetFlavor calls to the St_db_Maker.
   virtual void        SetDbOptions(StMaker *db=0);
   /// Apply GEANT/VMC-related SetAttr calls to the geometry/simulation maker.
   virtual void        SetGeantOptions(StMaker *geant=0);
   /// Apply tree-output configuration to StTreeMaker.
   virtual void        SetTreeOptions();
   /// Activate (k>0) or deactivate (k<0) the BFC option at index \a k.
   /// When activating, all options in \c fBFC[k].Opts are enabled first
   /// (recursive depth-first dependency expansion).
   virtual void        SetOption(const Int_t k, const Char_t *chain="Chain");
   /// \overload — looks up the option by string key before delegating.
   virtual void        SetOption(const Char_t*  Opt, const Char_t *chain="Chain") {SetOption(kOpt(Opt), chain);}
   /// \overload — TString variant.
   virtual void        SetOption(const TString* Opt, const Char_t *chain="Chain") {SetOption(kOpt(Opt),chain);}
   /// Activate a comma/space-separated set of BFC options.
   virtual void        SetOptions(const Char_t*  Opt, const Char_t *chain="Chain");
   /// Deactivate the named option (equivalent to SetOption with a \c - prefix).
   virtual void        SetOptionOff(const Char_t*  Opt, const Char_t *chain="Chain") {SetOption(-kOpt(Opt),chain);}
   /// \overload — TString variant.
   virtual void        SetOptionOff(const TString* Opt, const Char_t *chain="Chain") {SetOption(-kOpt(Opt),chain);}
   virtual Int_t       Finish();
   virtual Option_t*   GetOption() const{return TObject::GetOption();}
   /// Return \c kTRUE if BFC option at index \a k is active.
   virtual Bool_t      GetOption(const Int_t k)  const;
   /// Return \c kTRUE if the named BFC option is active.
   virtual Bool_t      GetOption(const TString *Opt, Bool_t Check = kTRUE) const {return GetOption(kOpt(Opt,Check));}
   /// \overload — char* variant.
   virtual Bool_t      GetOption(const Char_t  *Opt, Bool_t Check = kTRUE) const {return GetOption(kOpt(Opt,Check));}
   /// Return the comment/value string associated with the named option.
   /// Used to retrieve values set via the \c opt=value syntax.
   virtual Char_t     *GetOptionString(const Char_t  *);
   virtual const TString &GetFileIn()  const {return *(&fInFile);}
   virtual const TString &GetFileOut() const {return *(&fFileOut);}
                TString GetGeometry() const;
   virtual Long_t      ProcessLine(const char *line);
   virtual const char *GetCVS() const {
       static const char cvs[]="Tag $Name:  $ $Id: StBFChain.h,v 1.59 2019/11/19 17:26:17 jeromel Exp $ built " __DATE__ " " __TIME__ ;
       return cvs;
   }
   /// StBFChain control class
   ClassDef(StBFChain, 3)
};
#endif
