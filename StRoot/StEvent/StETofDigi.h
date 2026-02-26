/***************************************************************************
 *
 * $Id: StETofDigi.h,v 2.3 2019/02/11 18:52:38 ullrich Exp $
 *
 * Author: Philipp Weidenkaff, April 2018
 ***************************************************************************
 *
 * Description: Data class for expanded digital eTOF information:
 * eTOF digis capture the electronic response of each side of the MRPC
 * counter read-out
 *
 ***************************************************************************
 *
 * $Log: StETofDigi.h,v $
 * Revision 2.3  2019/02/11 18:52:38  ullrich
 * Added an additional access functions to get the associated hit.
 *
 * Revision 2.2  2018/07/13 14:55:09  ullrich
 * Added getter function for the associated hit (Florian)
 *
 * Revision 2.1  2018/07/09 14:53:48  ullrich
 * Initial Revision.
 *
 *
 ***************************************************************************/ 
#ifndef STETOFDIGI_H
#define STETOFDIGI_H

/// @file StETofDigi.h
/// @brief Electronic digitisation from a single MRPC strip-end in the STAR Endcap TOF (eTOF) detector.

#include <Stiostream.h>
#include "StObject.h"

class StETofHit;

/// @brief Electronic digitisation from a single MRPC strip-end in the STAR Endcap TOF (eTOF) detector.
class StETofDigi : public StObject {
public:
    /// @brief Default constructor.
    StETofDigi();

    /// @brief Constructs from detector geometry address and raw time/ToT.
    StETofDigi( const unsigned int sector, const unsigned int zPlane, const unsigned int counter,
                const unsigned int strip, const unsigned int side,
                const double& time, const double& tot );

    /// @brief Constructs from hardware (ROC/Get4/channel) address and raw time/ToT.
    StETofDigi( const unsigned int rocId, const unsigned int get4Id, const unsigned int elChan,   
                const double& time, const double& tot );

    /// @brief Copy constructor.
    StETofDigi( const StETofDigi& );

    /// @brief Destructor.
    ~StETofDigi();

    /// @brief Returns the uncalibrated raw time [ns].
    double rawTime()          const;

    /// @brief Returns the calibrated time [ns].
    double calibTime()        const;

    /// @brief Alias for rawTot(); returns the uncalibrated time-over-threshold [ns].
    double rawCharge()        const;
    /// @brief Returns the uncalibrated time-over-threshold [ns].
    double rawTot()           const; 
    /// @brief Alias for calibTot(); returns the calibrated time-over-threshold [ns].
    double calibCharge()      const;
    /// @brief Returns the calibrated time-over-threshold [ns].
    double calibTot()         const;

    /// @brief Returns the STAR sector number.
    unsigned int sector()     const;
    /// @brief Returns the eTOF z-plane number.
    unsigned int zPlane()     const;
    /// @brief Returns the counter (MRPC module) number.
    unsigned int counter()    const;
    /// @brief Returns the strip number within the counter.
    unsigned int strip()      const;
    /// @brief Alias for strip(); returns the strip number.
    unsigned int chan()       const;
    /// @brief Returns the readout side (1=top, 2=bottom).
    unsigned int side()       const;

    /// @brief Returns the electronic channel number within the Get4 chip.
    unsigned int elChan()     const;
    /// @brief Returns the Get4 TDC chip ID.
    unsigned int get4Id()     const;
    /// @brief Returns the Readout Controller (ROC) ID.
    unsigned int rocId()      const;

    /// @brief Returns a pointer to the reconstructed hit associated with this digi.
    StETofHit* associatedHit();
    /// @brief Returns a const pointer to the reconstructed hit associated with this digi.
    StETofHit* associatedHit() const;

    /// @brief Less-than operator; sorts by time within the same reference frame.
    bool operator < ( const StETofDigi& rhs ) const; //ordering operator

    /// @brief Compares this digi with another StObject by time.
    int compare( const StObject*    obj  )    const;
    /// @brief Compares this digi with another StETofDigi by time.
    int compare( const StETofDigi*  digi )    const;
    
