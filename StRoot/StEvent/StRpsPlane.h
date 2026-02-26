/***************************************************************************
 *
 * $Id: StRpsPlane.h,v 2.1 2009/11/23 22:18:25 ullrich Exp $
 *
 * Author: Thomas Ullrich, Nov 2009
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StRpsPlane.h,v $
 * Revision 2.1  2009/11/23 22:18:25  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRpsPlane_hh
#define StRpsPlane_hh

/// @file StRpsPlane.h
/// @brief A single silicon strip detector plane within a Roman Pot station.

#include "StObject.h"
#include "StContainers.h"


class StRpsCluster;
class StRpsRomanPot;

/// @brief One silicon strip detector plane within a Roman Pot station.
///
/// Each Roman Pot contains four planes arranged to measure the transverse proton
/// position. A plane holds reconstructed clusters and the geometric and status
/// parameters (z-position, alignment offset, rotation angle, and strip orientation).
class StRpsPlane : public StObject {
public:
    /// @brief Default constructor.
    StRpsPlane();
    ~StRpsPlane();

    /// @brief Returns the alignment offset of the plane along its sensitive axis (cm).
    double offset() const;
    /// @brief Returns the z-position of the plane along the beam axis (cm).
    double z() const;
    /// @brief Returns the rotation angle of the plane about the beam axis (rad).
    double angle() const;
    /// @brief Returns the strip orientation code (0 = x-measuring, 1 = y-measuring).
    short  orientation() const;
    /// @brief Returns the detector plane status flag.
    unsigned char status() const;
    /// @brief Returns the number of reconstructed clusters in this plane.
    unsigned int  numberOfClusters() const;
    /// @brief Returns the plane identifier within the Roman Pot (0–3).
    unsigned int  planeId() const;
    
    /// @brief Returns the parent Roman Pot identifier (0–7).
    unsigned int  romanPotId() const;

    /// @brief Returns a const pointer to the cluster at the given index.
    const StRpsCluster* cluster(unsigned int) const;
    /// @brief Returns a pointer to the cluster at the given index.
    StRpsCluster*       cluster(unsigned int);
        
    /// @brief Returns a const reference to the cluster container for this plane.
    const StSPtrVecRpsCluster& clusters() const;
    /// @brief Returns a reference to the cluster container for this plane.
    StSPtrVecRpsCluster&       clusters();

    /// @brief Adds a reconstructed cluster to this plane.
    void addCluster(StRpsCluster*);
    /// @brief Sets the alignment offset along the sensitive axis (cm).
    void setOffset(double);
    /// @brief Sets the z-position along the beam axis (cm).
    void setZ(double);
    /// @brief Sets the rotation angle of the plane about the beam axis (rad).
    void setAngle(double);
    /// @brief Sets the strip orientation code.
    void setOrientation(short);
    /// @brief Sets the detector plane status flag.
    void setStatus(unsigned char);
  
protected:    
    friend class StRpsCollection;
    void setPlaneId(unsigned char);
    void setRomanPotId(unsigned char);
    
protected:
    UChar_t  mPlaneId;    ///< Plane index within the Roman Pot (0–3).
    Double_t mOffset;     ///< Alignment offset along the sensitive axis (cm).
    Double_t mZ;          ///< z-position of the plane along the beam axis (cm).
    Double_t mAngle;      ///< Rotation angle of the plane about the beam axis (rad).
    Short_t  mOrientation; ///< Strip orientation code (0 = x-measuring, 1 = y-measuring).
    UChar_t  mStatus;     ///< Detector plane status flag.
    UChar_t  mRomanPotId; ///< Parent Roman Pot index (0–7).
    StSPtrVecRpsCluster mClusters; ///< Collection of reconstructed clusters in this plane.

    ClassDef(StRpsPlane,1)
};

#endif
