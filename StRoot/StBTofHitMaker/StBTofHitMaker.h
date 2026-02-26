#ifndef STAR_StBTofHitMaker_H
#define STAR_StBTofHitMaker_H

/**
 * \file  StBTofHitMaker.h
 * \brief Declaration of StBTofHitMaker, the maker that converts raw BTOF DAQ
 *        data into StBTofCollection in StEvent.
 */

/***************************************************************************
 *
 * $Id: StBTofHitMaker.h,v 1.12 2017/10/20 17:50:32 smirnovd Exp $
 * StBTofHitMaker - class to fille the StEvent from DAQ reader
 *--------------------------------------------------------------------------
 *
 ***************************************************************************/
#include "StRTSBaseMaker.h"

#include <vector>

#define VHRBIN2PS 24.4140625  // Very High resolution mode, pico-second per bin
                              // 1000*25/1024 (ps/chn)
#define HRBIN2PS 97.65625     // High resolution mode, pico-second per bin
                              // 97.65625= 1000*100/1024  (ps/chn)

struct tof_t;

class StEvent;
class StBTofCollection;
class StBTofRawHit;
class StBTofRawHitCollection;
class StBTofHit;
class StBTofHitCollection;
#include "StBTofUtil/StBTofSortRawHit.h"
class StBTofINLCorr;
class StBTofDaqMap;

struct TofRawHit {
  unsigned int   tdc;               /// tdc time (in bin) per hit.
  unsigned int   dataword;          /// data word before unpack
  unsigned char  fiberid;           /// 0 1 2,3
  unsigned char  trayID;            /// 1,2,......,120,for tray, 121, 122 for upvpd
  unsigned char  globaltdcchan;     /// 0,1,......,191   
};

typedef std::vector<Int_t>  IntVec;
typedef std::vector<UInt_t>  UIntVec;


/**
 * \class StBTofHitMaker
 * \brief Maker that converts raw BTOF DAQ data into StBTofCollection in StEvent.
 *
 * \details StBTofHitMaker reads raw time-of-flight data from the Barrel TOF
 * (BTOF) sub-event via the RTS DAQ reader.  It unpacks TDC leading- and
 * trailing-edge words, applies INL corrections, maps hardware channels to
 * physical tray/module/channel addresses, and constructs StBTofRawHit and
 * StBTofHit objects.  The completed StBTofCollection is posted to StEvent.
 *
 * \par Inputs
 * - Raw BTOF DAQ sub-event from the RTS reader.
 * - Daq map (StBTofDaqMap) and INL correction tables (StBTofINLCorr) from
 *   the STAR database.
 *
 * \par Output
 * - StBTofCollection in StEvent containing StBTofRawHit and StBTofHit
 *   objects, plus upVPD trigger-time information.
 *
 * \note TDC time-bin-to-picosecond conversion constants: \n
 *   VHRBIN2PS = 24.4140625 ps/bin (very-high-resolution mode) \n
 *   HRBIN2PS  = 97.65625   ps/bin (high-resolution mode)
 *
 * \sa StBTofHit, StBTofCollection, StBTofDaqMap, StBTofINLCorr
 */
class StBTofHitMaker:public StRTSBaseMaker
{
   private: 
      StEvent *mStEvent;
      tof_t   *fTof;
      Int_t mYear;  //! year time stamp of raw data

      Int_t UnpackTofRawData();
      void fillBTofHeader();
      void fillBTofRawHitCollection();
      void fillBTofHitCollection();
      void fillStEvent();     //! ship collection to StEvent and check
      /// TOF Raw hits info. struct
      ///----------------------------------------------------
      std::vector<TofRawHit> TofLeadingHits;
      std::vector<TofRawHit> TofTrailingHits;

     Int_t                    mNValidTrays;          //! number of valid TOF trays
     unsigned int             mTriggerTimeStamp[4];  //! Trigger Time in 4 fibers
     StBTofCollection*        mBTofCollection;       //! pointer to StBTofCollection
     StBTofRawHitCollection*  mRawHitCollection;     //! pointer to StBTofRawHitCollection
     StBTofHitCollection*     mHitCollection;        //! pointer to StBTofHitCollection
     StBTofDaqMap*            mBTofDaqMap;           //! pointer to the TOF daq map
     StBTofINLCorr*           mBTofINLCorr;          //! INL corretion;
     StBTofSortRawHit*        mBTofSortRawHit;       //! to sort the TOF hits
     enum {
            mNVPD = 19
          , mWestVpdTrayId = 121
          , mEastVpdTrayId = 122
     };

   protected:
      StRtsTable *GetNextRaw();
      StRtsTable *GetNextRaw(int sec);
      
      StBTofCollection *GetBTofCollection();

   public:

     /// Default constructor. \param name Maker name passed to StRTSBaseMaker.
     StBTofHitMaker(const char *name="tof_raw");
     
    ~StBTofHitMaker() ;

     void   Clear(Option_t* option=""); ///< Clears per-event transient data. \param option Passed to StMaker::Clear().
     Int_t  Init();                     ///< Initialises daq-map and INL-correction objects. \return kStOK on success.
     Int_t  InitRun(Int_t);             ///< Loads run-dependent calibration tables. \param runnumber Run number. \return kStOK on success.
     Int_t  FinishRun(Int_t);           ///< Run-end clean-up. \return kStOK.
     Int_t  Finish();                   ///< Job-end finalisation. \return kStOK.
     Int_t  Make();                     ///< Unpacks raw BTOF DAQ data and fills StBTofCollection in StEvent. \return kStOK on success.

     /// obtain the whole list of leading edge hits
     std::vector<TofRawHit> getLeadingHits();
     /// obtain the whole list of trailing edge hits
     std::vector<TofRawHit> getTrailingHits();
     
  /// cvs
  virtual const char *GetCVS() const
    {
      static const char cvs[]="Tag $Name:  $Id: built " __DATE__ " " __TIME__ ; return cvs;
    }
  
  ClassDef(StBTofHitMaker, 1)    ///StBTofHitMaker - class to fille the StEvent from DAQ reader
};

inline std::vector<TofRawHit> StBTofHitMaker::getLeadingHits() { return TofLeadingHits; }
inline std::vector<TofRawHit> StBTofHitMaker::getTrailingHits() { return TofTrailingHits; }

#endif