    /// @brief Sets the geometry address (sector, z-plane, counter, channel, side).
    void setGeoAddress( const unsigned int iSector, const unsigned int iZPlane, const unsigned int iCounter,
                        const unsigned int iChannel, const unsigned int iSide );
    /// @brief Sets the hardware address (ROC ID, Get4 ID, electronic channel).
    void setHwAddress( const unsigned int iRocId, const unsigned int iGet4Id, const unsigned int iElChan );

    /// @brief Sets the raw time [ns].
    void setRawTime(   const double& time );  //ns
    /// @brief Sets the raw time-over-threshold [ns].
    void setRawTot(    const double& tot  );  //ns
    /// @brief Sets the calibrated time [ns].
    void setCalibTime( const double& time );
    /// @brief Sets the calibrated time-over-threshold [ns].
    void setCalibTot(  const double& tot  );
    
    /// @brief Sets the pointer to the associated reconstructed hit.
    void setAssociatedHit( StETofHit* hit );


private:
    UInt_t      mSector;   ///< STAR sector number
    UInt_t      mZPlane;   ///< eTOF z-plane number
    UInt_t      mCounter;  ///< Counter (MRPC module) number
    UInt_t      mStrip;    ///< Strip number within the counter
    UInt_t      mSide;     ///< Readout side (1=top, 2=bottom)

    UInt_t      mRocId;    ///< Readout Controller (ROC) ID
    UInt_t      mGet4Id;   ///< Get4 TDC chip ID
    UInt_t      mElChan;   ///< Electronic channel number within the Get4 chip

    Double_t    mRawTime;    ///< Uncalibrated raw time [ns]
    Double_t    mCalibTime;  ///< Calibrated time [ns]
    Double_t    mRawTot;     ///< Uncalibrated time-over-threshold [ns]
    Double_t    mCalibTot;   ///< Calibrated time-over-threshold [ns]

    StETofHit*  mAssociatedHit; //$LINK


    ClassDef( StETofDigi, 1 );
};

ostream& operator << ( ostream&, const StETofDigi& digi ); // Printing operator


inline double StETofDigi::rawTime()      const { return mRawTime;   };
inline double StETofDigi::calibTime()    const { return mCalibTime; };
inline double StETofDigi::rawCharge()    const { return rawTot();   };
inline double StETofDigi::rawTot()       const { return mRawTot;    };
inline double StETofDigi::calibCharge()  const { return calibTot(); };
inline double StETofDigi::calibTot()     const { return mCalibTot;  };

inline unsigned int StETofDigi::sector()   const { return mSector;  };
inline unsigned int StETofDigi::zPlane()   const { return mZPlane;  };
inline unsigned int StETofDigi::counter()  const { return mCounter; };
inline unsigned int StETofDigi::strip()    const { return mStrip;   };
inline unsigned int StETofDigi::chan()     const { return strip();  };
inline unsigned int StETofDigi::side()     const { return mSide;    };
inline unsigned int StETofDigi::elChan()   const { return mElChan;  };
inline unsigned int StETofDigi::get4Id()   const { return mGet4Id;  };
inline unsigned int StETofDigi::rocId()    const { return mRocId;   };

inline StETofHit*   StETofDigi::associatedHit()         { return mAssociatedHit; };
inline StETofHit*   StETofDigi::associatedHit()  const  { return mAssociatedHit; };

inline void StETofDigi::setRawTime(   const double& time )  { mRawTime   = time; };
inline void StETofDigi::setRawTot(    const double& tot  )  { mRawTot    = tot;  };
inline void StETofDigi::setCalibTime( const double& time )  { mCalibTime = time; };
inline void StETofDigi::setCalibTot(  const double& tot  )  { mCalibTot  = tot;  };

inline void StETofDigi::setAssociatedHit( StETofHit* hit ) { mAssociatedHit = hit; };


#endif // STETOFDIGI_H
