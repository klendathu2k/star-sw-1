/***************************************************************************
 *
 * $Id: StAngle.hh,v 1.1 2001/04/20 04:54:26 ullrich Exp $
 *
 * Author: Thomas Ullrich, April 2001
 ***************************************************************************
 *
 * Description:  
 * Keeps an angle within the range of [-pi, pi] and deals with differences
 * between angles in the right way. Units are radian.
 * Other than that it can be handled pretty much like a 'double'.
 *
 ***************************************************************************
 *
 * $Log: StAngle.hh,v $
 * Revision 1.1  2001/04/20 04:54:26  ullrich
 * Initial Revision.
 *
 **************************************************************************/
/*!
 * \file StAngle.hh
 * \brief Angle value confined to [-π, π] with correct circular arithmetic.
 */
#ifndef StAngle_hh
#define StAngle_hh

#include <cmath>
#include <float.h>

/*!
 * \class StAngle
 * \brief Angle value confined to [-π, π] with correct circular arithmetic.
 * \details Keeps an angle within the range [-π, π] in radians and performs
 *          all arithmetic (addition, subtraction, scaling) with proper
 *          wrap-around so that differences between angles are always
 *          meaningful.  The class can be used transparently as a \c double
 *          through the implicit conversion operator.
 */
class StAngle {
public:
    StAngle();
    StAngle(const StAngle& val);
    StAngle(double);

    /// \brief Return the angle in degrees in the range [0, 360).
    /// \return Angle converted to degrees, mapped to [0, 360).
    double degree();  // range [0-360]

    /// \brief Implicit conversion to \c double (radians in [-π, π]).
    /// \return Angle value in radians.
    operator double() const { return phi; }

    /// \brief Assign a raw radian value, wrapping to [-π, π].
    /// \param val Radian value to assign.
    /// \return Reference to this angle after assignment.
    StAngle operator= (double val);

    /// \brief Add another angle in-place, keeping result in [-π, π].
    /// \param val Angle to add.
    /// \return This angle after addition.
    StAngle operator+= (StAngle val);

    /// \brief Subtract another angle in-place, keeping result in [-π, π].
    /// \param val Angle to subtract.
    /// \return This angle after subtraction.
    StAngle operator-= (StAngle val);

    /// \brief Scale angle by a scalar in-place, keeping result in [-π, π].
    /// \param val Scalar multiplier.
    /// \return This angle after scaling.
    StAngle operator*= (double val);

    /// \brief Divide angle by a scalar in-place, keeping result in [-π, π].
    /// \param val Scalar divisor.
    /// \return This angle after division.
    StAngle operator/= (double val);

    /// \brief Test equality within floating-point epsilon.
    /// \param a Angle to compare with.
    /// \return Non-zero if the angles are equal within FLT_EPSILON.
    int     operator== (const StAngle& a) const;

    /// \brief Add two angles, returning result wrapped to [-π, π].
    friend StAngle operator+ (StAngle, StAngle);
    /// \brief Subtract two angles, returning result wrapped to [-π, π].
    friend StAngle operator- (StAngle, StAngle);
    /// \brief Multiply an angle by a scalar, result wrapped to [-π, π].
    friend StAngle operator* (StAngle, double);
    /// \brief Divide an angle by a scalar, result wrapped to [-π, π].
    friend StAngle operator/ (StAngle, double);

    /// \brief Compute the circular average of two angles.
    /// \return The angle halfway between \p a and \p b along the shorter arc.
    friend StAngle average(StAngle, StAngle);
    
private: 
    double phi;
};

//
// Constructors for angles
//

inline StAngle::StAngle() 
{
   phi = 0.;   // default: set to 0
}

inline StAngle::StAngle(const StAngle& val) 
{
   phi = val.phi;
}

inline StAngle::StAngle(double val) 
{
   phi = val;
   if (fabs(phi) > M_PI) phi = atan2(sin(phi), cos(phi));
}

//
// Here follow the member functions for the operators ==, !=, =, +=, -=, *=, /=
//

inline StAngle StAngle::operator= (double val) 
{
   phi = val;
   if (fabs(phi) > M_PI) phi = atan2(sin(phi), cos(phi));
   return *this;
}

inline StAngle StAngle::operator+= (StAngle val) 
{
   phi += val.phi;
   if (fabs(phi) > M_PI) phi = atan2(sin(phi), cos(phi));
   return *this;
}

inline StAngle StAngle::operator-= (StAngle val) 
{
   phi -= val.phi;
   if (fabs(phi) > M_PI) phi = atan2(sin(phi), cos(phi));
   return *this;
}

inline StAngle StAngle::operator*= (double val) 
{
   phi *= val;
   if (fabs(phi) > M_PI) phi = atan2(sin(phi), cos(phi));
   return *this;
}

inline StAngle StAngle::operator/= (double val) 
{
   phi /= val;
   if (fabs(phi) > M_PI) phi = atan2(sin(phi), cos(phi));
   return *this;
}

inline int StAngle::operator== (const StAngle& a) const
{
   return (fabs(StAngle(phi-a.phi)) < FLT_EPSILON);
}

//
// The friends for operator +, -, *, /
//

inline StAngle operator+ (StAngle angle, StAngle val) 
{
   double res = angle.phi + val.phi;
   if (fabs(res) > M_PI) res = atan2(sin(res), cos(res));
   return res;
}

inline StAngle operator- (StAngle angle, StAngle val) 
{
   double res = angle.phi - val.phi;
   if (fabs(res) > M_PI) res = atan2(sin(res), cos(res));
   return res;
}

inline StAngle operator* (StAngle angle, double val) 
{
   double res = angle.phi * val;
   if (fabs(res) > M_PI) res = atan2(sin(res), cos(res));
   return res;
}

inline StAngle operator/ (StAngle angle, double val) 
{
   double res = angle.phi / val;
   if (fabs(res) > M_PI) res = atan2(sin(res), cos(res));
   return res;
}

inline StAngle average(StAngle a, StAngle b)
{
   return a+(b-a)/2.;
}

//
//  Return angle in degrees [0, 360]
//

inline double StAngle::degree()
{
   double val = phi*180./M_PI;  
   return (val < 0) ? val += 360 : val;
}


#endif
