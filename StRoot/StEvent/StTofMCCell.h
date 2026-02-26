/***************************************************************************
 *
 * $Id: StTofMCCell.h,v 2.1 2003/05/21 18:24:20 ullrich Exp $
 *
 * Author: F. Geurts, May 2003
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTofMCCell.h,v $
 * Revision 2.1  2003/05/21 18:24:20  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTofMCCell_hh
#define StTofMCCell_hh

/// @file StTofMCCell.h
/// @brief Monte Carlo TOF cell extending StTofCell with simulation truth information.

#include "StTofCell.h"
#include "StTofMCInfo.h"

/// @brief Monte Carlo TOF cell extending StTofCell with simulation truth information.
class StTofMCCell : public StTofCell {
public:
    /// @brief Default constructor.
    StTofMCCell();
    /// @brief Constructs from a Monte Carlo info object.
    StTofMCCell(const StTofMCInfo&);
    /// @brief Destructor.
    ~StTofMCCell();
    
    /// @brief Equality comparison operator.
    int operator==(const StTofMCCell&) const;
    /// @brief Inequality comparison operator.
    int operator!=(const StTofMCCell&) const;

    /// @brief Returns the Monte Carlo truth information for this cell.
    const StTofMCInfo&  mcInfo() const;

    /// @brief Sets the Monte Carlo truth information for this cell.
    void  setMCInfo(const StTofMCInfo&);

    /// @brief Sets the number of Monte Carlo hits in this cell.
    void  setNHits(int nHits);
    /// @brief Sets the number of photoelectrons produced.
    void  setNPhe(int nPhe);
    /// @brief Sets the energy deposit [GeV].
    void  setDe(float de);
    /// @brief Sets the step length of the Monte Carlo track [cm].
    void  setDs(float ds);
    /// @brief Sets the Monte Carlo time of flight [ns].
    void  setTof(float tof);

protected:
    StTofMCInfo  mTofMCInfo;  ///< Monte Carlo truth information for this cell

    ClassDef(StTofMCCell,1)
};

inline const StTofMCInfo&
StTofMCCell::mcInfo() const
{
    return mTofMCInfo;
}

inline void
StTofMCCell::setMCInfo(const StTofMCInfo& MCInfo)
{
    mTofMCInfo = MCInfo;
}

inline void
StTofMCCell::setNHits(int nHits)
{
    mTofMCInfo.mNHits = nHits;
}

inline void
StTofMCCell::setNPhe(int nPhe)
{
    mTofMCInfo.mNPhe = nPhe;
}

inline void
StTofMCCell::setDe(float de)
{
    mTofMCInfo.mDe = de;
}


inline void
StTofMCCell::setDs(float ds)
{
    mTofMCInfo.mDs = ds;
}


inline void
StTofMCCell::setTof(float tof)
{
    mTofMCInfo.mTof = tof;
}

ostream& operator<<(ostream& os, const StTofMCCell&);

#endif
