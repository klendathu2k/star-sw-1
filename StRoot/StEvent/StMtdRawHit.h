/*!
 * \class StMtdRawHit 
 */
/***************************************************************************
 *
 * $Id: StMtdRawHit.h,v 2.1 2011/04/25 21:24:02 ullrich Exp $
 *
 * Author: Frank Geurts, April 25, 2011
***************************************************************************
 *
 * Description: MTD raw hits from daq
 *
 ***************************************************************************
 *
 * $Log: StMtdRawHit.h,v $
 * Revision 2.1  2011/04/25 21:24:02  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StMtdRawHit_hh
#define StMtdRawHit_hh

/// @file StMtdRawHit.h
/// @brief Raw TDC hit from the DAQ stream for the STAR Muon Telescope Detector (MTD).

#include <Stiostream.h>
#include "StObject.h"
#include <cstdlib>

/// @brief Raw TDC hit from the DAQ stream for the STAR Muon Telescope Detector (MTD).
class StMtdRawHit : public StObject {
public:
    /// @brief Default constructor.
    StMtdRawHit();
    
    /// @brief Constructs with flag, backleg, channel, and TDC count.
    StMtdRawHit(char, unsigned char, unsigned char, unsigned int);
    /// @brief Destructor.
    ~StMtdRawHit();    
    
    /// @brief Equality comparison operator.
    int operator==(const StMtdRawHit&) const;
    /// @brief Inequality comparison operator.
    int operator!=(const StMtdRawHit&) const;
    
    /// @brief Returns true if this is a leading-edge hit.
    bool      leadingEdge() const;
    /// @brief Returns true if this is a trailing-edge hit.
    bool      trailingEdge() const;
    /// @brief Returns the fiber ID (0-based) derived from the flag.
    int       fiberId() const;
    /// @brief Returns the raw flag byte (positive=leading, negative=trailing; |flag|−1 = fiberId).
    int       flag() const;
    /// @brief Returns the backleg number.
    int       backleg() const;
    /// @brief Returns the TDC channel number.
    int       channel() const;
    /// @brief Returns the raw TDC count.
    unsigned int   tdc() const;
    
    /// @brief Sets the raw flag byte.
    void      setFlag(char);
    /// @brief Sets the backleg number.
    void      setBackleg(unsigned char);
    /// @brief Sets the TDC channel number.
    void      setChannel(unsigned char);
    /// @brief Sets the raw TDC count.
    void      setTdc(unsigned int);
    
protected:
    Char_t   mFlag;     ///< Edge flag: positive=leading, negative=trailing; |flag|−1 = fiberId
    UChar_t  mBackLeg;  ///< Backleg number
    UChar_t  mChannel;  ///< TDC channel number
    UInt_t   mTdc;      ///< Raw TDC count
    
    ClassDef(StMtdRawHit,1)
};

ostream& operator<<(ostream&, const StMtdRawHit&); // Printing operator
inline void StMtdRawHit::setFlag(char iflag) { mFlag = iflag;}
inline void StMtdRawHit::setBackleg(unsigned char ibackleg) { mBackLeg = ibackleg;}
inline void StMtdRawHit::setChannel(unsigned char ichannel){ mChannel = ichannel;}
inline void StMtdRawHit::setTdc(unsigned int rawTdc){ mTdc = rawTdc;}
inline bool StMtdRawHit::leadingEdge() const { return (mFlag>0);}
inline bool StMtdRawHit::trailingEdge() const{ return (mFlag<0);}
inline  int StMtdRawHit::fiberId() const { return abs(static_cast<int>(mFlag)) - 1;}
inline  int StMtdRawHit::flag() const {return mFlag;}
inline  int StMtdRawHit::backleg()  const {return mBackLeg;}
inline  int StMtdRawHit::channel()  const{return mChannel;}
inline unsigned int StMtdRawHit::tdc()  const { return mTdc;}

#endif
