/*!
 * \class StTofRawData 
 * \author Xin Dong, Feb 2005
 */
/***************************************************************************
 *
 * $Id: StTofRawData.h,v 2.3 2008/02/26 20:01:58 ullrich Exp $
 *
 * Author: Xin Dong, Feb 2005
 ***************************************************************************
 *
 * Description: TOF raw hits (from Run 5)
 *
 ***************************************************************************
 *
 * $Log: StTofRawData.h,v $
 * Revision 2.3  2008/02/26 20:01:58  ullrich
 * Added new member mTriggerrime and related methods.
 *
 * Revision 2.2  2007/11/21 00:28:32  ullrich
 * Added new data member mTray plus access functions. New overloaded constructor.
 *
 * Revision 2.1  2005/04/11 22:35:25  calderon
 * Tof Classes for Run 5.  Modifications and additions from Xin to
 * take care of new TOF daq and electronics.  Added StTofRawData and
 * modified containers and includes.
 *
 *
 **************************************************************************/
#ifndef StTofRawData_hh
#define StTofRawData_hh

/// @file StTofRawData.h
/// @brief Raw TDC hit record from the STAR legacy Time of Flight detector (Run 5+).

#include "StObject.h"

/// @brief Raw TDC hit record from the STAR legacy TOF detector electronics (Run 5+).
class StTofRawData : public StObject {
public:
    /// @brief Default constructor.
    StTofRawData();

    /// @brief Constructs without tray information.
    StTofRawData(unsigned short, unsigned short, unsigned int, unsigned short);
    /// @brief Constructs with tray, channel, TDC, and quality fields.
    StTofRawData(unsigned short, unsigned short, unsigned short, unsigned int, unsigned short);
    /// @brief Constructs with tray, channel, TDC, trigger time, and quality fields.
    StTofRawData(unsigned short, unsigned short, unsigned short, unsigned int, unsigned int, unsigned short);
    /// @brief Destructor.
    ~StTofRawData();    

    /// @brief Equality comparison operator.
    int operator==(const StTofRawData&) const;
    /// @brief Inequality comparison operator.
    int operator!=(const StTofRawData&) const;

    unsigned short  leteFlag() const; ///< Returns the leading/trailing edge flag (1=leading, 2=trailing).
    /// @brief Returns the tray number.
    unsigned short  tray() const;
    /// @brief Returns the TDC channel number.
    unsigned short  channel() const;
    /// @brief Returns the raw TDC count.
    unsigned int    tdc() const;
    /// @brief Returns the trigger time TDC count.
    unsigned int    triggertime() const;
    /// @brief Returns the data quality flag.
    unsigned short  quality() const;

    /// @brief Sets the leading/trailing edge flag.
    void      setLeTeFlag(unsigned short);
    /// @brief Sets the tray number.
    void      setTray(unsigned short);
    /// @brief Sets the TDC channel number.
    void      setChannel(unsigned short);
    /// @brief Sets the raw TDC count.
    void      setTdc(unsigned int);
    /// @brief Sets the trigger time TDC count.
    void      setTriggertime(unsigned int);
    /// @brief Sets the data quality flag.
    void      setQuality(unsigned short);
    
protected:
    UShort_t  mLeTeFlag;     ///< Leading/trailing edge flag (1=leading, 2=trailing)
    UShort_t  mTray;         ///< Tray number
    UShort_t  mChannel;      ///< TDC channel number
    UInt_t    mTdc;          ///< Raw TDC count
    UInt_t    mTriggertime;  ///< Trigger time TDC count
    UShort_t  mQuality;      ///< Data quality flag

    ClassDef(StTofRawData,3)
};

inline void
StTofRawData::setLeTeFlag(unsigned short iflag)
{
    mLeTeFlag = iflag;
}

inline void
StTofRawData::setTray(unsigned short itray)
{
    mTray = itray;
}

inline void
StTofRawData::setChannel(unsigned short ichannel)
{
    mChannel = ichannel;
}

inline void
StTofRawData::setTdc(unsigned int rawTdc)
{
    mTdc = rawTdc;
}

inline void
StTofRawData::setTriggertime(unsigned int rawtriggertime)
{
    mTriggertime = rawtriggertime;
}

inline void
StTofRawData::setQuality(unsigned short quality)
{
    mQuality = quality;
}

inline unsigned short
StTofRawData::leteFlag() const
{
    return mLeTeFlag;
}

inline unsigned short
StTofRawData::tray()  const
{
    return mTray;
}

inline unsigned short
StTofRawData::channel()  const
{
    return mChannel;
}

inline unsigned int
StTofRawData::tdc()  const
{
    return mTdc;
}

inline unsigned int
StTofRawData::triggertime()  const
{
    return mTriggertime;
}

inline unsigned short
StTofRawData::quality()  const
{
    return mQuality;
}

#endif
