#ifndef STAR_StIOInterFace
#define STAR_StIOInterFace

/*!
 * \file StIOInterFace.h
 * \brief Declaration of StIOInterFace, the abstract base class for STAR I/O makers.
 */

/*!
 * \class StIOInterFace
 * \brief Abstract base class for STAR input/output makers.
 *
 * \details
 * StIOInterFace extends StRTSBaseMaker to provide a uniform interface for makers
 * that read or write event data (e.g. MuDST files, XDF files, ROOT trees).
 * Concrete subclasses override MakeRead() or MakeWrite() to implement the actual
 * I/O; the framework calls Make() on every event, which delegates to the
 * appropriate method depending on the I/O mode.
 *
 * The I/O mode is set at construction time or via SetIOMode() and can be:
 *  - \c 'r' — read mode (default)
 *  - \c 'w' — write mode
 *  - \c 'u' — update (read+write) mode
 *
 * \sa StRTSBaseMaker, StMaker, StChain
 */

#ifndef STAR_StRTSBaseMaker_H
#include "StRTSBaseMaker.h"
#endif
#include "StTree.h"
class StUKey;

class StIOInterFace : public  StRTSBaseMaker {
private:
TString fFileName;
public:
   /// Construct an I/O maker with the given \a name and \a iomode ('r', 'w', or 'u').
   StIOInterFace(const char *name="",const char *iomode="r");
   virtual        ~StIOInterFace(){};

   /// Read the next event.  Delegates to Make() by default; override in subclasses.
   virtual  Int_t MakeRead() {return Make();};

   /// Read the event identified by \a RunEvent.  Delegates to MakeRead() by default.
   virtual  Int_t MakeRead(const StUKey &/* RunEvent */) {return MakeRead();};

   /// Pure I/O entry point called once per event; must be overridden by concrete makers.
   virtual  Int_t Make() {assert(0);return 1999;};
   virtual  Int_t Skip(int nskip);

   /// Write the current event.  Must be overridden by write-capable concrete makers.
   virtual  Int_t MakeWrite(){assert(0);return 1999;};

   /// Open the file or stream named \a filename.  Must be overridden by concrete makers.
   virtual  Int_t Open(const char *filename=0){assert(0&&filename);return 1999;};

   /// Close the currently open file or stream.  Must be overridden by concrete makers.
   virtual  void  Close(Option_t *opt=0){assert(0&&opt);};
   virtual  Int_t  Finish();
   TString         fIOMode;	//!r=read,w=write,u=update
   TString         fTreeName;	//!Tree name
   TString         fFile;	//!Main file name name
   Int_t           fNIO;	//!number of transactions

   virtual void  SetIOMode(Option_t *iomode="w") {fIOMode=tolower(iomode[0]);};
   virtual void  SetTreeName(const Char_t *treeName="bfcTree"){fTreeName=treeName;};
   virtual const Char_t  *GetTreeName() const {return (const Char_t*)fTreeName;};

   /// Set the primary input/output filename.
   virtual void  SetFileName(const char *fileName){fFile = fileName;};
   virtual void  SetFile(const char *fileName)    {fFile = fileName;};
   /// Return the primary input/output filename.
   virtual const char *GetFile()     const   {return (const char*)fFile;};
   virtual const char *GetFileName() const   {return GetFile();};

   virtual void SetBranch (const Char_t *brName,const Char_t *file=0,const Char_t *iomode="w",Option_t *opt=0);
   virtual void IntoBranch(const Char_t *brName,const Char_t *logNames);

   ClassDef(StIOInterFace, 1)   //StAR chain virtual base class for Makers
};

#endif
