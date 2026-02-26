/*!
 * \class StBTofHeader
 * \author Xin Dong, Nov 2008
 */
/***************************************************************************
 *
 * $Id: StBTofHeader.h,v 2.5 2021/05/28 19:00:21 ullrich Exp $
 *
 * Author: Xin Dong, Nov 2008
 ***************************************************************************
 *
 * Description:
 *   Barrel TOF header data, contains the TOF data header, vpd summary and
 * other event-wise information.
 *
 ***************************************************************************
 *
 * $Log: StBTofHeader.h,v $
 * Revision 2.5  2021/05/28 19:00:21  ullrich
 * Added 3 member plus access fct: mTCanFirst, mTCanLast, mNTzeroCan (Frank)
 *
 * Revision 2.4  2010/05/17 17:47:16  ullrich
 * Increase version number to 2.
 *
 * Revision 2.3  2010/05/12 15:12:03  ullrich
 * Added member mNTzero and access methods.
 *
 * Revision 2.2  2009/01/15 00:45:27  ullrich
 * mTriggerTime becomes array, setVpdVz() gets default argument.
 *
 * Revision 2.1  2008/12/22 20:30:57  ullrich
 * Initial Revision.
 *
 *
 **************************************************************************/
#ifndef StBTofHeader_hh
#define StBTofHeader_hh

/// @file StBTofHeader.h
/// @brief Event-wise header data for the STAR Barrel Time of Flight (BTOF) detector, including VPD summary.

#include <Stiostream.h>
#include "StObject.h"
#include "StEnumerations.h"

/// @brief Event-wise header data for the STAR Barrel TOF (BTOF) detector, including VPD summary.
class StBTofHeader : public StObject {
public:
    enum {MAXFIBER=4, MAXVPD=19, MAXVPDVZ=20};

    /// @brief Default constructor.
    StBTofHeader();
    /// @brief Destructor.
    ~StBTofHeader();

    /// @brief Returns the fiber header word for the given fiber ID.
    short          fiberHeader(int fiberId) const;
    /// @brief Returns the fiber trigger word for the given fiber ID.
    unsigned int   fiberTriggerWord(int fiberId) const;
    /// @brief Returns the VPD hit-pattern bitmask for east or west.
    unsigned int   vpdHitPattern(StBeamDirection eastwest) const;
    /// @brief Returns the number of VPD hits for east or west.
    unsigned short numberOfVpdHits(StBeamDirection eastwest) const;
    /// @brief Returns true if the specified VPD tube has a hit.
    bool           isVpdHit(StBeamDirection eastwest, int tubeId) const;
    /// @brief Returns the VPD vertex z estimate at the given rank [cm].
    float          vpdVz(int rank=0) const;
    /// @brief Returns the event start time from the VPD [ns].
    double         tStart() const;
    /// @brief Returns the uncertainty on the event start time [ns].
    double         tStartError() const;
    /// @brief Returns the VPD east–west time difference [ns].
    double         tDiff() const;
    /// @brief Returns the measured VPD tube time for east or west and the given tube ID [ns].
    double         vpdTime(StBeamDirection eastwest, int tubeId) const;
    /// @brief Returns the trigger time TDC count for the given fiber ID.
    unsigned int   triggerTime(int fiberId) const;
    /// @brief Returns the number of BTOF hits used for t0 determination.
    int            nTzero() const;

    /// @brief Returns the number of BTOF hits in the t0 candidate sample.
    int            nTzeroCan() const;
    /// @brief Returns the earliest t0 candidate time [ns].
    double         tCanFirst() const;
    /// @brief Returns the latest t0 candidate time [ns].
    double         tCanLast() const;
    /// @brief Returns the number of VPD east hits.
    int            vpdEHits() const;
    /// @brief Returns the number of VPD west hits.
    int            vpdWHits() const;
    /// @brief Returns the number of good VPD east hits.
    int            vpdEGoodHits() const;
    /// @brief Returns the number of good VPD west hits.
    int            vpdWGoodHits() const;
    /// @brief Returns the earliest VPD east hit time [ns].
    double         earliestVpdEHit() const;
    /// @brief Returns the earliest VPD west hit time [ns].
    double         earliestVpdWHit() const;
    /// @brief Returns the VPD east hit time closest to the event start [ns].
    double         closestVpdEHit() const;
    /// @brief Returns the VPD west hit time closest to the event start [ns].
    double         closestVpdWHit() const;
    /// @brief Returns the latest VPD east hit time [ns].
    double         latestVpdEHit() const;
    /// @brief Returns the latest VPD west hit time [ns].
    double         latestVpdWHit() const;

