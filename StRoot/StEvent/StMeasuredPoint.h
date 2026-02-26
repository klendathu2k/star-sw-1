/*!
 * \class StMeasuredPoint 
 * \author Thomas Ullrich, Sept 1999
 */
/***************************************************************************
 *
 * $Id: StMeasuredPoint.h,v 2.8 2005/07/06 18:58:15 fisyak Exp $
 *
 * Author: Thomas Ullrich, Sept 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StMeasuredPoint.h,v $
 * Revision 2.8  2005/07/06 18:58:15  fisyak
 * Add print out
 *
 * Revision 2.7  2004/07/15 16:36:24  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.6  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.5  2001/04/05 04:00:38  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.4  2001/03/24 03:34:52  perev
 * clone() -> clone() const
 *
 * Revision 2.3  1999/12/21 15:09:02  ullrich
 * Modified to cope with new compiler version on Sun (CC5.0).
 *
 * Revision 2.2  1999/10/28 22:26:05  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:43:26  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StMeasuredPoint_hh
#define StMeasuredPoint_hh
/// @file StMeasuredPoint.h
/// @brief Abstract base class for any detector measurement with a 3-D position.

#include <Stiostream.h>
#include "StObject.h"
#include "StThreeVectorF.hh"
#include "StMatrixF.hh"

/// @brief Abstract base class for measured space points with a 3-D position.
///
/// StMeasuredPoint provides the common position storage and interface for all
/// detector hits and reconstructed vertices in STAR.  Derived classes must
/// implement positionError() and covariantMatrix().
class StMeasuredPoint : public StObject {
public:
    /// @brief Default constructor; initialises position to zero.
    StMeasuredPoint();
    /// @brief Construct with a given 3-D position.
    StMeasuredPoint(const StThreeVectorF&);
    virtual ~StMeasuredPoint();
    // StMeasuredPoint(const StMeasuredPoint&);            use default
    // StMeasuredPoint& operator=(const StMeasuredPoint&); use default
    
    int operator==(const StMeasuredPoint&) const;
    int operator!=(const StMeasuredPoint&) const;
    
    /// @brief 3-D position of the measured point (cm).
    virtual const StThreeVectorF& position() const;
    /// @brief 1-sigma position errors (cm); pure virtual, implemented by sub-classes.
    virtual StThreeVectorF        positionError() const = 0;
    /// @brief Full 3×3 position covariance matrix; pure virtual, implemented by sub-classes.
    virtual StMatrixF             covariantMatrix() const = 0;
    
    /// @brief Set the 3-D position of this measured point.
    virtual void setPosition(const StThreeVectorF&);
    virtual void                  Print(Option_t *option="") const;
protected:
    StThreeVectorF mPosition; ///< 3-D position of the measured point (cm).
    ClassDef(StMeasuredPoint,1)
};
ostream&              operator<<(ostream& os, StMeasuredPoint const & v);
#endif
