/*!
 * \class StTrackNode 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StTrackNode.h,v 2.6 2002/02/22 22:56:53 jeromel Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTrackNode.h,v $
 * Revision 2.6  2002/02/22 22:56:53  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.5  2001/04/05 04:00:45  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.4  1999/11/09 15:44:19  ullrich
 * Removed method unlink() and all calls to it.
 *
 * Revision 2.3  1999/11/05 15:27:12  ullrich
 * Added non-const versions of several methods
 *
 * Revision 2.2  1999/10/28 22:27:47  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:44:15  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StTrackNode_hh
#define StTrackNode_hh
/// @file StTrackNode.h
/// @brief Groups a global track with its associated primary track(s) in a single node.

#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"
class StTrack;

/// @brief Groups a global track with its associated primary track(s) sharing the same detector hits.
///
/// StTrackNode acts as an owner of one StGlobalTrack and zero or more
/// StPrimaryTrack objects that were refitted from the same set of hits.
/// All track nodes for an event are stored in StEvent::trackNodes().
class StTrackNode : public StObject {
public:
    /// @brief Default constructor.
    StTrackNode();
    virtual ~StTrackNode();

    /// @brief Add a track to this node; owned tracks are stored in mOwnedTracks.
    void           addTrack(StTrack*);
    /// @brief Remove a track from this node's lists.
    void           removeTrack(StTrack*);

    /// @brief Total number of tracks (owned + referenced) held by this node.
    unsigned int   entries() const;
    /// @brief Track by sequential index over all tracks in this node.
    StTrack*       track(unsigned int);
    const StTrack* track(unsigned int) const;

    /// @brief Number of tracks of the specified type held by this node.
    unsigned int   entries(StTrackType) const;
    /// @brief Track of a given type by index (default 0 = first match).
    StTrack*       track(StTrackType, unsigned int = 0);
    const StTrack* track(StTrackType, unsigned int = 0) const;

private:
    StTrackNode(const StTrackNode&);
    StTrackNode& operator=(const StTrackNode&);
    
    StSPtrVecTrack  mOwnedTracks;      ///< Tracks owned (and memory-managed) by this node.
    StPtrVecTrack   mReferencedTracks; ///< Non-owning pointers to tracks associated with this node.

    ClassDef(StTrackNode,1)
};
#endif
