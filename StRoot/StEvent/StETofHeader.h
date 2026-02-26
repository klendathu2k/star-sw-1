/***************************************************************************
 *
 * $Id: StETofHeader.h,v 2.3 2021/03/19 19:56:50 ullrich Exp $
 *
 * Author: Pengfei Lyu, April 2018
 ***************************************************************************
 *
 * Description: This class stores ETofHeader information from the DAQ stream,
 * e.g. trigger time and STAR time (reset time stamp of the bTOF clock)
 * and other event-wise information
 *
 ***************************************************************************
 *
 * $Log: StETofHeader.h,v $
 * Revision 2.3  2021/03/19 19:56:50  ullrich
 * include the front-end missmatch pattern (Philipp)
 *
 * Revision 2.2  2019/08/01 22:52:19  smirnovd
 * Add non-c++ include defining uint64_t for rootcint
 *
 * rootcint is not c++11 aware and therefore cannot deal with the standard c++
 * header <cstdint>
 *
 * Revision 2.1  2018/07/09 14:53:48  ullrich
 * Initial Revision.
 *
 *
 ***************************************************************************/
#ifndef STETOFHEADER_H
#define STETOFHEADER_H

/// @file StETofHeader.h
/// @brief Event-wise header data for the STAR Endcap Time of Flight (eTOF) detector, including trigger timestamps.

#include <stdint.h>
#include <map>
#include <vector>


#include <Stiostream.h>
#include "StObject.h"

/// @brief Event-wise header data for the STAR Endcap TOF (eTOF) detector, including trigger timestamps.
class StETofHeader : public StObject {
public:

    /// @brief Default constructor.
    StETofHeader();
    /** 
     ** @brief default constructor for pre-2020 data. No missmatch information available. Used in StEtofDigiMaker to initialise the header.
     **/
    StETofHeader( const double&, const double&, const map< unsigned int, uint64_t >&, const map< unsigned int, uint64_t >& ,
                  const unsigned int&, const unsigned int&, const unsigned int&, const uint64_t& );
    /** 
     ** @brief default constructor for post-2020 data. Include missmatch information from FEE. Used in StEtofDigiMaker to initialise the header.
     **/              
    StETofHeader( const double&, const double&, const map< unsigned int, uint64_t >&, const map< unsigned int, uint64_t >& ,
                  const unsigned int&, const unsigned int&, const unsigned int&, const uint64_t&, const std::vector<bool>&  );
    /** 
     ** @brief Full constructor including goodEventFlag, which is normally set in calibrations only.
     **/                
    StETofHeader( const double&, const double&, const map< unsigned int, uint64_t >&, const map< unsigned int, uint64_t >& ,
                  const unsigned int&, const unsigned int&, const unsigned int&, const uint64_t&, const std::vector<bool>&, const std::vector<bool>& , const std::vector<bool>& );              

    /// @brief Destructor.
    ~StETofHeader();

    /// @brief Returns the full gDPB trigger timestamp [ns].
    double    trgGdpbFullTime()   const;
    /// @brief Returns the full STAR trigger timestamp [ns].
    double    trgStarFullTime()   const;

    /// @brief Returns the per-ROC gDPB timestamp map.
    map< unsigned int, uint64_t > rocGdpbTs()  const;
    /// @brief Returns the per-ROC STAR timestamp map.
    map< unsigned int, uint64_t > rocStarTs()  const;

    /// @brief Returns the STAR trigger token.
    unsigned int      starToken()         const;
    /// @brief Returns the STAR DAQ command input word.
    unsigned int      starDaqCmdIn()      const;
    /// @brief Returns the STAR trigger command input word.
    unsigned int      starTrgCmdIn()      const;
    /// @brief Returns the event status flag bitmask.
    uint64_t          eventStatusFlag()   const;    
    /** 
     ** @brief Flag for each Get4 TDC to mark if it is available in this event.
     **/     
    std::vector<bool>       missMatchFlagVec()  const;
    /** 
     ** @brief Flag to mark if the event is good for physics analysis for each counter. A counter is considered good in each event when there are zero missmatch flags set and pulser digis on both sides are found. In this case, the counter should perform at its best. Counter efficiency should be constant between good events. 
     **/      
    std::vector<bool>       goodEventFlagVec()  const;

    /// @brief Returns the per-counter flag indicating whether pulser digis were found on both sides.
    std::vector<bool>       hasPulsersVec()  const;


    /// @brief Sets the full gDPB trigger timestamp [ns].
    void    setTrgGdpbFullTime( const double& gdpbFullTime );
    /// @brief Sets the full STAR trigger timestamp [ns].
    void    setTrgStarFullTime( const double& starFullTime );

    /// @brief Sets the per-ROC gDPB timestamp map.
    void    setRocGdpbTs( const map< unsigned int, uint64_t >& gdpbTs );
    /// @brief Sets the per-ROC STAR timestamp map.
    void    setRocStarTs( const map< unsigned int, uint64_t >& starTs );

    /// @brief Sets the STAR trigger token.
    void    setStarToken(    const unsigned int& token    );
    /// @brief Sets the STAR DAQ command input word.
    void    setStarDaqCmdIn( const unsigned int& daqCmdIn );
    /// @brief Sets the STAR trigger command input word.
    void    setStarTrgCmdIn( const unsigned int& trgCmdIn );

    /// @brief Sets the event status flag bitmask.
    void    setEventStatusFlag( const uint64_t& statusFlag );
    /// @brief Sets the per-counter good-event flag vector.
    void    setGoodEventFlagVec( const std::vector<bool>& FlagVec );
    /// @brief No-op overload for backward compatibility.
    void    setGoodEventFlagVec( int blubb ) {return;}
    /// @brief Sets the per-counter has-pulsers flag vector.
    void    setHasPulsersVec( const std::vector<bool>& PulserVec );

private:
    Double_t    mTrgGdpbFullTime;   ///< Full gDPB trigger timestamp [ns]
    Double_t    mTrgStarFullTime;   ///< Full STAR trigger timestamp [ns]
    
    map< UInt_t, ULong64_t > mRocGdpbTs;   ///< Per-ROC gDPB timestamps
    map< UInt_t, ULong64_t > mRocStarTs;   ///< Per-ROC STAR timestamps
    
    UInt_t      mStarToken;      ///< STAR trigger token
    UInt_t      mStarDaqCmdIn;   ///< STAR DAQ command input word
    UInt_t      mStarTrgCmdIn;   ///< STAR trigger command input word
    
    ULong64_t   mEventStatusFlag;  ///< Event status flag bitmask

    std::vector< Bool_t > mMissMatchFlagVec;   ///< Per-Get4 mismatch flag (true=mismatch)
    std::vector< Bool_t > mGoodEventFlagVec;   ///< Per-counter good-event flag
    std::vector< Bool_t > mHasPulsersVec;      ///< Per-counter has-pulsers flag

    ClassDef( StETofHeader, 4 )
};

#endif // STETOFHEADER_H
