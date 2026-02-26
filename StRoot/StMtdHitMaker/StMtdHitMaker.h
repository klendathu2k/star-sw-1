#ifndef STAR_StMtdHitMaker_H
#define STAR_StMtdHitMaker_H

/**
 * \file  StMtdHitMaker.h
 * \brief Declaration of StMtdHitMaker, the maker that converts raw MTD DAQ
 *        data into StMtdCollection in StEvent.
 */

/***************************************************************************
 *
 * $Id: StMtdHitMaker.h,v 1.13 2015/01/22 22:10:58 marr Exp $ 
 * StMtdHitMaker - class to fill StEvent from DAQ reader
 *--------------------------------------------------------------------------
 *
 ***************************************************************************/
#include "StRTSBaseMaker.h"
#include "TH3D.h"
#include "StMtdUtil/StMtdConstants.h"
struct mtd_t;

class StEvent;
class StMtdCollection;
class StMtdRawHit;
class StMtdHit;
class StBTofINLCorr;

// 
#if !defined(ST_NO_TEMPLATE_DEF_ARGS) || defined(__CINT__)
typedef vector<Int_t>  IntVec;
typedef vector<UInt_t>  UIntVec;
#else
typedef vector<Int_t, allocator<Int_t>>  IntVec;
typedef vector<UInt_t, allocator<UInt_t>>  UIntVec;
#endif

struct MtdRawHit {
  UInt_t   tdc;               /// tdc time (in bin) per hit.
  UInt_t   dataword;          /// data word before unpack
  UChar_t  fiberid;           /// 0 1 2,3
  UChar_t  backlegid;            /// 1,2,...,117
  UChar_t  globaltdcchan;     /// 0,1,..   
};

struct MTDSingleHit{
	Int_t 			fiberId;
	Int_t           backleg; // 1-30
	Int_t           tray;  // 1-5
	Int_t           channel; // 0-23
	IntVec 			leadingEdgeTime;     
	IntVec			trailingEdgeTime;     
};

struct MTDOneSideHit{
        Int_t           fiberId;
	Int_t           backleg; // 1-30
	Int_t           tray;  // 1-5
	Int_t           channel; // 0-23
	Double_t        leadingEdgeTime;     
	Double_t		trailingEdgeTime;     
};

#ifndef ST_NO_TEMPLATE_DEF_ARGS
  typedef vector<MTDSingleHit> mtdSingleHitVector;
#else
  typedef vector<MTDSingleHit,allocator<MTDSingleHit>> mtdSingleHitVector;
#endif
  typedef vector<MTDSingleHit>::iterator mtdSingleHitVectorIter;

#define VHRBIN2PS 24.4140625  // Very High resolution mode, pico-second per bin
                              // 1000*25/1024 (ps/chn)
#define HRBIN2PS 97.65625     // High resolution mode, pico-second per bin
                              // 97.65625= 1000*100/1024  (ps/chn)

/**
 * \class StMtdHitMaker
 * \brief Maker that converts raw MTD DAQ data into StMtdCollection in StEvent.
 *
 * \details StMtdHitMaker reads raw Muon Telescope Detector (MTD) data from
 * the DAQ stream via the RTS reader.  It unpacks TDC leading- and
 * trailing-edge words per THUB fiber, applies INL corrections, matches
 * single-sided hits into double-sided StMtdHit objects using time-of-flight
 * trigger-window cuts, and stores the results in StMtdCollection in StEvent.
 *
 * \par Inputs
 * - Raw MTD DAQ sub-event from the RTS reader.
 * - INL correction tables (StBTofINLCorr) and tray/TDIG geometry maps from
 *   the STAR database.
 *
 * \par Output
 * - StMtdCollection in StEvent containing StMtdRawHit and StMtdHit objects.
 *
 * \note TDC time-bin-to-picosecond conversion constants: \n
 *   VHRBIN2PS = 24.4140625 ps/bin (very-high-resolution mode) \n
 *   HRBIN2PS  = 97.65625   ps/bin (high-resolution mode) \n
 *   Run-specific corrections (backleg swap in Run 13, strip reversal in
 *   Run 14) are applied when the corresponding setters are called.
 *
 * \sa StMtdHit, StMtdCollection, StBTofINLCorr
 */
class StMtdHitMaker:public StRTSBaseMaker {
 private:
  StEvent                 *mStEvent;
  mtd_t                   *fMtd;
  Int_t                   mUseMuDst;
  Bool_t                  mCosmicFlag;
  TString                 mCosmicTrigTimeWinFile;
  Bool_t                  mTriggerWndSelection;
  Int_t                   mSwapBacklegInRun13;                         // 0 - do not swap; 1 - for first part of Run13
                                                                       // 2 - for second part of Run13
  Bool_t                  mReverseStripInRun14;                        // Reverse strips in backleg 7, module 5 in Run14
  Int_t                   mYear;                                       //! RHIC run year
  Int_t                   mNValidTrays;                                //! number of valid MTD trays

  StMtdCollection*        mMtdCollection;                              //! pointer to StMtdCollection
  StBTofINLCorr*          mINLCorr;                                    //! pointer to INL correction class

