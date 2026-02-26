// $Id: StEmcRawMaker.h,v 1.10 2014/08/06 11:43:07 jeromel Exp $

/**
 * \file  StEmcRawMaker.h
 * \brief Declaration of StEmcRawMaker, the maker that decodes raw
 *        electromagnetic calorimeter DAQ data into StEmcCollection in StEvent.
 */

/**
 * \class StEmcRawMaker
 * \brief Maker that decodes raw electromagnetic calorimeter data into StEmcCollection.
 *
 * \author Alexandre A. P. Suaide
 *
 * \details This maker reads raw BEMC and EEMC sub-events from the DAQ stream,
 * unpacks ADC values, performs basic corruption checks, and fills
 * StEmcCollection (accessible via StEvent::emcCollection()) with
 * StEmcRawData and StEmcRawHit objects.
 *
 * Tasks performed each event:
 * -# Retrieve raw EMC data from the DAQ reader.
 * -# Fill BEMC and EEMC raw-data containers with DAQ payload.
 * -# Check BEMC sub-event for corruption (flagged but not removed).
 * -# Check EEMC sub-event for corruption (removed if corrupt).
 * -# Convert ADC counts to StEmcRawHit objects using controlADCtoE calibration tables.
 * -# Fill QA monitoring histograms for both BEMC and EEMC.
 *
 * The \c controlADCtoE tables can be tuned via the BFC chain option \c GoptEMC
 * followed by 6 hexadecimal digits, one per sub-detector component
 * (BTOW, ETOW, BSMD, ESMD, BPSD, EPSD).  Bit 0 controls \c CheckStatus;
 * bit 1 controls \c CutOffType.  Example: \c "GoptEMC000020" sets
 * CutOffType=1 for BPSD only.
 *
 * \note This maker is intended for production running or when reading raw DAQ
 *       files.  It fills StEmcCollection inside StEvent.
 *
 * \sa StBemcRaw, StEemcRaw, StEmcCollection
 */

#ifndef STAR_StEmcRawMaker
#define STAR_StEmcRawMaker

#include "StMaker.h"
#include "StRTSBaseMaker.h"
#include "TH1.h"
#include "TH2.h"
#include "tables/St_controlADCtoE_Table.h"
#include "StBemcRaw.h"
#include "defines.h"

class StEmcCollection;
class StEmcDecoder;
class StEmcGeom;
class StEmcRawData;
class StEvent;

class StEEmcDb;
class StEemcRaw;

class StEmcRawMaker : public StRTSBaseMaker
{
protected:
    StEvent*                 mEvent;
    StBemcRaw*               mBemcRaw;
    StEemcRaw*               mEemcRaw;
    StEEmcDb*                eeStDb; ///< to assess EEMC DB

    void                     fillHistograms();///<Fill QA histograms
    Bool_t                   prepareEnvironment();///< Prepare the StEvent environment to fill the EMC data
    Bool_t                   makeBemc(); ///< Make the Barrel-EMC detector
    Bool_t                   makeEemc(); ///< Make the Endcap-EMC detector

public:
    StEmcRawMaker(const char *name="EmcRaw"); ///< Constructor. \param name Maker name passed to StMaker.
    virtual                   ~StEmcRawMaker(); ///< Destructor; releases owned resources.
    virtual Int_t             Init(); ///< Initialises QA histograms and sub-maker objects. \return kStOK on success.
    virtual Int_t             InitRun(Int_t runumber); ///< Loads run-dependent calibration tables. \param runumber Run number. \return kStOK on success.
    virtual Int_t             Make(); ///< Decodes raw BEMC/EEMC DAQ data and fills StEmcCollection in StEvent. \return kStOK on success, kStWarn/kStErr on data issues.
    virtual Int_t             Finish(); ///< Writes QA histograms and finalises. \return kStOK.

    StRtsTable* Dta()
      {
	return DaqDta();
      }

    StRtsTable* GetDaqElement(const char *elementPath)
      {
	return GetNextDaqElement(elementPath);
      }


    StBemcRaw*                getBemcRaw()
    {
        return mBemcRaw;
    } ///< Return the StBemcRaw pointer
    void                      setPrint(Bool_t a); ///< Obsolete function; users can control messages with logger config file.

    virtual const char *      GetCVS() const
    {
        static const char cvs[]="Tag $Name:  $ $Id: StEmcRawMaker.h,v 1.10 2014/08/06 11:43:07 jeromel Exp $ built " __DATE__ " " __TIME__ ;
        return cvs;
    }

    ClassDef(StEmcRawMaker, 1)
};

#endif

// $Log: StEmcRawMaker.h,v $
// Revision 1.10  2014/08/06 11:43:07  jeromel
// Suffix on literals need to be space (later gcc compiler makes it an error) - first wave of fixes
//
// Revision 1.9  2009/02/04 21:05:42  kocolosk
// Refactor StEEmcDb(Maker), new location for StEmcDecoder. Fixes RT #1388.
//
// Revision 1.8  2009/01/27 19:58:36  mattheww
// Updates to StEmcRawMaker to be compatible with 2009 DAQ Format
//
// Revision 1.7  2008/03/27 19:54:16  genevb
// Utilize new BFC option for GoptEMC for controlADCtoE table
//
// Revision 1.6  2007/01/22 19:13:37  kocolosk
// use STAR logger for all output
//
// Revision 1.5  2006/01/16 11:12:00  suaide
// tower map bug fixed and astyle run
//
// Revision 1.4  2004/10/21 00:01:50  suaide
// small changes in histogramming and messages for BEMC
// Complete version for EEMC done by Jan Balewski
//
// Revision 1.3  2004/10/19 23:48:49  suaide
// Initial implementation of the endcap detector done by Jan Balewski
//
// Revision 1.2  2004/10/19 17:53:00  suaide
// code clean up
//
// Revision 1.1  2004/10/18 18:20:07  suaide
// New Maker. Will replace StEmcADCtoEMaker in production.
// It reads only DAQ structures. Output is StEvent.
//
//
