/*!
 * \class StBTofRawHit 
 * \author Xin Dong, Nov 2008
 */
/***************************************************************************
 *
 * $Id: StBTofRawHit.h,v 2.3 2009/08/25 15:41:28 fine Exp $
 *
 * Author: Xin Dong, Nov 2008
 ***************************************************************************
 *
 * Description: TOF raw hits from daq
 *
 ***************************************************************************
 *
 * $Log: StBTofRawHit.h,v $
 * Revision 2.3  2009/08/25 15:41:28  fine
 * fix the compilation issues under SL5_64_bits  gcc 4.3.2
 *
 * Revision 2.2  2009/01/15 00:48:10  ullrich
 * mLeTeFlag changed to mFlag, tray(), module(), cell() now return int.
 *
 * Revision 2.1  2008/12/22 20:31:01  ullrich
 * Initial Revision.
 *
 *
 **************************************************************************/
#ifndef StBTofRawHit_hh
#define StBTofRawHit_hh

/// @file StBTofRawHit.h
/// @brief Raw TDC hit from the DAQ stream for the STAR Barrel Time of Flight (BTOF) detector.

#include <Stiostream.h>
#include "StObject.h"
#include "TMath.h"

/// @brief Raw TDC hit from the DAQ stream for the STAR Barrel Time of Flight (BTOF) detector.
class StBTofRawHit : public StObject {
public:
    /// @brief Default constructor.
    StBTofRawHit();

    /// @brief Constructs with flag, tray, channel, and TDC count.
    StBTofRawHit(char, unsigned char, unsigned char, unsigned int);
    /// @brief Destructor.
    ~StBTofRawHit();    

    /// @brief Equality comparison operator.
    int operator==(const StBTofRawHit&) const;
    /// @brief Inequality comparison operator.
    int operator!=(const StBTofRawHit&) const;

    /// @brief Returns true if this is a leading-edge hit.
    bool      leadingEdge() const;
    /// @brief Returns true if this is a trailing-edge hit.
    bool      trailingEdge() const;
    /// @brief Returns the fiber ID (0–3) derived from the flag.
    int       fiberId() const;
    /// @brief Returns the raw flag byte (positive=leading, negative=trailing; |flag|−1 = fiberId).
    int       flag() const;
    /// @brief Returns the tray number.
    int       tray() const;
    /// @brief Returns the TDC channel number.
    int       channel() const;
    /// @brief Returns the raw TDC count.
    unsigned int   tdc() const;

    /// @brief Sets the raw flag byte.
    void      setFlag(char);
    /// @brief Sets the tray number.
    void      setTray(unsigned char);
    /// @brief Sets the TDC channel number.
    void      setChannel(unsigned char);
    /// @brief Sets the raw TDC count.
    void      setTdc(unsigned int);
    
protected:
    Char_t   mFlag;     ///< Edge flag: positive=leading, negative=trailing; |flag|−1 = fiberId
    UChar_t  mTray;     ///< Tray number
    UChar_t  mChannel;  ///< TDC channel number
    UInt_t   mTdc;      ///< Raw TDC count

    ClassDef(StBTofRawHit,1)
};

ostream& operator<<(ostream&, const StBTofRawHit&); // Printing operator

inline void
StBTofRawHit::setFlag(char iflag)
{
    mFlag = iflag;
}

inline void
StBTofRawHit::setTray(unsigned char itray)
{
    mTray = itray;
}

inline void
StBTofRawHit::setChannel(unsigned char ichannel)
{
    mChannel = ichannel;
}

inline void
StBTofRawHit::setTdc(unsigned int rawTdc)
{
    mTdc = rawTdc;
}

inline bool
StBTofRawHit::leadingEdge() const
{
    return (mFlag>0);
}

inline bool
StBTofRawHit::trailingEdge() const
{
    return (mFlag<0);
}

inline int
StBTofRawHit::fiberId() const
{
    return TMath::Abs(mFlag) - 1;   //! fiber Id = 0, 1, 2, 3
}

inline int
StBTofRawHit::flag() const
{
    return mFlag;
}

inline int
StBTofRawHit::tray()  const
{
    return mTray;
}

inline int
StBTofRawHit::channel()  const
{
    return mChannel;
}

inline unsigned int
StBTofRawHit::tdc()  const
{
    return mTdc;
}

#endif
