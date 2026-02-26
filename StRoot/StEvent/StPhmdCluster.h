/*!
 * \class StPhmdCluster
 * \author
 */
/********************************************************************
 *
 * $Id: StPhmdCluster.h,v 2.3 2003/09/02 17:58:05 perev Exp $
 *
 * Author: Subhasis Chattopadhyay, Dec 2002
 ********************************************************************
 *
 * Description: Base class for PMD cluster
 *
 ********************************************************************
 *
 * $Log: StPhmdCluster.h,v $
 * Revision 2.3  2003/09/02 17:58:05  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.2  2003/04/22 00:08:13  ullrich
 * Removed //! comments
 *
 * Revision 2.1  2002/12/20 22:33:00  ullrich
 * Initial Revision.
 *
 ********************************************************************/
#ifndef STAR_StPhmdCluster
#define STAR_StPhmdCluster

/// @file StPhmdCluster.h
/// @brief Reconstructed cluster in the STAR Photon Multiplicity Detector (PMD/PHMD).

#include <math.h>
#include <Stiostream.h>
#include "StObject.h"
#include "StPhmdHit.h"
#include "StContainers.h"

/// @brief A reconstructed cluster of adjacent cells in the PHMD (Photon Multiplicity Detector).
class StPhmdCluster : public StObject {
public: 
    StPhmdCluster();           
    ~StPhmdCluster();          

    /// @brief Returns the supermodule number containing this cluster.
    int           module() const;
    /// @brief Returns the number of cells (hits) forming the cluster.
    int           numberOfCells() const;
    /// @brief Returns the pseudorapidity (η) of the cluster centroid.
    float         eta() const;
    /// @brief Returns the azimuthal angle (φ) of the cluster centroid (radians).
    float         phi() const;
    /// @brief Returns the total energy deposition of the cluster (GeV).
    float         energy() const;
    /// @brief Returns the spatial spread (σ) of the cluster.
    float         sigma() const;
    /// @brief Returns the particle ID (PID) based on energy deposition.
    int           energyPid() const;
    /// @brief Returns the particle ID (PID) from track matching.
    int           pid() const;
    /// @brief Returns the Monte Carlo particle ID.
    int           mcPid() const;
    
    /// @brief Sets the supermodule number.
    void          setModule(int);
    /// @brief Sets the number of cells in the cluster.
    void          setNumberOfCells(int);
    /// @brief Sets the pseudorapidity (η) of the cluster centroid.
    void          setEta(float);
    /// @brief Sets the azimuthal angle (φ) of the cluster centroid (radians).
    void          setPhi(float);
    /// @brief Sets the total energy deposition (GeV).
    void          setEnergy(float);
    /// @brief Sets the spatial spread (σ) of the cluster.
    void          setSigma(float);
    /// @brief Sets the particle ID (PID) based on energy deposition.
    void          setEnergyPid(int);
    /// @brief Sets the particle ID (PID) from track matching.
    void          setPid(int);
    /// @brief Sets the Monte Carlo particle ID.
    void          setMcPid(int);
    
    /// @brief Adds a PMD hit to this cluster.
    void addHit(StPhmdHit*);
    
    /// @brief Returns a reference to the vector of hits in this cluster.
    StPtrVecPhmdHit&        hit();
    /// @brief Returns a const reference to the vector of hits in this cluster.
    const StPtrVecPhmdHit&  hit() const;    

private:
    Int_t             mModule;          ///< Supermodule number.
    Int_t             mNumberOfCells;   ///< Number of cells (hits) forming the cluster.
    Float_t           mEta;             ///< Pseudorapidity (η) of the cluster centroid.
    Float_t           mPhi;             ///< Azimuthal angle (φ) of the cluster centroid (radians).
    Float_t           mEnergy;          ///< Total energy deposition of the cluster (GeV).
    Float_t           mSigma;           ///< Spatial spread (σ) of the cluster.
    Int_t             mPID;             ///< Particle ID (PID) from track matching.
    Int_t             mEnergyPID;       ///< Particle ID (PID) based on energy deposition.
    Int_t             mMcPID;           ///< Monte Carlo particle ID.
    StPtrVecPhmdHit   mHits;            ///< Collection of PMD hits belonging to this cluster.

    ClassDef(StPhmdCluster,1)
};

ostream &operator<<(ostream&, StPhmdCluster&); // Printing operator


inline   int    StPhmdCluster::module() const    {return mModule;}
inline   int    StPhmdCluster::numberOfCells() const {return mNumberOfCells;}
inline   float  StPhmdCluster::eta() const       {return mEta;} 
inline   float  StPhmdCluster::phi() const       {return mPhi;}
inline   float  StPhmdCluster::energy() const    {return mEnergy;}
inline   float  StPhmdCluster::sigma() const     {return mSigma;}
inline   int    StPhmdCluster::energyPid() const {return mEnergyPID;}
inline   int    StPhmdCluster::pid() const       {return mPID;}
inline   int    StPhmdCluster::mcPid() const     {return mMcPID;}

inline   void   StPhmdCluster::setModule(int var)    {mModule=var;}
inline   void   StPhmdCluster::setNumberOfCells(int var) {mNumberOfCells=var;}
inline   void   StPhmdCluster::setEta(float var)     {mEta=var;}
inline   void   StPhmdCluster::setPhi(float var)     {mPhi=var;}
inline   void   StPhmdCluster::setEnergy(float var)  {mEnergy=var;}
inline   void   StPhmdCluster::setSigma(float var)   {mSigma=var;}
inline   void   StPhmdCluster::setEnergyPid(int var) {mEnergyPID=var;}
inline   void   StPhmdCluster::setPid(int var)       {mPID=var;}
inline   void   StPhmdCluster::setMcPid(int var)     {mMcPID=var;}

ostream& operator<<(ostream &, const StPhmdCluster&);

#endif







