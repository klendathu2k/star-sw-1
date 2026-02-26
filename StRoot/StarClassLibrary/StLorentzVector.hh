/***************************************************************************
 *
 * $Id: StLorentzVector.hh,v 1.14 2012/06/11 15:29:26 fisyak Exp $
 *
 * Author: Brian Lasiuk, Thomas Ullrich, April 1998
 ***************************************************************************
 *
 * Description:
 *
 * Remarks:   Since not all compilers support member templates
 *            we have to specialize the templated member on these
 *            platforms. If member templates are not supported the
 *            ST_NO_MEMBER_TEMPLATES flag has to be set. tu.
 *
 *            In the near future when all compilers can handle member
 *            templates this class should be cleaned up. A lot of
 *            redundant code can be removed as soon as the compilers
 *            are up-to-date. tu
 *
 ***************************************************************************
 *
 * $Log: StLorentzVector.hh,v $
 * Revision 1.14  2012/06/11 15:29:26  fisyak
 * std namespace
 *
 * Revision 1.13  2009/09/22 16:42:38  fine
 * Add the extra ctor to complete the neet the template specilication signatures #1612
 *
 * Revision 1.12  2006/01/09 23:47:27  fisyak
 * Add missing methods (found by Zhangbu) to Cint dictionary
 *
 * Revision 1.11  2005/09/22 20:09:20  fisyak
 * Make StLorentzVector persistent
 *
 * Revision 1.10  2005/07/06 18:49:56  fisyak
 * Replace StHelixD, StLorentzVectorD,StLorentzVectorF,StMatrixD,StMatrixF,StPhysicalHelixD,StThreeVectorD,StThreeVectorF by templated version
 *
 * Revision 1.9  2005/03/28 06:02:45  perev
 * Defence FPE added
 *
 * Revision 1.8  2003/09/02 17:59:35  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 1.7  2003/05/01 19:24:31  ullrich
 * Corrected problem in boost().
 *
 * Revision 1.6  1999/10/15 15:56:36  ullrich
 * Changed output format in operator<<, added operator>>
 *
 * Revision 1.5  1999/06/04 18:01:36  ullrich
 * New operators operator() and operator[] which can be used
 * as lvalues.
 *
 * Revision 1.4  1999/04/14 23:12:07  fisyak
 * Add __CINT__ to handle references
 *
 * Revision 1.3  1999/02/17 11:38:36  ullrich
 * Removed specialization for 'long double'.
 *
 * Revision 1.2  1999/02/14 23:11:42  fisyak
 * Fixes for Rootcint
 *
 * Revision 1.1  1999/01/30 03:59:02  fisyak
 * Root Version of StarClassLibrary
 *
 * Revision 1.1  1999/01/23 00:27:52  ullrich
 * Initial Revision
 *
 **************************************************************************/
/*!
 * \file StLorentzVector.hh
 * \brief Template Lorentz 4-vector (px, py, pz, E).
 * \author Brian Lasiuk, Thomas Ullrich, April 1998
 */
#ifndef ST_LORENTZ_VECTOR_HH
#define ST_LORENTZ_VECTOR_HH

#include "StThreeVector.hh"
/*!
 * \class StLorentzVector
 * \brief Template Lorentz 4-vector (px, py, pz, E) for relativistic kinematics.
 *
 * \tparam T Numeric type (typically \c float or \c double).
 *
 * \details Stores a momentum 3-vector and an energy/time component.
 * Provides all standard relativistic quantities: invariant mass, rapidity,
 * transverse mass, boost, etc.  Instantiated in practice as
 * \c StLorentzVectorF (float) and \c StLorentzVectorD (double).
 * Modelled after the CLHEP \c HepLorentzVector class.
 */
