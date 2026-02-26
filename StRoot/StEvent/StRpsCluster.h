/***************************************************************************
 *
 * $Id: StRpsCluster.h,v 2.2 2015/10/02 19:50:09 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:  Reconstructed cluster in the Roman Pot Silicon 
 *               detectors.         
 *
 ***************************************************************************
 *
 * $Log: StRpsCluster.h,v $
 * Revision 2.2  2015/10/02 19:50:09  ullrich
 * Added mPositionRMS and accessors.
 *
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRpsCluster_hh
#define StRpsCluster_hh

/// @file StRpsCluster.h
/// @brief Reconstructed cluster in a Roman Pot silicon strip detector plane.

#include <Stiostream.h>
#include "StObject.h"

class StRpsPlane;

/// @brief Reconstructed hit cluster in a Roman Pot (RPS) silicon strip detector plane.
///
/// A cluster is formed from one or more adjacent strips with signal in a single detector
/// plane. It stores the reconstructed hit position along the sensitive axis, the RMS
/// spread of that position, the energy deposition, and a quality flag.
class StRpsCluster : public StObject {
public:
    /// @brief Default constructor.
    StRpsCluster();
    /// @brief Construct a cluster with position, position RMS, strip length, energy, xy coordinate, and quality.
    StRpsCluster(double pos, double posRMS, short len,
                 double e, double xy, unsigned char qual);
    ~StRpsCluster();

    /// @brief Returns the reconstructed hit position along the strip direction (cm).
    double position() const;
    /// @brief Returns the RMS spread of the cluster position (cm).
    double positionRMS() const;
    /// @brief Returns the number of strips in the cluster.
    short  length() const;
    /// @brief Returns the total energy deposited in the cluster (ADC counts).
    double energy() const;
    /// @brief Returns the transverse coordinate perpendicular to the strips (cm).
    double xy() const;
    /// @brief Returns the cluster quality flag.
    unsigned char quality() const;

    /// @brief Returns the parent Roman Pot identifier (0–7).
    unsigned int romanPotId() const;
    /// @brief Returns the parent detector plane identifier (0–3).
    unsigned int planeId() const;
    
    /// @brief Sets the reconstructed hit position (cm).
    void setPosition(double);
    /// @brief Sets the RMS of the cluster position (cm).
    void setPositionRMS(double);
    /// @brief Sets the number of strips in the cluster.
    void setLength(short);
    /// @brief Sets the energy deposited in the cluster (ADC counts).
    void setEnergy(double);
    /// @brief Sets the transverse coordinate perpendicular to strips (cm).
    void setXY(double);
    /// @brief Sets the cluster quality flag.
    void setQuality(unsigned char);

protected:
    friend class StRpsPlane;
    void setPlaneId(unsigned char);
    void setRomanPotId(unsigned char);

 protected:
    Double_t       mPosition;    ///< Reconstructed hit position along the strip direction (cm).
    Double_t       mPositionRMS; ///< RMS spread of the cluster position (cm).
    Short_t        mLength;      ///< Number of strips in the cluster.
    Double_t       mEnergy;      ///< Total energy deposited in the cluster (ADC counts).
    Double_t       mXY;          ///< Transverse coordinate perpendicular to the strips (cm).
    UChar_t        mQuality;     ///< Cluster quality flag.
    UChar_t        mPlaneId;     ///< Parent detector plane index (0–3).
    UChar_t        mRomanPotId;  ///< Parent Roman Pot index (0–7).

    ClassDef(StRpsCluster,2)
};

ostream& operator<<(ostream&, const StRpsCluster&);

#endif
