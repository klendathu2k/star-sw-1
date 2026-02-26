/*!
 * \class StFtpcHit 
 * \author Thomas Ullrich, Jan 1999
 */
/***************************************************************************
 *
 * $Id: StFtpcHit.h,v 2.13 2016/02/25 17:10:20 ullrich Exp $
 *
 * Author: Thomas Ullrich, Jan 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StFtpcHit.h,v $
 * Revision 2.13  2016/02/25 17:10:20  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.12  2009/11/23 16:34:06  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.11  2004/07/15 16:36:24  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.10  2004/05/07 15:05:28  calderon
 * Adding constructor based on StFtpcPoint from Markus.
 *
 * Revision 2.9  2004/04/08 19:02:33  ullrich
 * Added additional data member and access methods to hold the position in
 * pad and time units including their std deviation. Constructors updated.
 *
 * Revision 2.8  2003/01/08 19:43:11  perev
 * CleanUp
 *
 * Revision 2.7  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.6  2001/04/05 04:00:37  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.5  2001/03/24 03:34:46  perev
 * clone() -> clone() const
 *
 * Revision 2.4  1999/12/13 20:16:15  ullrich
 * Changed numbering scheme for hw_position unpack methods (STAR conventions).
 *
 * Revision 2.3  1999/12/06 18:28:24  ullrich
 * Changed method names xxxInCluster to xxxInHit
 *
 * Revision 2.2  1999/11/09 19:35:12  ullrich
 * Memory now allocated using StMemoryPool via overloaded new/delete
 *
 * Revision 2.1  1999/10/28 22:25:19  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.0  1999/10/12 18:42:07  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StFtpcHit_hh
#define StFtpcHit_hh

/// @file StFtpcHit.h
/// @brief Defines the StFtpcHit class representing a reconstructed hit in the Forward Time Projection Chamber.

#include "StHit.h"
#include "StMemoryPool.hh"

class StFtpcPoint;

/// @brief Reconstructed hit in the STAR Forward Time Projection Chamber (FTPC).
class StFtpcHit : public StHit {
public:
    /// @brief Default constructor.
    StFtpcHit();
    /// @brief Constructor with global position, position error, hardware address, charge, and fit flag.
    StFtpcHit(const StThreeVectorF&,
              const StThreeVectorF&,
              unsigned int, float, unsigned char = 0);
    /// @brief Constructor from an StFtpcPoint reconstructed cluster object.
    StFtpcHit(const StFtpcPoint&);
    // StFtpcHit(const StFtpcHit&);            use default
    // StFtpcHit& operator=(const StFtpcHit&); use default
    /// @brief Destructor.
    ~StFtpcHit();

    void* operator new(size_t sz,void *p)     { return p;}
    void* operator new(size_t)     { return mPool.alloc(); }
    void  operator delete(void* p) { mPool.free(p); }
  
    /// @brief Returns the FTPC sector number [1-6].
    unsigned int sector() const;
    /// @brief Returns the FTPC readout plane number [1-20].
    unsigned int plane() const;
    /// @brief Returns the number of pads contributing to this hit cluster.
    unsigned int padsInHit() const;
    /// @brief Returns the number of time bins contributing to this hit cluster.
    unsigned int timebinsInHit() const;

    /// @brief Updates hit properties from an StFtpcPoint cluster object.
    void update(const StFtpcPoint&);

    /// @brief Returns the hit centroid position in pad units.
    double padPosition() const;
    /// @brief Returns the hit centroid position in time-bin units.
    double timePosition() const;
    /// @brief Returns the standard deviation of the pad position.
    double sigmaPadPosition() const;
    /// @brief Returns the standard deviation of the time-bin position.
    double sigmaTimePosition() const;

    /// @brief Sets the hit centroid position in pad units.
    void setPadPosition(float);
    /// @brief Sets the hit centroid position in time-bin units.
    void setTimePosition(float);
    /// @brief Sets the standard deviation of the pad position.
    void setSigmaPadPosition(float);
    /// @brief Sets the standard deviation of the time-bin position.
    void setSigmaTimePosition(float);
 
    /// @brief Returns the detector identifier for this hit (East or West FTPC).
    StDetectorId   detector() const;
    

protected:
    static StMemoryPool mPool;  //! Memory pool for efficient hit allocation (not streamed).

    Float_t    mPadPos;        ///<  Hit centroid position in pad units.
    Float_t    mTimePos;       ///<  Hit centroid position in time-bin units.
    Float_t    mPadPosSigma;   ///<  Standard deviation of the pad position.
    Float_t    mTimePosSigma;  ///<  Standard deviation of the time-bin position.
   
    ClassDef(StFtpcHit,2)
};

inline StDetectorId StFtpcHit::detector() const {return static_cast<StDetectorId>(StHit::bits(0, 4));} 

inline double StFtpcHit::padPosition() const {return mPadPos;}
inline double StFtpcHit::timePosition() const {return mTimePos;}
inline double StFtpcHit::sigmaPadPosition() const {return mPadPosSigma;}
inline double StFtpcHit::sigmaTimePosition() const {return mTimePosSigma;}
inline void StFtpcHit::setPadPosition(float val) {mPadPos = val;}
inline void StFtpcHit::setTimePosition(float val) {mTimePos = val;}
inline void StFtpcHit::setSigmaPadPosition(float val) {mPadPosSigma = val;}
inline void StFtpcHit::setSigmaTimePosition(float val) {mTimePosSigma = val;}

#endif