  static const Int_t nTHUB     = 2;
  static const Int_t mNFIBER   = 1;
  UInt_t           	  mTriggerTimeStamp[nTHUB];                    //! Trigger Time in 4 fibers
  Int_t                   mTray2TdigMap[gMtdNBacklegs][gMtdNModules];  //! map TDIG-Id to MTD tray
  Int_t                   mTrayId[gMtdNBacklegs][gMtdNModules];        //! map MTD trayIDs
  Int_t                   mTdigId[gMtdNModulesAll];                    //! map TDIG Ids on MTD TrayIds
  Int_t 		  mtdStrip[gMtdNChannels];		       //! strip channel to glabal tdc chan
  Double_t                mTriggerTimeWindow[gMtdNModulesAll][2];      //! trigger time window cut

  vector<MtdRawHit> MtdLeadingHits;
  vector<MtdRawHit> MtdTrailingHits;
  mtdSingleHitVector mSingleHitVec[gMtdNModulesAll];

  TH3D *hxhyhz;


 protected:
  StMtdCollection *GetMtdCollection();
  Int_t UnpackMtdRawData();
  StRtsTable *GetNextRaw();
  StRtsTable *GetNextRaw(Int_t sec);
  Int_t tdcChan2globalStrip11(Int_t, Int_t, Int_t, Int_t);
  Int_t tdcChan2globalStrip(Int_t, Int_t, Int_t, Int_t);
  Int_t getTdigBoardId(Int_t backlegid, Int_t tray, Int_t chn);
  Int_t getLocalTdcChan(Int_t backlegid, Int_t tray, Int_t chn);
  Int_t getTdigLocalChan(Int_t backlegid, Int_t itdigid);
  void fillMtdHeader();
  void fillMtdRawHitCollection();
  void fillMtdHitCollection();
  void fillMtdSingleHits();
  void fillStEvent();     //! ship collection to StEvent and check
  IntVec  GetValidTrays();
  IntVec  GetValidChannel(int backleg, int tray, int &fiber);
  UIntVec GetLeadingTdc(int backleg, int tray, int channel);
  UIntVec GetTrailingTdc(int backleg, int tray, int channel);

 public:

  /// Default constructor. \param name Maker name passed to StRTSBaseMaker.
  StMtdHitMaker(const char *name="mtd_raw");     
  ~StMtdHitMaker() ;
  void setUseMuDst(Int_t val);                   ///< Enable reading from MuDst instead of StEvent. \param val 1 = use MuDst.
  void setTriggerWndSelection(Bool_t val);        ///< Enable/disable trigger time-window selection. \param val kTRUE to enable.
  void setSwapBacklegInRun13(Int_t swap);         ///< Apply backleg swap correction for Run 13. \param swap 0 = none, 1 = first half, 2 = second half.
  void setReverseStripInRun14(Bool_t re);         ///< Reverse strips in backleg 7 module 5 for Run 14. \param re kTRUE to apply.
  void setCosmicEvent(Bool_t val);                ///< Flag run as a cosmic-ray event (relaxes hit-matching). \param val kTRUE for cosmics.
  void setCosmicTrigTimeWinFile(const char *file);///< Set path to cosmic trigger time-window file. \param file File path.

  void   Clear(Option_t* option=""); ///< Clears per-event transient data. \param option Passed to StMaker::Clear().
  Int_t  Init();                     ///< Initialises channel maps and INL corrections. \return kStOK on success.
  Int_t  InitRun(Int_t);             ///< Loads run-dependent calibration tables and geometry. \param runnumber Run number. \return kStOK on success.
  Int_t  FinishRun(Int_t);           ///< Run-end clean-up. \return kStOK.
  Int_t  Finish();                   ///< Job-end finalisation. \return kStOK.
  Int_t  Make();                     ///< Unpacks raw MTD DAQ data and fills StMtdCollection in StEvent. \return kStOK on success.

  /// obtain the whole list of leading edge hits
  vector<MtdRawHit> getLeadingHits();
  /// obtain the whole list of trailing edge hits
  vector<MtdRawHit> getTrailingHits();
  
  /// cvs
  virtual const char *GetCVS() const {
    static const char cvs[]="Tag $Name:  $Id: built " __DATE__ " " __TIME__ ; return cvs;
  }
  
  ClassDef(StMtdHitMaker, 4)    ///StMtdHitMaker - class to fille the StEvent from DAQ reader
};

inline vector<MtdRawHit> StMtdHitMaker::getLeadingHits()      { return MtdLeadingHits; }
inline vector<MtdRawHit> StMtdHitMaker::getTrailingHits()     { return MtdTrailingHits;}
inline void StMtdHitMaker::setUseMuDst(Int_t val)             { mUseMuDst = val;}
inline void StMtdHitMaker::setTriggerWndSelection(Bool_t val) { mTriggerWndSelection = val;}
inline void StMtdHitMaker::setSwapBacklegInRun13(Int_t swap)  { mSwapBacklegInRun13 = swap; }
inline void StMtdHitMaker::setReverseStripInRun14(Bool_t re)  { mReverseStripInRun14 = re; }
inline void StMtdHitMaker::setCosmicEvent(Bool_t val)         { mCosmicFlag = val; }
inline void StMtdHitMaker::setCosmicTrigTimeWinFile(const char *file)
{ mCosmicTrigTimeWinFile = file; }

#endif