template<class T> class StLorentzVector {
public:
    /// Construct from explicit (x, y, z, t) components.
    StLorentzVector(T, T, T, T);
    /// Default constructor; initialises all components to zero.
    StLorentzVector();
    virtual ~StLorentzVector();
    
    template<class X> StLorentzVector(const StThreeVector<X>&, T);
    template<class X, class Y> StLorentzVector(const StThreeVector<X>&, Y);
    template<class X, class Y> StLorentzVector(Y, const StThreeVector<X>&);   
    template<class X> StLorentzVector(T, const StThreeVector<X>&);   

    template<class X> StLorentzVector(const StLorentzVector<X>&);
    template<class X> StLorentzVector<T>& operator=(const StLorentzVector<X>&);
    // StLorentzVector(const StLorentzVector<T>&);                use default
    // StLorentzVector<T>& operator=(const StLorentzVector<T>&);  use default
    /// Return the x (px) component.
    T x()                     const;
    /// Return the y (py) component.
    T y()                     const;
    /// Return the z (pz) component.
    T z()                     const;
    /// Return the time (energy) component.
    T t()                     const;
    /// Return the x momentum component (synonym for x()).
    T px()                    const;
    /// Return the y momentum component (synonym for y()).
    T py()                    const;
    /// Return the z momentum component (synonym for z()).
    T pz()                    const;
    /// Return the energy component (synonym for t()).
    T e()                     const;
    /// Element access by index (0–2: px/py/pz, 3: E); throws out_of_range if index > 3.
    T operator()  (size_t)    const;
    /// Element access by index (0–2: px/py/pz, 3: E); throws out_of_range if index > 3.
    T operator[]  (size_t)    const;
    
    /// Mutable element access by index (0–2: px/py/pz, 3: E).
    T& operator()  (size_t);
    /// Mutable element access by index (0–2: px/py/pz, 3: E).
    T& operator[]  (size_t);

    /// Return a const reference to the spatial 3-vector component.
    const StThreeVector<T>& vect() const;    
    
    /// Set the x (px) component.
    void setX(T);
    /// Set the y (py) component.
    void setY(T);
    /// Set the z (pz) component.
    void setZ(T);
    /// Set the x momentum component (synonym for setX()).
    void setPx(T);
    /// Set the y momentum component (synonym for setY()).
    void setPy(T);
    /// Set the z momentum component (synonym for setZ()).
    void setPz(T);
    /// Set the energy component (synonym for setT()).
    void setE(T);
    /// Set the time (energy) component.
    void setT(T);
    
#if !defined(ST_NO_MEMBER_TEMPLATES) && !defined(__CINT__)
    template <class X> void setVect(const StThreeVector<X>&);
#else
    void setVect(const StThreeVector<float>&);
    void setVect(const StThreeVector<double>&);
#endif   

    /// Return the transverse momentum \f$p_T = \sqrt{p_x^2+p_y^2}\f$.
    T perp()               const;
    /// Return \f$p_T^2\f$.
    T perp2()              const;
    /// Return the pseudorapidity \f$\eta\f$ of the spatial 3-vector.
    T pseudoRapidity()     const;
    /// Return the azimuthal angle \f$\phi\f$ of the spatial 3-vector (radians).
    T phi()                const;
    /// Return the polar angle \f$\theta\f$ of the spatial 3-vector (radians).
    T theta()              const;
    /// Return \f$\cos\theta\f$ of the spatial 3-vector.
    T cosTheta()           const;
    
    /// Return the light-cone plus component \f$E + p_z\f$.
    T plus()               const;
    /// Return the light-cone minus component \f$E - p_z\f$.
    T minus()              const;
    
    /// Return the invariant mass \f$m = \text{sign}(m^2)\sqrt{|m^2|}\f$.
    T m()                  const; 
    /// Return the Lorentz invariant \f$m^2 = E^2 - |\mathbf{p}|^2\f$.
    T m2()                 const; 
    /// Return the transverse mass \f$m_T = \text{sign}(m_T^2)\sqrt{|m_T^2|}\f$.
    T mt()                 const;
    /// Return \f$m_T^2 = p_T^2 + m^2\f$.
    T mt2()                const;
    /// Return the rapidity \f$y = \frac{1}{2}\ln\!\left(\frac{E+p_z}{E-p_z}\right)\f$.
    T rapidity()           const;
    
#if !defined(ST_NO_MEMBER_TEMPLATES) && !defined(__CINT__)
    /// Boost this 4-vector into the rest frame defined by \a pframe.
    template<class X> StLorentzVector<T> boost(const StLorentzVector<X>&) const;
#else
    StLorentzVector<T> boost(const StLorentzVector<float>&) const;
    StLorentzVector<T> boost(const StLorentzVector<double>&) const;
#endif   
    
    /// Unary negation: return \f$(-E, -\mathbf{p})\f$.
    StLorentzVector<T>  operator- ();
    /// Unary plus: return a copy of this 4-vector.
    StLorentzVector<T>  operator+ ();
    /// Multiply all components by scalar \a c in place.
    StLorentzVector<T>& operator*= (double);
    /// Divide all components by scalar \a c in place.
    StLorentzVector<T>& operator/= (double);

#if !defined(ST_NO_MEMBER_TEMPLATES) && !defined(__CINT__)
    template<class X> bool operator == (const StLorentzVector<X>&) const;
    template<class X> bool operator != (const StLorentzVector<X>&) const;
    template<class X> StLorentzVector<T>& operator+= (const StLorentzVector<X>&);
    template<class X> StLorentzVector<T>& operator-= (const StLorentzVector<X>&);
#else    
    bool operator == (const StLorentzVector<float>&) const;
    bool operator != (const StLorentzVector<float>&) const;
    bool operator == (const StLorentzVector<double>&) const;
    bool operator != (const StLorentzVector<double>&) const;

    StLorentzVector<T>& operator+= (const StLorentzVector<float>&);
    StLorentzVector<T>& operator-= (const StLorentzVector<float>&);
    StLorentzVector<T>& operator+= (const StLorentzVector<double>&);
    StLorentzVector<T>& operator-= (const StLorentzVector<double>&);
#endif

protected:
    StThreeVector<T> mThreeVector;
    T	             mX4;
#ifdef __ROOT__
  ClassDef(StLorentzVector,3)
#endif
};
#ifndef __CINT__
//
//        Implementation of member functions
//
template<class T>
StLorentzVector<T>::StLorentzVector()
    : mThreeVector(0, 0, 0), mX4(0) { /* nop */ }

