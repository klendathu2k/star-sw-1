/***************************************************************************
 *
 * $Id: StEventMaker.h,v 2.16 2014/08/06 11:43:08 jeromel Exp $
 *
 * Author: Original version by T. Wenaus, BNL
 *         Revised version for new StEvent by T. Ullrich, Yale
 ***************************************************************************
 *
 * Description: Setup of StEvent
 *
 ***************************************************************************
 *
 * $Log: StEventMaker.h,v $
 * Revision 2.16  2014/08/06 11:43:08  jeromel
 * Suffix on literals need to be space (later gcc compiler makes it an error) - first wave of fixes
 *
 * Revision 2.15  2009/11/23 16:37:08  fisyak
 * Clean up, fix problem with bunch crossing information in StEventInfo and StHddr
 *
 * Revision 2.13  2009/11/19 16:54:09  fisyak
 * Clean up
 *
 * Revision 2.12  2009/11/10 20:45:08  fisyak
 * pams Cleanup
 *
 * Revision 2.11  2002/04/18 23:29:35  jeromel
 * Implementation of the SVT 2 tables scheme ...
 *
 * Revision 2.10  2002/02/25 19:34:14  ullrich
 * Fill parts of StRunInfo from StDetectorDbBeamInfo.
 *
 * Revision 2.9  2001/09/28 22:22:05  ullrich
 * Load helix geometry at last point of each track.
 *
 * Revision 2.8  2001/09/18 00:16:07  ullrich
 * Fill and add StRunInfo.
 *
 * Revision 2.7  2001/09/12 23:49:22  ullrich
 * Removed code to build StRun and StRunSummary.
 *
 * Revision 2.6  2001/05/17 22:46:38  ullrich
 * Removed loading of event summary params.
 *
 * Revision 2.5  2001/02/22 04:59:16  ullrich
 * Added protected method getStEventInstance().
 *
 * Revision 2.4  2000/08/17 00:38:52  ullrich
 * Allow loading of tpt tracks.
 *
 * Revision 2.3  2000/05/24 15:48:20  ullrich
 * Instance of StEvent now also created if no DST dataset
 * is available.
 *
 * Revision 2.2  2000/01/05 16:07:47  ullrich
 * Added loading of SSD hits and handling of runco branch.
 *
 * Revision 2.1  1999/11/05 18:35:57  ullrich
 * Added methods and flags for debugging and monitoring.
 *
 * Revision 2.0  1999/11/04 19:03:00  ullrich
 * Revised to build new StEvent version
 *
 **************************************************************************/
/**
 * \file  StEventMaker.h
 * \brief Declaration of StEventMaker, the maker that creates and registers
 *        the StEvent object on the StChain whiteboard.
 */
#ifndef STAR_StEventMaker
#define STAR_StEventMaker

#include "StMaker.h"
#include "StEventMaker/StEventManager.hh"
class StEvent;
class StTrack;
class StRunInfo;

/**
 * \class StEventMaker
 * \brief Maker that creates and registers the StEvent object for each event.
 *
 * \details StEventMaker is the first maker in the reconstruction chain
 * responsible for creating a new StEvent instance at the start of every
 * event and posting it to the StChain whiteboard under the key "StEvent".
 * Downstream makers retrieve this object via \c GetInputDS("StEvent") or
 * the typed helper \c GetStEvent().
 *
 * In legacy mode, StEventMaker can also load pre-formed hit collections from
 * a DST dataset (TPC, FTPC, SVT, SSD) and optionally populate tracking
 * results from the Tpt or Est finders.  These code paths are controlled by
 * the public boolean flags (doLoadTpcHits, doLoadFtpcHits, etc.).
 *
 * \par Inputs
 * - Optional DST dataset from GetDataSet("dst").
 * - StRunInfo filled from detector-database beam-info tables.
 *
 * \par Output
 * - A new StEvent instance registered on the StChain whiteboard under
 *   the key "StEvent".
 *
 * \sa StEvent, StRunInfo, StChain
 */
class StEventMaker : public StMaker {
public: 
    StEventMaker(const char *name="StEventMaker", const char *title=""); ///< Constructor. \param name Maker name. \param title Optional title.
    virtual ~StEventMaker();
    
    virtual Int_t           Init();  ///< Initialises internal state and event manager. \return kStOK on success.
    virtual Int_t           Make();  ///< Creates a new StEvent, loads optional legacy hit collections, and posts it to the whiteboard. \return kStOK on success.
    virtual void            setEventManager(StEventManager* mgr); ///< Override the default event manager. \param mgr Pointer to the replacement manager.
    virtual StEventManager* eventManager(); ///< \return Pointer to the current event manager.
    virtual StEvent*        event();        ///< \return Pointer to the StEvent for the current event, or nullptr if Make() has not been called.
    virtual void            Clear(const char* opt=""); ///< Releases the current StEvent pointer without deleting the object. \param opt Passed to StMaker::Clear().
       
    virtual const char *GetCVS() const
    {
	static const char cvs[]="$Id: StEventMaker.h,v 2.16 2014/08/06 11:43:08 jeromel Exp $ built " __DATE__ " " __TIME__ ;
	return cvs;
    }

public:    
    Bool_t  doLoadTpcHits;         //!
    Bool_t  doLoadFtpcHits;        //!
    Bool_t  doLoadSvtHits;         //!
    Bool_t  doLoadSsdHits;         //!
    Bool_t  doLoadTptTracks;       //!
    Bool_t  doLoadEstTracks;       //!
    
    Bool_t  doPrintEventInfo;      //! lots of screen output
    Bool_t  doPrintMemoryInfo;     //! 
    Bool_t  doPrintCpuInfo;        //!
    
protected:
    Int_t     makeEvent();
    StEvent*  getStEventInstance();
    void   printEventInfo();
    void   printTrackInfo(StTrack*);
    
private:
    StEventManager*       mEventManager;		//!
    StEvent*              mCurrentEvent;                //!
    Bool_t                mCreateEmptyInstance;         //!
    ClassDef(StEventMaker, 0)
};
#endif
