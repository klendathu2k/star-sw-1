/// \file StTree.h
/// \brief Tree/branch I/O classes for multi-file STAR event storage.
///
/// \details Provides StIO (low-level ROOT-file helpers), StBranch (single-file
/// branch of an event tree), StTree (collection of StBranch nodes),
/// StIOEvent (browser wrapper), and StFile (list-of-files event iterator).
#ifndef ROOT_StTree
#define ROOT_StTree
 


#include "TSystem.h"
#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TDataSet.h"
#include "St_ObjectSet.h"
#include "TDataSetIter.h"
#include "StFileI.h"



/**
 * \class StIO
 * \brief Static helpers for low-level STAR ROOT-file I/O.
 *
 * \details All methods are static; no instances of StIO are created.  They
 * wrap TFile operations with StUKey-based addressing so that events are
 * identified by run/event-number pairs rather than by sequential position.
 */
class StIO 
{

 public:
 /// Write \a obj to \a file under key \a ukey; \return bytes written or <0 on error.
 static Int_t    Write    (TFile *file, const StUKey &ukey, TObject  *obj);
 /// Read the object named \a name from \a file.
 static TObject *Read     (TFile *file, const char *name);
 /// Read the object identified by \a ukey from \a file.
 static TObject *Read     (TFile *file, const StUKey &ukey);
 /// Advance \a ukey and \a handle to the next key in \a file; \return 1 if successful.
 static Int_t   GetNextKey(TFile *file,       StUKey &ukey, ULong_t &handle);
 /// Read the next object from \a file, updating \a ukey and \a handle.
 static TObject *ReadNext (TFile *file,       StUKey &ukey, ULong_t &handle);
 /// \return An RFIO-prefixed path for \a name (for dCache/HPSS access).
 static TString  RFIOName (const char *name);
 /// Open a ROOT file; wraps TFile::Open with RFIO fallback.
 static TFile   *Open     (const char *name, Option_t *option="",const char *title="",Int_t compress=1);
 /// \return 1 if the file \a file exists and is accessible.
 static Int_t    IfExi(const char *file);
 static void    SetDebug(Int_t dblev=1){fgDebug=dblev;}
 static Int_t   fgDebug; ///< Global debug level for StIO operations.
};

class StTree;
/**
 * \class StBranch
 * \brief Single-file branch of a STAR event tree.
 *
 * \details An StBranch manages one ROOT file that stores a subset of the
 * event data (e.g. tracks, or hits).  Multiple StBranch objects are
 * collected by an StTree, which provides coordinated read/write across all
 * branches.
 *
 * \sa StTree, StIO
 */
class StBranch : public TDataSet {
//friend class StBranch;
friend class StTree;
public:
  StBranch(const char *name="", StTree* parent=0,Option_t *opt=0);
 ~StBranch();

  /// Set the I/O mode for this branch ("r", "w", "u", or "0" = inactive).
  virtual void SetIOMode(Option_t *iomode="0");
  virtual Option_t *GetIOMode();
  virtual void SetOption(Option_t *opt);
  virtual Option_t *GetOption() const {return fOption;};
  virtual Bool_t IsOption(Option_t *opt) const 
          {return fOption.Contains(opt,TString::kIgnoreCase);};
  virtual Int_t UpdateFile(const char *file);
  /// \return The path of the ROOT file associated with this branch.
  virtual const char *GetFile();
  /// Associate this branch with the file at \a file; open it with mode \a iomode.
  virtual Int_t SetFile(const char *file,const char *iomode=0,int insist=0);
  virtual Int_t SetTFile(TFile *tfile);
  virtual TFile        *GetTFile(){return fTFile;};
  virtual void SetName(const char *name){fUKey=name;TDataSet::SetName(name);};
  virtual void SetUKey(Int_t ukey){fUKey=ukey;};
  /// \return The current run/event key for this branch.
  virtual StUKey GetUKey()  const {return fUKey;};
  /// \return Total number of events written to this branch's file.
  virtual Int_t GetNEvents() const {return fNEvents;};
  /// Read (\a mode=0) or write (\a mode=1) the current event.
  virtual Int_t GetEvent(Int_t mode);
  /// Read the event identified by \a ukey.
  virtual Int_t ReadEvent (const StUKey &ukey);
  /// Advance to the next event and return its key in \a ukey.
  virtual Int_t NextEvent (      StUKey &ukey);
  virtual Int_t NextEvent ();
  /// Write the current event under key \a ukey.
  virtual Int_t WriteEvent(const StUKey &ukey);
  virtual void Clear(const char *opt=0);
  virtual void Close(const char *opt=0);
  virtual Int_t Open();
  virtual Int_t GetDebug(){return fDebug;};
  virtual void  SetDebug(int dbl=1){fDebug=dbl;};

protected:
  virtual void OpenTFile();
  void SetParAll(TDataSet *parNew,TDataSet *parOld, TList *savList);
  void SetParAll(TList *savList);
  Int_t fNEvents; 		//  Number of written events in file
  StUKey  fUKey;          	//! Current RunEvent number 
  char fIOMode;		        //! 1=ReadOnly; 2=WriteOnly; 1+2=Update;0=do nothing
  TString fFile;		//  File name
  TString fOption;		//  Option string
  TFile   *fTFile;		//! Opened TFile
  Int_t   fDebug;		//! debug level
  ULong_t fHandle;              //!
ClassDef(StBranch,1)
};  


