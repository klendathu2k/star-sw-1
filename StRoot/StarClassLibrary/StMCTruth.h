/***************************************************************************
 *
 * $Id: StMCTruth.h,v 1.3 2009/12/17 08:37:26 fisyak Exp $
 *
 * Author: Victor Perev, Jun 2005
 ***************************************************************************
 *
 * Description: 
 *
 ***************************************************************************
 *
 * $Log: StMCTruth.h,v $
 * Revision 1.3  2009/12/17 08:37:26  fisyak
 * account signature change snce root 5.24
 *
 * Revision 1.2  2005/09/09 21:42:03  perev
 * Method Size() added
 *
 * Revision 1.1  2005/07/19 22:40:32  perev
 * IdTruth classes
 *
 *
 **************************************************************************/
/*!
 * \file StMCTruth.h
 * \brief MC truth track-identity helpers for STAR hit-level reconstruction.
 */
#ifndef ST_MCTRUTH_H
#define ST_MCTRUTH_H
#include "Rtypes.h"
#if ROOT_VERSION_CODE <= 333312 /*  ROOT_VERSION(5,22,0) */
typedef Long_t   LongKey_t;
#else
typedef Long64_t LongKey_t;
#endif
/*!
 * \struct StMCTruth
 * \brief Compact 32-bit record pairing a Geant track ID with its fractional contribution.
 */
struct StMCTruth {
    StMCTruth(int id,int wt) 		{trackId=(short)id; trackWt=(short)wt;}
    StMCTruth(int word=0) 		{*this=word;}
StMCTruth &operator=(int word);
           operator int() const;	//conversion to int

    short trackId;     ///< Geant track id.
    short trackWt;     ///< Contribution or weight to signal (in promille).

};


/*!
 * \class StMCPivotTruth
 * \brief Accumulates weighted MC truth contributions for a single detector element.
 */
class StMCPivotTruth {
public:
       StMCPivotTruth(int normInput=0);
  /// \brief Reset the accumulator, discarding all recorded contributions.
  void Reset() {fN=0;}
  /// \brief Add a contribution by raw track id and floating-point weight.
  /// \param trackId  Geant track identifier.
  /// \param wt       Weight (energy deposit or hit count) of the contribution.
  void Add(int trackId, double wt);
  /// \brief Add a contribution given an StMCTruth record and an explicit weight.
  /// \param Id  Truth record containing the track id.
  /// \param wt  Additional weight to apply on top of the record's own weight.
  void Add(StMCTruth Id,double wt);
  /// \brief Add a contribution using the weight already stored in the StMCTruth record.
  /// \param Id  Truth record to accumulate.
  void Add(StMCTruth Id);
  /// \brief Return the number of distinct MC tracks accumulated so far.
  /// \return Count of accumulated track entries.
  int  Size() const {return fN;}
  /// \brief Retrieve the dominant (or Nth most significant) truth entry.
  /// \param byCount  0 selects the highest-weight contributor; higher values walk down the list.
  /// \return StMCTruth record for the requested contributor.
  StMCTruth Get(int byCount=0) const;
private:
  enum {HOWMANY=20};
int    fN;
int    fNorm;
int    mTrackIds[HOWMANY];
float  mTrackWts[HOWMANY];
float  mTrackNum[HOWMANY];
int qwe;
};

class TExMap;
class TExMapIter;
/*!
 * \class StMCPivotTruthMap
 * \brief Maps arbitrary keys to their dominant MC truth contributor.
 */
class StMCPivotTruthMap {

public:
       StMCPivotTruthMap(int normInput=0);
      ~StMCPivotTruthMap();
//void Reset();
  /// \brief Record a weighted contribution from \p trackId for the element identified by \p token.
  /// \param token    Arbitrary key identifying the detector element (e.g., channel id).
  /// \param trackId  Geant track identifier.
  /// \param wt       Contribution weight.
  void Add(LongKey_t token, int trackId, double wt);
  /// \brief Record a contribution using a pre-formed StMCTruth record.
  /// \param token  Key of the detector element.
  /// \param truth  StMCTruth record to accumulate.
  void Add(LongKey_t token, StMCTruth truth);
  /// \brief Retrieve the dominant truth contributor for a given detector element.
  /// \param token    Key of the detector element to query.
  /// \param byCount  0 returns the highest-weight contributor.
  /// \return StMCTruth record for the dominant contributor, or a zeroed record if not found.
  StMCTruth Get(LongKey_t token,int byCount=0) const;
  /// \brief Iterate over all stored elements and return the truth for each.
  /// \param token  Set to -1 to start iteration; updated to the current key on each call;
  ///               set back to -1 when iteration is complete.
  /// \return StMCTruth record for the current element.
  StMCTruth Iter(LongKey_t &token) const; //token=-1 to start iteration
                                     //token=-1 at the end of iteration
private:
int    fNorm;
TExMap *fMap;
mutable TExMapIter *fIter;
};




#endif
