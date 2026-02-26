/*!
 * \class StTofData 
 * \author W.J. Llope, Sep 2001
 */
/***************************************************************************
 *
 * $Id: StTofData.h,v 2.4 2005/04/11 22:35:25 calderon Exp $
 *
 * Author: W.J. Llope, Sep 2001
 ***************************************************************************
 *
 * Description: TOFp Systems raw data (TOFp+pVPD)
 *
 ***************************************************************************
 *
 * $Log: StTofData.h,v $
 * Revision 2.4  2005/04/11 22:35:25  calderon
 * Tof Classes for Run 5.  Modifications and additions from Xin to
 * take care of new TOF daq and electronics.  Added StTofRawData and
 * modified containers and includes.
 *
 * Revision 2.3  2003/05/21 18:22:46  ullrich
 * Major Revision of ToF classes (F. Geurts)
 *
 * Revision 2.2  2002/02/22 22:56:51  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.1  2001/10/01 19:39:52  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTofData_hh
#define StTofData_hh

/// @file StTofData.h
/// @brief Raw electronic data from the legacy TOFp detector (TOFp + pVPD).

#include "StObject.h"

/// @brief Raw electronic data from a single channel of the legacy TOFp detector system.
class StTofData : public StObject {
public:
    /// @brief Default constructor.
    StTofData();
    /// @brief Constructs with all raw data fields.
    StTofData(unsigned short, unsigned short, unsigned short, short, unsigned short, unsigned int, unsigned int);
    /// @brief Destructor.
    ~StTofData();    

    /// @brief Equality comparison operator.
    int operator==(const StTofData&) const;
    /// @brief Inequality comparison operator.
    int operator!=(const StTofData&) const;

    /// @brief Returns the data channel index.
    unsigned short  dataIndex() const;
    /// @brief Returns the raw ADC value.
    unsigned short  adc() const;
    /// @brief Returns the raw TDC value.
    unsigned short  tdc() const;
    /// @brief Returns the time calibration (TC) value.
             short  tc() const;
    /// @brief Returns the slewing correction (SC) value.
    unsigned short  sc() const;

    /// @brief Returns the leading-edge TDC count.
    unsigned int    leadingTdc() const;
    /// @brief Returns the trailing-edge TDC count.
    unsigned int    trailingTdc() const;
 
    /// @brief Sets the data channel index.
    void      setDataIndex(unsigned short);
    /// @brief Sets the raw ADC value.
    void      setAdc(unsigned short);
    /// @brief Sets the raw TDC value.
    void      setTdc(unsigned short);
    /// @brief Sets the time calibration (TC) value.
    void      setTc(short);
    /// @brief Sets the slewing correction (SC) value.
    void      setSc(unsigned short);

    /// @brief Sets the leading-edge TDC count.
    void      setLeadingTdc(unsigned int);
    /// @brief Sets the trailing-edge TDC count.
    void      setTrailingTdc(unsigned int);

protected:
    UShort_t  mDataIndex;  ///< Data channel index
    UShort_t  mAdc;        ///< Raw ADC value
    UShort_t  mTdc;        ///< Raw TDC value
    Short_t   mTc;         ///< Time calibration value
    UShort_t  mSc;         ///< Slewing correction value

    UInt_t    mLeadingTdc;   ///< Leading-edge TDC count
    UInt_t    mTrailingTdc;  ///< Trailing-edge TDC count

    ClassDef(StTofData,3)
};

inline void
StTofData::setDataIndex(unsigned short dataId)
{
    mDataIndex = dataId;
}

inline void
StTofData::setAdc(unsigned short rawAdc)
{
    mAdc = rawAdc;
}

inline void
StTofData::setTdc(unsigned short rawTdc)
{
    mTdc = rawTdc;
}

inline void
StTofData::setTc(short rawTc)
{
    mTc = rawTc;
}

inline void
StTofData::setSc(unsigned short rawSc)
{
    mSc = rawSc;
}

inline void
StTofData::setLeadingTdc(unsigned int rawLTdc)
{
  mLeadingTdc = rawLTdc;
}

inline void
StTofData::setTrailingTdc(unsigned int rawTTdc)
{
  mTrailingTdc = rawTTdc;
}


inline unsigned short
StTofData::dataIndex() const
{
    return mDataIndex;
}

inline unsigned short
StTofData::adc()  const
{
    return mAdc;
}

inline unsigned short
StTofData::tdc()  const
{
    return mTdc;
}

inline short
StTofData::tc()  const
{
    return mTc;
}

inline unsigned short
StTofData::sc()  const
{
    return mSc;
}

inline unsigned int
StTofData::leadingTdc() const
{
  return mLeadingTdc;
}

inline unsigned int
StTofData::trailingTdc() const
{
  return mTrailingTdc;
}

ostream& operator<<(ostream& os, const StTofData&);

#endif
