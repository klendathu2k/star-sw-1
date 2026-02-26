/***************************************************************************
 *
 * $Id: StPhysicalHelix.hh,v 1.4 2005/07/06 18:49:56 fisyak Exp $
 *
 * Author: Brian Lasiuk, Sep 1997
 ***************************************************************************
 *
 * Description: 
 * Parametrization of a physical helix. See the SCL user guide for more.
 * 
 ***************************************************************************
 *
 * $Log: StPhysicalHelix.hh,v $
 * Revision 1.4  2005/07/06 18:49:56  fisyak
 * Replace StHelixD, StLorentzVectorD,StLorentzVectorF,StMatrixD,StMatrixF,StPhysicalHelixD,StThreeVectorD,StThreeVectorF by templated version
 *
 * Revision 1.3  2002/06/21 17:49:26  genevb
 * Some minor speed improvements
 *
 * Revision 1.2  2002/02/20 00:56:23  ullrich
 * Added methods to calculate signed DCA.
 *
 * Revision 1.1  1999/01/30 03:59:04  fisyak
 * Root Version of StarClassLibrary
 *
 * Revision 1.1  1999/01/23 00:27:59  ullrich
 * Initial Revision
 *
 **************************************************************************/
/*!
 * \file StPhysicalHelix.hh
 * \brief Physical helix with magnetic field and charge.
 * \author Brian Lasiuk, Sep 1997
 */
#ifndef ST_PHYSICAL_HELIX_HH
#define ST_PHYSICAL_HELIX_HH

#include "StThreeVector.hh"
#include "StHelix.hh"

/*!
 * \class StPhysicalHelix
 * \brief Helix of a charged particle in a uniform magnetic field.
 *
 * \details Extends StHelix by coupling the geometric parametrisation to
 * physical quantities: given a momentum 3-vector, an origin, the signed
 * magnetic-field strength, and the particle charge, it provides momentum
 * reconstruction, charge determination, and signed distance-of-closest-approach
 * (DCA) calculations.
 *
 * \sa StHelix
 */
class StPhysicalHelix : public StHelix {
public:
    /**
     * \brief Construct from physical quantities.
     * \param momentum  Momentum 3-vector at the origin.
     * \param origin    Starting point of the helix.
     * \param B         Signed magnetic field strength (in Tesla·units of SystemOfUnits).
     * \param charge    Particle charge (+1 or -1).
     */
    StPhysicalHelix(const StThreeVector<double>&,
		    const StThreeVector<double>&,
		    double, double);
    
    /**
     * \brief Construct from helix parameters directly.
     * \param curvature Helix curvature (1/radius).
     * \param dipAngle  Dip angle (pitch angle) in radians.
     * \param phase     Phase angle at origin.
     * \param origin    Origin point.
     * \param h         Sense of rotation: +1 (right-handed) or -1 (left-handed, default).
     */
    StPhysicalHelix(double, double, double,
		    const StThreeVector<double>&, int h=-1);
    /// Default constructor.
    StPhysicalHelix();
    
    ~StPhysicalHelix();

    /// Return the momentum 3-vector at the origin for the given signed magnetic field \a B.
    StThreeVector<double> momentum(double) const;
    /// Return the momentum 3-vector at arc-length \a s for the given signed magnetic field \a B.
    StThreeVector<double> momentumAt(double, double) const;
    /// Return the particle charge sign (+1 or -1) for the given signed magnetic field \a B.
    int                   charge(double)   const;
    /// Return the 2-D DCA to point (x,y) with sign relative to the curvature direction.
    double curvatureSignedDistance(double x, double y) ;
    /// Return the 2-D DCA to point (x,y) with sign relative to the rotation direction.
    double geometricSignedDistance(double x, double y) ;
    /// Return the 3-D DCA to a 3-D point with sign relative to the curvature direction.
    double curvatureSignedDistance(const StThreeVector<double>&) ;
    /// Return the 3-D DCA to a 3-D point with sign relative to the rotation direction.
    double geometricSignedDistance(const StThreeVector<double>&) ;
    
#ifdef __ROOT__
  ClassDef(StPhysicalHelix,1)
#endif
};

#endif
