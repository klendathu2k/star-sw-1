/*!
 * \class StTofMCSlat 
 * \author Wei-Ming Zhang, April 2001 
 */
/***************************************************************************
 *
 * $Id: StTofMCSlat.h,v 2.5 2003/05/21 18:23:18 ullrich Exp $
 *
 * Author: Wei-Ming Zhang, April 2001 
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTofMCSlat.h,v $
 * Revision 2.5  2003/05/21 18:23:18  ullrich
 * Major Revision of ToF classes (F. Geurts)
 *
 * Revision 2.4  2002/02/22 22:56:52  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/27 21:43:18  ullrich
 * Moved MC info class into separate file.
 *
 * Revision 2.2  2001/04/26 15:19:10  ullrich
 * Added ClassDef and ClassImp macros.
 *
 * Revision 2.1  2001/04/26 01:07:42  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTofMCSlat_hh
#define StTofMCSlat_hh

/// @file StTofMCSlat.h
/// @brief Monte Carlo TOF slat extending StTofSlat with simulation truth information.

#include "StTofSlat.h"
#include "StTofMCInfo.h"

/// @brief Monte Carlo TOF slat extending StTofSlat with simulation truth information.
class StTofMCSlat : public StTofSlat {
public:
    /// @brief Default constructor.
    StTofMCSlat();
    /// @brief Constructs from a Monte Carlo info object.
    StTofMCSlat(const StTofMCInfo&);
    /// @brief Destructor.
    ~StTofMCSlat();
    
    /// @brief Equality comparison operator.
    int operator==(const StTofMCSlat&) const;
    /// @brief Inequality comparison operator.
    int operator!=(const StTofMCSlat&) const;

    /// @brief Returns the Monte Carlo truth information for this slat.
    const StTofMCInfo&  mcInfo() const;

    /// @brief Sets the Monte Carlo truth information for this slat.
    void                setMCInfo(const StTofMCInfo&);

    /// @brief Sets the number of Monte Carlo hits in this slat.
    void                setNHits(int nHits);
    /// @brief Sets the number of photoelectrons produced.
    void                setNPhe(int nPhe);
    /// @brief Sets the energy deposit [GeV].
    void                setDe(float de);
    /// @brief Sets the step length of the Monte Carlo track [cm].
    void                setDs(float ds);
    /// @brief Sets the Monte Carlo time of flight [ns].
    void                setTof(float tof);

protected:
    StTofMCInfo  mTofMCInfo;  ///< Monte Carlo truth information for this slat

    ClassDef(StTofMCSlat,1)
};

inline const StTofMCInfo&
StTofMCSlat::mcInfo() const
{
    return mTofMCInfo;
}

inline void
StTofMCSlat::setMCInfo(const StTofMCInfo& MCInfo)
{
    mTofMCInfo = MCInfo;
}

inline void
StTofMCSlat::setNHits(int nHits)
{
    mTofMCInfo.mNHits = nHits;
}

inline void
StTofMCSlat::setNPhe(int nPhe)
{
    mTofMCInfo.mNPhe = nPhe;
}

inline void
StTofMCSlat::setDe(float de)
{
    mTofMCInfo.mDe = de;
}


inline void
StTofMCSlat::setDs(float ds)
{
    mTofMCInfo.mDs = ds;
}


inline void
StTofMCSlat::setTof(float tof)
{
    mTofMCInfo.mTof = tof;
}

ostream& operator<<(ostream& os, const StTofMCSlat&);
#endif