template<class T>
StLorentzVector<T>::StLorentzVector(T X, T Y, T Z, T Time)
    : mThreeVector(X, Y, Z), mX4(Time) { /* nop */ }

template<class T>
StLorentzVector<T>::~StLorentzVector() { /* nopt */ }    

template<class T>
const StThreeVector<T>& StLorentzVector<T>::vect() const 
{
    return mThreeVector;
}

template<class T>
T StLorentzVector<T>::m2() const
{
    return (mX4*mX4 - mThreeVector*mThreeVector);    
}

template<class T>
T StLorentzVector<T>::plus() const { return (e() + pz()); }

template<class T>
T StLorentzVector<T>::minus() const { return (e() - pz()); }

template<class T>
T StLorentzVector<T>::m() const
{
    T mass2 = m2();
    if (mass2 < 0)
	return -::sqrt(-mass2);
    else
	return ::sqrt(mass2);
}

template<class T>
T StLorentzVector<T>::mt2() const
{
    return this->perp2() + m2();
}

template<class T>
T StLorentzVector<T>::mt() const
{
    //
    // change to more optimal code ?
    // return e()*e() - pz()*pz();
    T massPerp2 = mt2();
    if (massPerp2 < 0)
	return -::sqrt(-massPerp2);
    else
	return ::sqrt(massPerp2);
}

template<class T>
void StLorentzVector<T>::setPx(T X) {mThreeVector.setX(X);}

template<class T>
void StLorentzVector<T>::setPy(T Y) {mThreeVector.setY(Y);}

template<class T>
void StLorentzVector<T>::setPz(T Z) {mThreeVector.setZ(Z);}

template<class T>
void StLorentzVector<T>::setX(T X) {mThreeVector.setX(X);}

template<class T>
void StLorentzVector<T>::setY(T Y) {mThreeVector.setY(Y);}

template<class T>
void StLorentzVector<T>::setZ(T Z) {mThreeVector.setZ(Z);}

template<class T>
void StLorentzVector<T>::setT(T Time) {mX4 = Time;}

template<class T>
void StLorentzVector<T>::setE(T E) {mX4 = E;}

template<class T>
T StLorentzVector<T>::x() const {return mThreeVector.x();}

