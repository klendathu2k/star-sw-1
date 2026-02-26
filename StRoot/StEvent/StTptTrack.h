/*!
 * \class StTptTrack 
 * \author Thomas Ullrich, Aug 2000
 */
/***************************************************************************
 *
 * $Id: StTptTrack.h,v 2.5 2009/11/23 16:34:07 fisyak Exp $
 *
 * Author: Thomas Ullrich, Aug 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTptTrack.h,v $
 * Revision 2.5  2009/11/23 16:34:07  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.4  2004/07/15 16:36:25  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.3  2002/02/22 22:56:52  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/03/24 03:34:59  perev
 * clone() -> clone() const
 *
 * Revision 2.1  2000/08/17 00:10:38  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTptTrack_hh
#define StTptTrack_hh

/// @file StTptTrack.h
/// @brief Track class produced by the TPT (TPC Pattern recognition and Tracking) algorithm.

#include "StTrack.h"

/// @brief Track produced by the TPT (TPC Pattern recognition and Tracking) online algorithm.
class StTptTrack : public StTrack {
public:
    /// @brief Default constructor.
    StTptTrack();
    /// @brief Destructor.
    ~StTptTrack();

    /// @brief Returns the track type identifier (tpt track).
    StTrackType     type() const;
    /// @brief Returns the primary vertex associated with this track (always null for TPT tracks).
    const StVertex* vertex() const;

    ClassDef(StTptTrack,1)
};
#endif
