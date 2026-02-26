/*!
 * \class StSsdHit 
 * \author Thomas Ullrich, Jan 1999
 */
/***************************************************************************
 *
 * $Id: StSsdHit.h,v 2.13 2016/02/25 17:10:20 ullrich Exp $
 *
 * Author: Thomas Ullrich, Jan 1999
 *         Lilian Martin, Dec 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSsdHit.h,v $
 * Revision 2.13  2016/02/25 17:10:20  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.12  2009/11/23 22:20:51  ullrich
 * Minor cleanup performed, fixed compiler warnings.
 *
 * Revision 2.11  2009/11/23 16:34:07  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.10  2006/04/27 21:58:53  ullrich
 * Added data member and methods to deal with local positions.
 *
 * Revision 2.9  2004/07/15 16:36:25  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.8  2003/01/08 19:43:11  perev
 * CleanUp
 *
 * Revision 2.7  2002/02/22 22:56:50  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.6  2001/04/05 04:00:42  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.5  2001/03/24 03:34:58  perev
 * clone() -> clone() const
 *
 * Revision 2.4  2000/01/05 16:05:39  ullrich
 * Updated for actual use in StEvent. Unpacking changed.
 *
 * Revision 2.3  1999/11/09 19:35:17  ullrich
 * Memory now allocated using StMemoryPool via overloaded new/delete
 *
 * Revision 2.2  1999/10/28 22:26:39  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:43:42  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StSsdHit_hh
#define StSsdHit_hh

/// @file StSsdHit.h
/// @brief Defines the StSsdHit class representing a reconstructed hit in the Silicon Strip Detector.

#include "StHit.h"
#include "StMemoryPool.hh"

/// @brief Reconstructed hit in the STAR Silicon Strip Detector (SSD).
class StSsdHit : public StHit {
public:
    /// @brief Default constructor.
    StSsdHit();
    /// @brief Constructor with global position, position error, hardware address, charge, and fit flag.
    StSsdHit(const StThreeVectorF&,
             const StThreeVectorF&,
             unsigned int, float, unsigned char = 0);
    // StSsdHit(const StSsdHit&);            use default
    // StSsdHit& operator=(const StSsdHit&); use default
    /// @brief Destructor.
    ~StSsdHit();

    void* operator new(size_t sz,void *p)     { return p;}
    void* operator new(size_t)     { return mPool.alloc(); }
    void  operator delete(void* p) { mPool.free(p); }    

    /// @brief Returns the ladder number [1-20].
    unsigned int ladder() const;
    /// @brief Returns the wafer number on the ladder [1-16].
    unsigned int wafer() const;
    /// @brief Returns the central strip index on the N-side of the hit cluster.
    unsigned int centralStripNSide() const;  
    /// @brief Returns the central strip index on the P-side of the hit cluster.
    unsigned int centralStripPSide() const;  
    /// @brief Returns the cluster size (number of strips) on the N-side.
    unsigned int clusterSizeNSide() const;   
    /// @brief Returns the cluster size (number of strips) on the P-side.
    unsigned int clusterSizePSide() const;
    /// @brief Returns the local hit position along the given coordinate axis (0 or 1).
    float        localPosition(unsigned int) const;
    /// @brief Returns the sector number [1-4] for the given ladder index.
    static unsigned int sector(unsigned int);
    /// @brief Returns the sector number [1-4] for this hit.
    unsigned int sector() const;
    /// @brief Sets the local hit position along both coordinate axes.
    void         setLocalPosition(float, float);
    /// @brief Returns the geometry volume ID encoding ladder and wafer.
    virtual int  volumeID() const;
    
    /// @brief Returns the detector identifier for this hit.
    StDetectorId detector() const;
    /// @brief Prints hit information to the output stream.
    void         Print(const Option_t *option="") const;
    
    

protected:
    static StMemoryPool mPool;  //! Memory pool for efficient hit allocation (not streamed).
    Float_t mLocalPosition[2];  ///<  Local hit position: [0] = P-side strip direction, [1] = N-side strip direction.
    
private:
    enum {mWaferPerLadder=16};
    ClassDef(StSsdHit,2)
};

inline unsigned int
StSsdHit::sector(unsigned int ladder) {
    if (ladder <=  2 || ladder == 20) return 1;
    if (ladder >=  3 && ladder <=  9) return 2;
    if (ladder >= 10 && ladder <= 12) return 3;
    if (ladder >= 13 && ladder <= 19) return 4;
    return 0;
}

inline unsigned int StSsdHit::sector() const {return sector(ladder()); }
inline StDetectorId StSsdHit::detector() const {return static_cast<StDetectorId>(StHit::bits(0, 4));}

ostream&              operator<<(ostream& os, StSsdHit const & v);

#endif