template<class T>
T StLorentzVector<T>::y() const {return mThreeVector.y();}

template<class T>
T StLorentzVector<T>::z() const {return mThreeVector.z();}

template<class T>
T StLorentzVector<T>::px() const {return mThreeVector.x();}

template<class T>
T StLorentzVector<T>::py() const {return mThreeVector.y();}

template<class T>
T StLorentzVector<T>::pz() const {return mThreeVector.z();}

template<class T>
T StLorentzVector<T>::e() const {return mX4;}

template<class T>
T StLorentzVector<T>::t() const {return mX4;}

template<class T>
T StLorentzVector<T>::perp() const {return mThreeVector.perp();}

template<class T>
T StLorentzVector<T>::perp2() const {return mThreeVector.perp2();}

template<class T>
T StLorentzVector<T>::pseudoRapidity() const {return mThreeVector.pseudoRapidity();}

template<class T>
T StLorentzVector<T>::phi() const {return mThreeVector.phi();}

template<class T>
T StLorentzVector<T>::theta() const {return mThreeVector.theta();}

template<class T>
T StLorentzVector<T>::cosTheta() const {return mThreeVector.cosTheta();}

template<class T>
T StLorentzVector<T>::operator() (size_t i) const
{
    if (i < 3)
        return mThreeVector(i);
    else if (i == 3)
        return mX4;
    else {
#ifndef ST_NO_EXCEPTIONS
      throw out_of_range("StLorentzVector<T>::operator(): bad index");  
#else
      cerr << "StLorentzVector<T>::operator(): bad index." << endl;
#endif
      return 0;
    }
}

template<class T>
T& StLorentzVector<T>::operator() (size_t i)
{
    if (i < 3)
        return mThreeVector(i);
    else if (i == 3)
        return mX4;
    else {
#ifndef ST_NO_EXCEPTIONS
      throw out_of_range("StLorentzVector<T>::operator(): bad index");  
#else
      cerr << "StLorentzVector<T>::operator(): bad index." << endl;
#endif
      return mX4;
    }
}

template<class T>
T StLorentzVector<T>::operator[] (size_t i) const
{
    if (i < 3)
        return mThreeVector[i];
    else if (i == 3)
        return mX4;
    else {
#ifndef ST_NO_EXCEPTIONS
      throw out_of_range("StLorentzVector<T>::operator[]: bad index"); 
#else
      cerr << "StLorentzVector<T>::operator[]: bad index." << endl;
#endif
      return 0;
    }
}

template<class T>
T& StLorentzVector<T>::operator[] (size_t i)
{
    if (i < 3)
        return mThreeVector[i];
    else if (i == 3)
        return mX4;
    else {
#ifndef ST_NO_EXCEPTIONS
      throw out_of_range("StLorentzVector<T>::operator[]: bad index"); 
#else
      cerr << "StLorentzVector<T>::operator[]: bad index." << endl;
#endif
      return mX4;
    }
}

template<class T>
T StLorentzVector<T>::rapidity() const
{
    return 0.5*::log((mX4+mThreeVector.z())/(mX4-mThreeVector.z())+1e-20);
}

template<class T>
StLorentzVector<T> StLorentzVector<T>::operator- ()
{
    return StLorentzVector<T>(-mX4,-mThreeVector);
}

template<class T>
StLorentzVector<T> StLorentzVector<T>::operator+ ()
{
    return *this;
}

template<class T>
StLorentzVector<T>& StLorentzVector<T>::operator*= (double c)
{
    mThreeVector *= c;
    mX4 *= c;
    return *this;
}

template<class T>
StLorentzVector<T>& StLorentzVector<T>::operator/= (double c)
{
    mThreeVector /= c;
    mX4 /= c;
    return *this;
}

#if !defined(ST_NO_MEMBER_TEMPLATES) && !defined(__CINT__)

template<class T>
template<class X>
StLorentzVector<T>::StLorentzVector(const StThreeVector<X> &vec, T Time)
	: mThreeVector(vec), mX4(Time) { /* nop */ }

template<class T>
template<class X>
StLorentzVector<T>::StLorentzVector(T Time, const StThreeVector<X> &vec)
	: mThreeVector(vec), mX4(Time) { /* nop */ }

