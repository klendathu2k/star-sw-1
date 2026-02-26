/**
 * \class StPrimaryTrack
 * \brief Reconstructed primary track (refitted through the primary vertex).
 * \author Thomas Ullrich, Sep 1999
 *
 * \details StPrimaryTrack is the concrete StTrack sub-class for tracks
 * that have been refitted with a constraint to pass through the primary
 * vertex, giving improved momentum resolution for prompt particles.
 * Each StPrimaryTrack is linked to its parent StPrimaryVertex and shares
 * hit-detector info with the corresponding StGlobalTrack sibling via the
 * common StTrackNode.
 *
 * Primary tracks are accessed through StPrimaryVertex::daughters() or
 * through the track node's track list.
 *
 * \sa StTrack, StGlobalTrack, StPrimaryVertex, StTrackNode
 */
/***************************************************************************
 *
 * $Id: StPrimaryTrack.h,v 2.13 2013/07/23 11:21:49 jeromel Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StPrimaryTrack.h,v $
 * Revision 2.13  2013/07/23 11:21:49  jeromel
 * Undo past week changes
 *
 * Revision 2.11  2013/04/10 19:15:53  jeromel
 * Step back from StEvent changes - previous change recoverable [Thomas OK-ed]
 *
 * Revision 2.9  2012/05/07 14:42:57  fisyak
 * Add handilings for Track to Fast Detectors Matching
 *
 * Revision 2.8  2009/11/23 22:20:51  ullrich
 * Minor cleanup performed, fixed compiler warnings.
 *
 * Revision 2.7  2009/11/23 16:34:06  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.6  2004/07/15 16:36:24  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.5  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.4  2001/05/30 17:45:54  perev
 * StEvent branching
 *
 * Revision 2.3  2001/03/24 03:34:53  perev
 * clone() -> clone() const
 *
 * Revision 2.2  1999/10/28 22:26:13  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:43:31  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StPrimaryTrack_hh
#define StPrimaryTrack_hh
/// @file StPrimaryTrack.h
/// @brief Reconstructed primary track (refitted through the primary vertex).

#include "StTrack.h"
class StPrimaryVertex;
class StPrimaryTrack;
ostream&  operator<<(ostream& os,  const StPrimaryTrack& t);

class StPrimaryTrack : public StTrack {
 public:
  StPrimaryTrack();
  ~StPrimaryTrack()  {/* noop */}

  StTrackType      type() const  { return primary; }     ///< Returns the track type (primary).
  /// @brief Returns the primary vertex this track is associated with.
  const StVertex*  vertex() const;
  
  /// @brief Set the primary vertex link for this track.
  void setVertex(StVertex*);
  void Print(Option_t *option="") const {cout << option << *this << endl; }
 private:
  //  StPrimaryVertex*         	mVertex; 	//$LINK
#ifdef __CINT__
  StObjLink  		mVertex; 	///< Link to the parent primary vertex.
#else
  StLink<StPrimaryVertex>  	mVertex; 	///< Link to the parent primary vertex.
#endif //__CINT__
  ClassDef(StPrimaryTrack,2)
};
#endif
