/*!
 * \class StRichPixel 
 * \author Thomas Ullrich, Aug 1999
 */
/***************************************************************************
 *
 * $Id: StRichPixel.h,v 2.6 2002/02/22 22:56:50 jeromel Exp $
 *
 * Author: Thomas Ullrich, Aug 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StRichPixel.h,v $
 * Revision 2.6  2002/02/22 22:56:50  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.5  2001/04/05 04:00:41  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.4  2001/02/07 16:04:05  lasiuk
 * check the 11th bit for overflow.
 * Overflow is 1024
 *
 * Revision 2.3  2000/01/13 21:06:22  lasiuk
 * add rich pixel info/containers
 *
 * Revision 2.2  2000/01/10 17:12:21  lasiuk
 * remove dst_rch_pixel dependency;
 * change stored data to a single long;
 * modify unpacking routines;
 *
 * Revision 2.1  1999/10/13 19:43:36  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRichPixel_hh
#define StRichPixel_hh

/// @file StRichPixel.h
/// @brief Raw RICH detector pixel, storing pad address and ADC amplitude in a packed 32-bit word.

#include "StObject.h"
#include "StEnumerations.h"

/// @brief Raw RICH pad pixel: pad address (row, column) and ADC value packed into a single 32-bit word.
class StRichPixel : public StObject {
public:
    /// @brief Default constructor.
    StRichPixel();
    /// @brief Constructor from a packed 32-bit data word.
    StRichPixel(unsigned int packedData);
    // StRichPixel(const StRichPixel&);            use default
    // StRichPixel& operator=(const StRichPixel&); use default
    ~StRichPixel();

    int operator==(const StRichPixel&) const;
    int operator!=(const StRichPixel&) const;

    /// @brief Sets the packed data word containing pad address and ADC value.
    void            setPackedData(unsigned int);

    /// @brief Returns the pad (column) number, decoded from bits 7:0 of the packed word.
    unsigned short  pad() const;
    /// @brief Returns the row number, decoded from bits 15:8 of the packed word.
    unsigned short  row() const;
    /// @brief Returns the ADC value from bits 25:16; returns 1024 if the overflow bit (bit 26) is set.
    unsigned short  adc() const;

protected:
    UInt_t  mPackedData;  ///< Packed 32-bit word: bits[7:0]=pad, bits[15:8]=row, bits[26:16]=ADC+overflow

    ClassDef(StRichPixel,1)
};

inline void
StRichPixel::setPackedData(unsigned int pixel)
{
    mPackedData = pixel;
}

inline unsigned short
StRichPixel::pad() const
{
    return (mPackedData & 0xff);  // first 8 bits
}

inline unsigned short
StRichPixel::row() const
{
    return ( (mPackedData>>8) & 0xff);  // second 8 bits
}

inline unsigned short
StRichPixel::adc()  const
{
    //
    // 11 bits are stored.  The 11th bit is the overflow
    // if the 11th bit is set, return saturated
    // otherwise, return the 10 bit value
    //return ( (mPackedData>>16) & 0x3ff); // 10bits
    
    return ( ( (mPackedData>>26) & 0x1) ? 1024 : ( (mPackedData>>16) & 0x3ff) );

}

#endif
