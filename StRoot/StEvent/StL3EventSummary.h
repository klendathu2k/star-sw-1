/*!
 * \class StL3EventSummary
 * \author Christof Struck, July 2001
 * \brief Summary of L3 online trigger decisions and algorithm results for one event.
 */
/***************************************************************************
 *
 * $Id: StL3EventSummary.h,v 2.5 2002/02/22 22:56:49 jeromel Exp $
 *
 * Author: Christof Struck, July 2001
 ***************************************************************************
 *
 * Description: L3 Event Summary Information
 *
 ***************************************************************************
 *
 * $Log: StL3EventSummary.h,v $
 * Revision 2.5  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.4  2001/11/28 18:52:57  struck
 * updated classdef version
 *
 * Revision 2.3  2001/11/14 23:29:35  struck
 * minor changes in 'unbiasedTrigger' function, trigger word added for debugging purposes
 *
 * Revision 2.2  2001/08/20 21:29:53  ullrich
 * Added method setCounters().
 *
 * Revision 2.1  2001/08/02 01:26:31  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StL3EventSummary_hh
#define StL3EventSummary_hh

/// @file StL3EventSummary.h
/// @brief Summary of L3 online trigger decisions, algorithm results, and event counters.

#include "StObject.h"
#include "StContainers.h"
#include "StDaqLib/L3/L3_Banks.hh"

class StL3AlgorithmInfo;

/// @brief Aggregates the L3 online trigger outcome: event counters, algorithm accept/reject
///        decisions, unbiased-trigger flag, z-vertex trigger flag, and the L0 trigger word.
class StL3EventSummary : public StObject
{
public:
    StL3EventSummary();
    /// @brief Construct from the raw DAQ L3 summary bank.
    StL3EventSummary(Bank_L3_SUMD *l3sumd);
    // StL3EventSummary(const StL3EventSummary&);            use default
    // StL3EventSummary& operator=(const StL3EventSummray&); use default
    ~StL3EventSummary() {}

    /// @brief Return the total number of events processed by the L3 farm in this run segment.
    int                              numberOfProcessedEvents() const;
    /// @brief Return the number of events for which full TPC reconstruction was performed.
    int                              numberOfReconstructedEvents() const;
    /// @brief Return the number of online tracks reconstructed in this event.
    unsigned int                     numberOfTracks() const;
    /// @brief Return the number of L3 algorithms that ran on this event.
    unsigned int                     numberOfAlgorithms() const;

    /// @brief Return the pre-scale factor used for the unbiased (pass-through) trigger stream.
    int                              unbiasedTriggerPreScale() const;

    /// @brief Return true if this event was selected by the unbiased (pass-through) trigger.
    bool                             unbiasedTrigger() const;
    /// @brief Return true if this event passed the online z-vertex trigger.
    bool                             zVertexTrigger() const;

    /// @brief Return the L0 trigger word stored in the L3 summary for cross-check.
    unsigned int                     l0TriggerWord() const;

    /// @brief Return the list of algorithm objects that accepted this event.
    StPtrVecL3AlgorithmInfo&         algorithmsAcceptingEvent();
    const StPtrVecL3AlgorithmInfo&   algorithmsAcceptingEvent() const;

    /// @brief Return the full list of algorithm objects (including those that did not accept).
    StSPtrVecL3AlgorithmInfo&        algorithms();
    const StSPtrVecL3AlgorithmInfo&  algorithms() const;

    /// @brief Add an L3 algorithm info object (takes ownership).
    void                             addAlgorithm(StL3AlgorithmInfo*);
    /// @brief Set the number of online tracks.
    void                             setNumberOfTracks(int);
    /// @brief Set the processed and reconstructed event counters.
    void                             setCounters(int, int);
    /// @brief Mark this event as selected by the unbiased trigger.
    void                             setUnbiasedTrigger();
    /// @brief Set the pre-scale factor for the unbiased trigger stream.
    void                             setUnbiasedTriggerPreScale(int);
    /// @brief Mark this event as selected by the z-vertex trigger.
    void                             setZVertexTrigger();
    /// @brief Store the L0 trigger word in the L3 summary.
    void                             setL0TriggerWord(unsigned int);

private:
    Int_t    mNumberOfProcessedEvents;    ///< Total events processed by L3 farm.
    Int_t    mNumberReconstructedEvents;  ///< Events with full TPC reconstruction.
    UInt_t   mNumberOfTracks;             ///< Online tracks in this event.
    UInt_t   mNumberOfAlgorithms;         ///< Number of algorithms that ran.
    Bool_t   mZVertexTrigger;             ///< True if z-vertex trigger fired.
    Bool_t   mUnbiasedTrigger;            ///< True if unbiased pass-through trigger fired.
    UInt_t   mL0TriggerWord;              ///< L0 trigger word (for cross-check).
    Int_t    mUnbiasedPreScale;           ///< Pre-scale factor for unbiased stream.
    StPtrVecL3AlgorithmInfo   mL3AcceptAlgorithms; ///< Algorithms that accepted this event.
    StSPtrVecL3AlgorithmInfo  mL3Algorithms;       ///< All algorithms (owned).

    ClassDef(StL3EventSummary,2)
};


inline int
StL3EventSummary::numberOfProcessedEvents() const { return mNumberOfProcessedEvents; }

inline int
StL3EventSummary::numberOfReconstructedEvents() const { return mNumberReconstructedEvents; }

inline unsigned int
StL3EventSummary::numberOfTracks() const { return mNumberOfTracks; }

inline unsigned int
StL3EventSummary::numberOfAlgorithms() const { return mNumberOfAlgorithms; }

inline bool
StL3EventSummary::unbiasedTrigger() const { return mUnbiasedTrigger; }

inline bool
StL3EventSummary::zVertexTrigger() const { return mZVertexTrigger; }

inline unsigned int
StL3EventSummary::l0TriggerWord() const { return mL0TriggerWord; }

inline StPtrVecL3AlgorithmInfo&
StL3EventSummary::algorithmsAcceptingEvent() { return mL3AcceptAlgorithms; }

inline const StPtrVecL3AlgorithmInfo&
StL3EventSummary::algorithmsAcceptingEvent() const { return mL3AcceptAlgorithms; }

inline StSPtrVecL3AlgorithmInfo&
StL3EventSummary::algorithms() { return mL3Algorithms; }

inline const StSPtrVecL3AlgorithmInfo&
StL3EventSummary::algorithms() const { return mL3Algorithms; }

inline int
StL3EventSummary::unbiasedTriggerPreScale() const { return mUnbiasedPreScale; }

inline void
StL3EventSummary::setCounters(int nProcessed, int nReconstructed) {
      mNumberOfProcessedEvents = nProcessed;
      mNumberReconstructedEvents = nReconstructed;
}

inline void
StL3EventSummary::setUnbiasedTrigger() { mUnbiasedTrigger = true; }

inline void
StL3EventSummary::setZVertexTrigger() { mZVertexTrigger = true; }

inline void
StL3EventSummary::setUnbiasedTriggerPreScale(int preScale) { mUnbiasedPreScale = preScale; }

inline void
StL3EventSummary::setL0TriggerWord(unsigned int triggerWord) { mL0TriggerWord = triggerWord; }

#endif