    /// @brief Sets the fiber header word for the given fiber ID.
    void         setFiberHeader(int fiberId, short val);
    /// @brief Sets the fiber trigger word for the given fiber ID.
    void         setFiberTriggerWord(int fiberId, unsigned int val);
    /// @brief Sets the hit flag for the specified VPD tube.
    void         setVpdHit(StBeamDirection eastwest, int tubeId);
    /// @brief Clears the hit flag for the specified VPD tube.
    void         removeVpdHit(StBeamDirection eastwest, int tubeId);
    /// @brief Sets the VPD hit-pattern bitmask for east or west.
    void         setVpdHitPattern(StBeamDirection eastwest, unsigned int val);
    /// @brief Sets the VPD vertex z estimate at the given rank [cm].
    void         setVpdVz(float vz, int rank=0);
    /// @brief Sets the event start time from the VPD [ns].
    void         setTStart(double t);
    /// @brief Sets the uncertainty on the event start time [ns].
    void         setTStartError(double t_err);
    /// @brief Sets the VPD east–west time difference [ns].
    void         setTDiff(double tdiff);
    /// @brief Sets the measured VPD tube time for east or west and the given tube ID [ns].
    void         setVpdTime(StBeamDirection eastwest, int tubeId, double t);
    /// @brief Sets the trigger time TDC count for the given fiber ID.
    void         setTriggerTime(unsigned int tdc, int fiberId);
    /// @brief Sets the number of BTOF hits used for t0 determination.
    void         setNTzero(short n);

    /// @brief Sets the number of BTOF hits in the t0 candidate sample.
    void         setNTzeroCan(short nCan);
    /// @brief Sets the earliest t0 candidate time [ns].
    void         setTCanFirst(double tFirst);
    /// @brief Sets the latest t0 candidate time [ns].
    void         setTCanLast(double tLast);
    /// @brief Sets the number of VPD east hits.
    void         setVpdEHits(short vpdEHits);
    /// @brief Sets the number of VPD west hits.
    void         setVpdWHits(short vpdWHits);
    /// @brief Sets the number of good VPD east hits.
    void         setVpdEGoodHits(short vpdEGoodHits);
    /// @brief Sets the number of good VPD west hits.
    void         setVpdWGoodHits(short vpdWGoodHits);
    /// @brief Sets the earliest VPD east hit time [ns].
    void         setEarliestVpdEHit(double earliestVpdEHit);
    /// @brief Sets the earliest VPD west hit time [ns].
    void         setEarliestVpdWHit(double earliestVpdWHit);
    /// @brief Sets the VPD east hit time closest to the event start [ns].
    void         setClosestVpdEHit(double closestVpdEHit);
    /// @brief Sets the VPD west hit time closest to the event start [ns].
    void         setClosestVpdWHit(double closestVpdWHit);
    /// @brief Sets the latest VPD east hit time [ns].
    void         setLatestVpdEHit(double latestVpdEHit);
    /// @brief Sets the latest VPD west hit time [ns].
    void         setLatestVpdWHit(double latestVpdWHit);

protected:
    Short_t      mFiberHeader[MAXFIBER];         ///< Fiber header words, one per fiber
    UInt_t       mFiberTriggerWord[MAXFIBER];    ///< Fiber trigger words, one per fiber
    UInt_t       mVpdHitPattern[2];              ///< VPD hit-pattern bitmasks (east=0, west=1)
    Float_t      mVpdVz[MAXVPDVZ];              ///< VPD vertex z candidates [cm]
    Double_t     mTStart;                        ///< Event start time from VPD [ns]
    Double_t     mTStartErr;                     ///< Uncertainty on event start time [ns]
    Double_t     mTDiff;                         ///< VPD east–west time difference [ns]
    Double_t     mVpdTime[2][MAXVPD];            ///< Individual VPD tube times [ns] (east=0, west=1)
    UInt_t       mTriggerTime[MAXFIBER];         ///< Trigger time TDC counts, one per fiber
    Short_t      mNTzero;                        ///< Number of BTOF hits used for t0 determination
    Short_t      mNTzeroCan;                     ///< Number of BTOF hits in the t0 candidate sample
    Double_t     mTCanFirst;                     ///< Earliest t0 candidate time [ns]
    Double_t     mTCanLast;                      ///< Latest t0 candidate time [ns]
    Double_t     mVpdEHits;                      ///< Number of VPD east hits
    Double_t     mVpdWHits;                      ///< Number of VPD west hits
    Double_t     mVpdEGoodHits;                  ///< Number of good VPD east hits
    Double_t     mVpdWGoodHits;                  ///< Number of good VPD west hits
    Double_t     mEarliestVpdEHit;               ///< Earliest VPD east hit time [ns]
    Double_t     mEarliestVpdWHit;               ///< Earliest VPD west hit time [ns]
    Double_t     mClosestVpdEHit;                ///< VPD east hit closest to event start [ns]
    Double_t     mClosestVpdWHit;                ///< VPD west hit closest to event start [ns]
    Double_t     mLatestVpdEHit;                 ///< Latest VPD east hit time [ns]
    Double_t     mLatestVpdWHit;                 ///< Latest VPD west hit time [ns]

    ClassDef(StBTofHeader,3)
};

#endif