template<class T>
template<class X>
StLorentzVector<T>::StLorentzVector(const StLorentzVector<X> &vec)
	: mThreeVector(vec.vect()), mX4(vec.t()) { /* nop */ }

template<class T>
template<class X>
StLorentzVector<T>
StLorentzVector<T>::boost(const StLorentzVector<X>& pframe) const
{
    T mass               = abs(pframe);
    StThreeVector<T> eta = (-1./mass)*pframe.vect();            // gamma*beta
    T gamma              = fabs(pframe.e())/mass;
    StThreeVector<T> pl  = ((this->vect()*eta)/(eta*eta))*eta;  // longitudinal momentum
    return StLorentzVector<T>(gamma*this->e() - this->vect()*eta,
                              this->vect() + (gamma-1.)*pl - this->e()*eta);
}

template<class T>
template<class X>
void StLorentzVector<T>::setVect(const StThreeVector<X>& v)
{
    mThreeVector = v;
}

template<class T>
template<class X>
StLorentzVector<T>&
StLorentzVector<T>::operator=(const StLorentzVector<X>& vec)
{
    mThreeVector = vec.vect();
    mX4 = vec.t();
    return *this;
}

template<class T>
template<class X>
bool
StLorentzVector<T>::operator== (const StLorentzVector<X>& v) const
{
    return (mThreeVector == v.vect()) && (mX4 == v.t());
}

template<class T>
template<class X>
bool
StLorentzVector<T>::operator!= (const StLorentzVector<X>& v) const
{
    return !(*this == v);
}

template<class T>
template<class X>
StLorentzVector<T>&
StLorentzVector<T>::operator+= (const StLorentzVector<X>& v)
{
    mThreeVector += v.vect();
    mX4 += v.t();
    return *this;
}

template<class T>
template<class X>
StLorentzVector<T>&
StLorentzVector<T>::operator-= (const StLorentzVector<X>& v)
{
    mThreeVector -= v.vect();
    mX4 -= v.t();
    return *this;
}

#else

template<class T>
StLorentzVector<T>::StLorentzVector(const StThreeVector<float> &vec, T t)
	: mThreeVector(vec), mX4(t) { /* nop */ }

template<class T>
StLorentzVector<T>::StLorentzVector(const StThreeVector<double> &vec, T t)
	: mThreeVector(vec), mX4(t) { /* nop */ }

template<class T>
StLorentzVector<T>::StLorentzVector(T t, const StThreeVector<float> &vec)
	: mThreeVector(vec), mX4(t) { /* nop */ }

template<class T>
StLorentzVector<T>::StLorentzVector(T t, const StThreeVector<double> &vec)
	: mThreeVector(vec), mX4(t) { /* nop */ }

template<class T>
StLorentzVector<T>::StLorentzVector(const StLorentzVector<float> &vec)
	: mThreeVector(vec.vect()), mX4(vec.t()) { /* nop */ }
    
template<class T>
StLorentzVector<T>::StLorentzVector(const StLorentzVector<double> &vec)
	: mThreeVector(vec.vect()), mX4(vec.t()) { /* nop */ }
    
template<class T>
StLorentzVector<T>
StLorentzVector<T>::boost(const StLorentzVector<float>& pframe) const
{
    T mass               = abs(pframe);
    StThreeVector<T> eta = (-1./mass)*pframe.vect();            // gamma*beta
    T gamma              = fabs(pframe.e())/mass;
    StThreeVector<T> pl  = ((this->vect()*eta)/(eta*eta))*eta;  // longitudinal momentum
    return StLorentzVector<T>(gamma*this->e() - this->vect()*eta,
                              this->vect() + (gamma-1.)*pl - this->e()*eta);
}

template<class T>
StLorentzVector<T>
StLorentzVector<T>::boost(const StLorentzVector<double>& pframe) const
{
    T mass               = abs(pframe);
    StThreeVector<T> eta = (-1./mass)*pframe.vect();            // gamma*beta
    T gamma              = fabs(pframe.e())/mass;
    StThreeVector<T> pl  = ((this->vect()*eta)/(eta*eta))*eta;  // longitudinal momentum
    return StLorentzVector<T>(gamma*this->e() - this->vect()*eta,
                              this->vect() + (gamma-1.)*pl - this->e()*eta);
}

