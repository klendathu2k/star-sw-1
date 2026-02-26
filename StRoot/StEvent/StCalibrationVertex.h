/*!
 * \class StCalibrationVertex 
 * \author Thomas Ullrich, Nov 2001
 *
 *               various types of vertices useful for calibration
 *               and diagnostics. No daughters, no parent.
 *
 */
/***************************************************************************
 *
 * $Id: StCalibrationVertex.h,v 2.5 2009/11/23 16:34:05 fisyak Exp $
 *
 * Author: Thomas Ullrich, Nov 2001
 ***************************************************************************
 *
 * Description: Concrete implementatin of StVertex to represent
 *              various types of vertices useful for calibration
 *              and diagnostics. No daughters, no parent.
 *
 ***************************************************************************
 *
 * $Log: StCalibrationVertex.h,v $
 * Revision 2.5  2009/11/23 16:34:05  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.4  2004/11/04 15:43:22  ullrich
 * Added set funyion for type.
 *
 * Revision 2.3  2004/07/15 16:36:23  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.2  2002/02/22 22:56:46  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.1  2001/11/10 23:52:14  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StCalibrationVertex_hh
#define StCalibrationVertex_hh
/// @file StCalibrationVertex.h
/// @brief Concrete StVertex sub-class for calibration and diagnostic vertices (no daughters, no parent).

#include "StVertex.h"

/// @brief Concrete StVertex sub-class for calibration and diagnostic vertices (no daughters or parent).
///
/// StCalibrationVertex is used to store vertices found during detector
/// calibration procedures (e.g. laser tracks, cosmic vertices).  It
/// does not hold daughter tracks and has no parent track.
class StCalibrationVertex : public StVertex {
public:
    /// @brief Default constructor.
    StCalibrationVertex();
    // StCalibrationVertex(const StCalibrationVertex&);            use default
    // StCalibrationVertex& operator=(const StCalibrationVertex&); use default
    virtual ~StCalibrationVertex();
    
    /// @brief Returns the calibration vertex type identifier.
    StVertexId     type() const;
    
    /// @brief Always returns 0; calibration vertices have no daughters.
    unsigned int   numberOfDaughters() const;
    /// @brief Always returns null; calibration vertices have no daughters.
    StTrack*       daughter(unsigned int);
    const StTrack* daughter(unsigned int) const;
    /// @brief Always returns an empty container; calibration vertices have no daughters.
    StPtrVecTrack  daughters(StTrackFilter&);
    
    /// @brief No-op; calibration vertices do not store daughter tracks.
    void addDaughter(StTrack*);
    /// @brief No-op; calibration vertices do not store daughter tracks.
    void removeDaughter(StTrack*);
    /// @brief Set the calibration vertex type identifier.
    void setType(StVertexId);

protected:    
    ClassDef(StCalibrationVertex,1)
};
#endif
