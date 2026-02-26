/***************************************************************************
 *
 * $Id: StETofHit.h,v 2.3 2019/02/11 18:53:09 ullrich Exp $
 *
 * Author: Philipp Weidenkaff, April 2018
 ***************************************************************************
 *
 * Description: Data class for expanded digital eTOF information:
 * eTOF hits created out of 2 or more eTOF digis from different sides of the
 * MRPC counters 
 *
 ***************************************************************************
 *
 * $Log: StETofHit.h,v $
 * Revision 2.3  2019/02/11 18:53:09  ullrich
 * Added additional access functions to get the associated track & idTruth and qaTruth variables for simulated Hits.
 *
 * Revision 2.2  2018/07/13 14:55:09  ullrich
 * Added getter function for the associated hit (Florian)
 *
 * Revision 2.1  2018/07/09 14:53:48  ullrich
 * Initial Revision.
 *
 *
 ***************************************************************************/
#ifndef STETOFHIT_H
#define STETOFHIT_H

/// @file StETofHit.h
/// @brief Reconstructed hit in the STAR Endcap TOF (eTOF) detector, formed from two or more MRPC strip digis.

#include <Stiostream.h>
#include "StObject.h"

class StTrack;

/// @brief Reconstructed hit in the STAR Endcap TOF (eTOF) MRPC detector.
class StETofHit : public StObject {
public:
    /// @brief Default constructor.
    StETofHit();

    /// @brief Constructs with full geometry and measurement information.
    StETofHit( const unsigned int sector, const unsigned int zPlane, const unsigned int counter,
               const double& time, const double& tot, const unsigned int clusterSize,
               const double& localX, const double& localY );

    /// @brief Copy constructor.
    StETofHit( const StETofHit& );

    /// @brief Destructor.
    ~StETofHit();

    /// @brief Returns the STAR sector number.
    unsigned int sector()      const;
    /// @brief Returns the eTOF z-plane number.
    unsigned int zPlane()      const;
    /// @brief Returns the counter (MRPC module) number.
    unsigned int counter()     const;

    /// @brief Returns the calibrated hit time [ns].
    double time()              const;

    /// @brief Returns the summed time-over-threshold of the contributing digis [ns].
    double totalTot()          const;

    /// @brief Returns the number of strips contributing to this hit.
    unsigned int clusterSize() const;

    /// @brief Returns the local X position on the counter [cm].
    double localX()            const;
    /// @brief Returns the local Y position on the counter [cm].
    double localY()            const;

    /// @brief Returns a pointer to the reconstructed track matched to this hit.
    StTrack* associatedTrack();
    
    /// @brief Returns a const pointer to the reconstructed track matched to this hit.
    StTrack* associatedTrack() const;

    /// @brief Returns the Monte Carlo truth track ID.
    unsigned int idTruth()     const;
    /// @brief Returns the quality of the MC truth association (% of charge from mIdTruth).
    unsigned int qaTruth()     const;

    /// @brief Less-than operator; sorts by time within the same reference frame.
    bool operator<( const StETofHit& rhs ) const; 
    
    /// @brief Compares this hit with another StObject by time.
    int compare( const StObject*  obj )    const;
    /// @brief Compares this hit with another StETofHit by time.
    int compare( const StETofHit* hit )    const;

    /// @brief Sets the hardware address (sector, z-plane, counter).
    void setHwAddress( const unsigned int iSector, const unsigned int iZPlane, const unsigned int iCounter);

    /// @brief Sets the sector number.
    void setSector(  const unsigned int sector   );
    /// @brief Sets the z-plane number.
    void setZPlane(  const unsigned int zPlane   );
    /// @brief Sets the counter number.
    void setCounter( const unsigned int counter  );

    /// @brief Sets the summed time-over-threshold [ns].
    void setTotalTot   ( const double& tot   );
    /// @brief Sets the calibrated hit time [ns].
    void setTime       ( const double& time  );

    /// @brief Sets the number of strips contributing to this hit.
    void setClusterSize( const unsigned int clustSize );

    /// @brief Sets the local X position on the counter [cm].
    void setLocalX     ( const double& X     );
    /// @brief Sets the local Y position on the counter [cm].
    void setLocalY     ( const double& Y     );

    /// @brief Sets the pointer to the matched reconstructed track.
    void setAssociatedTrack( StTrack* trk );

    /// @brief Sets the Monte Carlo truth track ID and quality.
    void setIdTruth( unsigned short idtruth, unsigned short qatruth=0 );


private:
    UInt_t      mSector;   ///< STAR sector number
    UInt_t      mZPlane;   ///< eTOF z-plane number
    UInt_t      mCounter;  ///< Counter (MRPC module) number
    
    Double_t    mTime;      ///< Calibrated hit time [ns]
    Double_t    mTotalTot;  ///< Summed time-over-threshold [ns]

    UInt_t      mClusterSize;  ///< Number of contributing strips

    Double_t    mLocalX;  ///< Local X position on counter [cm]
    Double_t    mLocalY;  ///< Local Y position on counter [cm]

    StTrack*    mAssociatedTrack;  //$LINK

    UShort_t     mIdTruth;  ///< Monte Carlo truth track ID
    UShort_t     mQuality;  ///< Quality of MC truth association (% charge from mIdTruth)



    ClassDef( StETofHit, 1 );
};

ostream& operator << ( ostream&, const StETofHit& hit ); // Printing operator


inline unsigned int StETofHit::sector()      const { return mSector;  };
inline unsigned int StETofHit::zPlane()      const { return mZPlane;  };
inline unsigned int StETofHit::counter()     const { return mCounter; };

inline double StETofHit::time()     const { return mTime;     }
inline double StETofHit::totalTot() const { return mTotalTot; };

inline unsigned int StETofHit::clusterSize() const { return mClusterSize; };

inline double StETofHit::localX()   const { return mLocalX; };
inline double StETofHit::localY()   const { return mLocalY; };

inline StTrack* StETofHit::associatedTrack()       { return mAssociatedTrack; };
inline StTrack* StETofHit::associatedTrack() const { return mAssociatedTrack; };

inline unsigned int StETofHit::idTruth()     const { return mIdTruth; }
inline unsigned int StETofHit::qaTruth()     const { return mQuality; }

inline void StETofHit::setSector(  const unsigned int sector  )  { mSector   = sector;  }
inline void StETofHit::setZPlane(  const unsigned int zPlane  )  { mZPlane   = zPlane;  }
inline void StETofHit::setCounter( const unsigned int counter )  { mCounter  = counter; }

inline void StETofHit::setTotalTot   ( const double& tot   )  { mTotalTot    = tot;     }
inline void StETofHit::setTime       ( const double& time  )  { mTime        = time;    }

inline void StETofHit::setClusterSize( const unsigned int clustSize ) { mClusterSize = clustSize; }

inline void StETofHit::setLocalX     ( const double& X     )  { mLocalX = X; }
inline void StETofHit::setLocalY     ( const double& Y     )  { mLocalY = Y; }

inline void StETofHit::setAssociatedTrack( StTrack* trk ) { mAssociatedTrack = trk; }


#endif // STETOFHIT_H 