template<class T>
void StLorentzVector<T>::setVect(const StThreeVector<float>& v)
{
    mThreeVector = v;
}

template<class T>
void StLorentzVector<T>::setVect(const StThreeVector<double>& v)
{
    mThreeVector = v;
}

template<class T>
StLorentzVector<T>&
StLorentzVector<T>::operator=(const StLorentzVector<float>& vec)
{
    mThreeVector = vec.vect();
    mX4 = vec.t();
    return *this;
}

template<class T>
StLorentzVector<T>&
StLorentzVector<T>::operator=(const StLorentzVector<double>& vec)
{
    mThreeVector = vec.vect();
    mX4 = vec.t();
    return *this;
}

template<class T>
bool
StLorentzVector<T>::operator== (const StLorentzVector<float>& v) const
{
    return (this->vect() == v.vect()) && (mX4 == v.t());
}

template<class T>
bool
StLorentzVector<T>::operator== (const StLorentzVector<double>& v) const
{
    return (mThreeVector == v.vect()) && (mX4 == v.t());
}

template<class T>
bool
StLorentzVector<T>::operator!= (const StLorentzVector<float>& v) const
{
    return !(*this == v);
}

template<class T>
bool
StLorentzVector<T>::operator!= (const StLorentzVector<double>& v) const
{
    return !(*this == v);
}

template<class T>
StLorentzVector<T>&
StLorentzVector<T>::operator+= (const StLorentzVector<float>& v)
{
    mThreeVector += v.vect();
    mX4 += v.t();
    return *this;
}

template<class T>
StLorentzVector<T>&
StLorentzVector<T>::operator+= (const StLorentzVector<double>& v)
{
    mThreeVector += v.vect();
    mX4 += v.t();
    return *this;
}

template<class T>
StLorentzVector<T>&
StLorentzVector<T>::operator-= (const StLorentzVector<float>& v)
{
    mThreeVector -= v.vect();
    mX4 -= v.t();
    return *this;
}

template<class T>
StLorentzVector<T>&
StLorentzVector<T>::operator-= (const StLorentzVector<double>& v)
{
    mThreeVector -= v.vect();
    mX4 -= v.t();
    return *this;
}

#endif // ST_NO_MEMBER_TEMPLATES
#endif /* ! __CINT__ */

//
//   Non-member operators
//
template<class T, class X>
StLorentzVector<T>
operator+ (const StLorentzVector<T>& v1, const StLorentzVector<X>& v2)
{
    return StLorentzVector<T>(v1) += v2;
}

template<class T, class X>
StLorentzVector<T>
operator- (const StLorentzVector<T>& v1, const StLorentzVector<X>& v2)
{
    return StLorentzVector<T>(v1) -= v2;
}

template<class T, class X>
T operator* (const StLorentzVector<T>& v1, const StLorentzVector<X>& v2)
{
    return v1.t()*v2.t() - v1.vect()*v2.vect();
}

template<class T>
StLorentzVector<T>
operator* (const StLorentzVector<T>& v, double c)
{
    return StLorentzVector<T>(v) *= c;
}

template<class T>
StLorentzVector<T> operator* (double c, const StLorentzVector<T>& v)
{
    return StLorentzVector<T>(v) *= c;
}

template<class T, class X>
StLorentzVector<T> operator/ (const StLorentzVector<T>& v, X c)
{
    return StLorentzVector<T>(v) /= c;
}

template<class T>
ostream& operator<< (ostream& os, const StLorentzVector<T>& v)
{
    return os << v.vect() << "\t\t" << v.t();
}

template<class T>
istream&  operator>>(istream& is, StLorentzVector<T>& v)
{
    T  x, y, z, t;
    is >> x >> y >> z >> t;
    v.setX(x);
    v.setY(y);
    v.setZ(z);
    v.setT(t);
    return is;
}

//
//        Non-member functions
//
template<class T>
T abs(const StLorentzVector<T>& v) {return v.m();}
#endif