/**
 * \class StTree
 * \brief Collection of StBranch nodes representing a multi-file STAR event tree.
 *
 * \details StTree coordinates read/write operations across all its child
 * StBranch objects so that a single NextEvent() call advances all branches
 * consistently.  Individual branches can be stored in separate ROOT files,
 * allowing selective loading of event data.
 *
 * \sa StBranch, StIO, StFile
 */
class StTree : public StBranch {
public:
  StTree(const char *name="");
 ~StTree();

  virtual void  SetIOMode (Option_t *iomode="0");			//Set for all branches
  virtual Int_t ReadEvent (const StUKey &ukey);
  virtual Int_t NextEvent (      StUKey &ukey);
  virtual Int_t NextEvent ();
  /// Advance to the next key without reading; \return 1 on success, 0 at end-of-file.
          Int_t NextKey();
  /// Skip \a nskip events forward.
          Int_t Skip(int nskip);
  virtual Int_t WriteEvent(const StUKey &ukey);
  virtual void  Close(const char *opt=0);
  virtual Int_t Open();
  virtual void Clear(Option_t *opt="");
  virtual Int_t SetFile(const char *file,const char *iomode=0,int insist=0);
  /// Set the base name used to construct per-branch file names as
  /// \<basename\>.\<branchname\>.root.
  virtual void SetBaseName(const char* basename,const char* dirname=0);
  virtual const char *GetBaseName() 
          {return (fBaseName.IsNull()) ? 0:(const char*)fBaseName;};
  /// Locate and return the StTree named \a treeName stored in \a file.
  static StTree *GetTree(TFile *file, const char *treeName);
  virtual Int_t UpdateFile(const char *file);

protected:
  TString fBaseName;		//base name to construct branch file name
				// as <basename>.<branchname>.root
ClassDef(StTree,1)
};  

/**
 * \class StIOEvent
 * \brief TBrowser-compatible wrapper used by StIO to display a full event tree.
 * \note Auxiliary class for StIO; not intended for direct use.
 */
class StIOEvent : public TObject 
{
public:
  StIOEvent();
 ~StIOEvent(){};
TObject *fObj;	// Pointer to full tree
 virtual void  Browse(TBrowser *b);
 virtual Bool_t IsFolder() const { return kTRUE; }

ClassDef(StIOEvent,1)
};

/**
 * \class StFile
 * \brief Event-file iterator that manages a list of ROOT files for sequential reading.
 *
 * \details StFile implements the StFileI interface.  Files are added with
 * AddFile() / AddWild(); the framework then calls GetNextEvent() repeatedly
 * to iterate over all run/event keys across all files in the list.
 *
 * \sa StTree, StBranch, StFileI
 */
class StFile : public StFileI
{
public:
  StFile(const char** fileList=0);
  virtual ~StFile();

  virtual void  ls(Option_t *opt="") ;
  virtual void  ls(Option_t *opt="") const{ ((StFile*)this)->ls(opt);}
  /// Add a single file to the list.  Pass \a opt="remove" or "exclude" to skip it.
  virtual Int_t AddFile(const char *file,const char *opt=0); //opt="remove" or "exclude" remove file from list
  virtual Int_t AddFile(const char **fileList);
  virtual Int_t AddWild(const char *file,const char *opt=0);
  virtual Int_t AddEvent(UInt_t r,UInt_t e=0);
  virtual Int_t GetNBundles();
  virtual Int_t GetNFiles();
  virtual Int_t GetBundleSize(){return 1;};
  virtual StUKey GetNextEvent();
  virtual Int_t GetNextEvent(UInt_t *NextEventNumber){return StFileI::GetNextEvent(NextEventNumber);}

  virtual const char *GetFileName(Int_t idx=-1);
  virtual const char *GetCompName(Int_t idx=0);
  virtual const char *GetFormat(Int_t idx=0);
  virtual Int_t GetNextBundle();
  virtual void Rewind(){fIter = -1; delete fKeyIter; fKeyIter =0;}
  
protected:
  void  lsFull(Option_t *opt="") ;
  void SetInfo(TDataSet *ds);
  const char *GetAttr(TDataSet *ds,const char *att);
  TDataSet *GetFileDS(int idx);
  TDataSet *fDS;
  Int_t    fIter; 
  TDataSetIter *fKeyIter; 
  ClassDef(StFile,1)
};
#endif
