/*!
 * \class StL3AlgorithmInfo
 * \author Christof Struck, July 2001
 * \brief Stores configuration and per-event counters for a single L3 trigger algorithm.
 */
/***************************************************************************
 *
 * $Id: StL3AlgorithmInfo.h,v 2.4 2003/05/23 20:40:44 ullrich Exp $
 *
 * Author: Christof Struck, July 2001
 ***************************************************************************
 *
 * Description: L3 Algorithm Information
 *
 ***************************************************************************
 *
 * $Log: StL3AlgorithmInfo.h,v $
 * Revision 2.4  2003/05/23 20:40:44  ullrich
 * Removed dependcies on DAQ lib in header file.
 *
 * Revision 2.3  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/08/20 21:29:00  ullrich
 * Changed counter type from UInt_t to Int_t.
 *
 * Revision 2.1  2001/08/02 01:26:31  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StL3AlgorithmInfo_hh
#define StL3AlgorithmInfo_hh

/// @file StL3AlgorithmInfo.h
/// @brief Configuration and event-level counters for one L3 online trigger algorithm.

#include "StObject.h"
#include "TArrayF.h"
#include "TArrayI.h"

class Algorithm_Data;

/// @brief Holds configuration parameters and cumulative processing counters for a single
///        L3 online trigger algorithm, including accept/build decisions and pre/post-scale factors.
class StL3AlgorithmInfo : public StObject
{
public:
      StL3AlgorithmInfo();
      /// @brief Construct from raw DAQ Algorithm_Data bank.
      StL3AlgorithmInfo(Algorithm_Data*);
      ~StL3AlgorithmInfo();

      /// @brief Return the algorithm identifier.
      int          id() const;
      /// @brief Return true if this algorithm was active (enabled) in this run.
      bool         on() const;
      /// @brief Return true if this algorithm accepted the current event.
      bool         accept() const;
      /// @brief Return true if this algorithm requested full TPC reconstruction (build).
      bool         build() const;
      /// @brief Return the cumulative number of events processed by this algorithm.
      int          numberOfProcessedEvents() const;
      /// @brief Return the cumulative number of events accepted by this algorithm.
      int          numberOfAcceptedEvents() const;
      /// @brief Return the cumulative number of events for which a full build was requested.
      int          numberOfBuildEvents() const;
      /// @brief Return the number of floating-point data words stored for this event.
      int          dataSize() const;
      /// @brief Return the floating-point data word at @p index.
      float        data(int index) const;
      /// @brief Return the pre-scale factor (1-in-N pass-through before algorithm runs).
      int          preScale() const;
      /// @brief Return the post-scale factor (1-in-N keep after algorithm accepts).
      int          postScale() const;
      /// @brief Return the number of integer configuration parameters.
      int          intParameterSize() const;
      /// @brief Return the integer configuration parameter at index @p i.
      int          intParameter(int) const;
      /// @brief Return the number of floating-point configuration parameters.
      int          floatParameterSize() const;
      /// @brief Return the floating-point configuration parameter at index @p i.
      float        floatParameter(int) const;

      /// @brief Set the cumulative event counters (processed, accepted, built).
      void         setCounters(int, int, int);
      /// @brief Set the integer and floating-point configuration parameter arrays.
      void         setParameters(int*, float*);
      /// @brief Set the pre-scale factor.
      void         setPreScale(int);
      /// @brief Set the post-scale factor.
      void         setPostScale(int);

private:
      void         initArrays();

      Int_t    mId;                      ///< Algorithm identifier.
      Bool_t   mOn;                      ///< True if algorithm was enabled.
      Bool_t   mAccept;                  ///< True if algorithm accepted this event.
      Bool_t   mBuild;                   ///< True if full TPC build was requested.
      Int_t    mNumberOfProcessedEvents; ///< Cumulative count of processed events.
      Int_t    mNumberOfAcceptedEvents;  ///< Cumulative count of accepted events.
      Int_t    mNumberOfBuildEvents;     ///< Cumulative count of build-requested events.
      UShort_t mDataSize;                ///< Number of per-event data words.
      TArrayF  mDataArray;               ///< Per-event floating-point data words.
      Int_t    mPreScale;                ///< Pre-scale factor.
      Int_t    mPostScale;               ///< Post-scale factor.
      UShort_t mIntParameterSize;        ///< Number of integer configuration parameters.
      TArrayI  mIntParameterArray;       ///< Integer configuration parameters.
      UShort_t mFloatParameterSize;      ///< Number of floating-point configuration parameters.
      TArrayF  mFloatParameterArray;     ///< Floating-point configuration parameters.
      
      ClassDef(StL3AlgorithmInfo, 1)
};

inline int
StL3AlgorithmInfo::id() const { return mId; }

inline bool
StL3AlgorithmInfo::on() const { return mOn; }

inline bool
StL3AlgorithmInfo::accept() const { return mAccept; }

inline bool
StL3AlgorithmInfo::build() const { return mBuild; }

inline int
StL3AlgorithmInfo::numberOfProcessedEvents() const { return mNumberOfProcessedEvents; }

inline int
StL3AlgorithmInfo::numberOfAcceptedEvents() const { return mNumberOfAcceptedEvents; }

inline int
StL3AlgorithmInfo::numberOfBuildEvents() const { return mNumberOfBuildEvents; }

inline int
StL3AlgorithmInfo::dataSize() const { return mDataSize; }

inline float
StL3AlgorithmInfo::data(int i) const
{
      return i < mDataSize ? const_cast<TArrayF&>(mDataArray)[i] : 0;
}

inline int
StL3AlgorithmInfo::preScale() const { return mPreScale; }

inline int
StL3AlgorithmInfo::postScale() const { return mPostScale; }

inline int
StL3AlgorithmInfo::intParameterSize() const { return mIntParameterSize; }

inline int
StL3AlgorithmInfo::intParameter(int i) const
{ 
      return i < mIntParameterSize ? const_cast<TArrayI&>(mIntParameterArray)[i] : 0; 
}

inline int
StL3AlgorithmInfo::floatParameterSize() const { return mFloatParameterSize; }

inline float
StL3AlgorithmInfo::floatParameter(int i) const
{
      return i < mFloatParameterSize ? const_cast<TArrayF&>(mFloatParameterArray)[i] :0;
}
#endif
