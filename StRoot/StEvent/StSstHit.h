/*!
 * \class StSstHit 
 * \author Jonathan Bouchet, Thomas Ullrich, May 2015
 */
/***************************************************************************
 *
 * $Id: StSstHit.h,v 2.3 2016/02/25 17:10:20 ullrich Exp $
 *
 * Author: Jonathan Bouchet, Thomas Ullrich, May 2015
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSstHit.h,v $
 * Revision 2.3  2016/02/25 17:10:20  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.2  2015/05/21 14:11:43  ullrich
 * Changed mADC from int to unsigned short.
 *
 * Revision 2.1  2015/05/13 16:50:59  ullrich
 * Initial Revision.
 *
  *
 **************************************************************************/
#ifndef StSstHit_hh
#define StSstHit_hh

/// @file StSstHit.h
/// @brief Defines the StSstHit class representing a reconstructed hit in the Silicon Strip Tracker.

#include "StHit.h"
#include "StMemoryPool.hh"

/// @brief Reconstructed hit in the STAR Silicon Strip Tracker (SST), the upgrade replacement for the SSD.
class StSstHit : public StHit {
public:
    /// @brief Default constructor.
    StSstHit();
    /// @brief Constructor with global position, position error, hardware address, charge, and fit flag.
    StSstHit(const StThreeVectorF&,
             const StThreeVectorF&,
             unsigned int, float, unsigned char = 0);
    // StSstHit(const StSstHit&);            use default
    // StSstHit& operator=(const StSstHit&); use default
    /// @brief Destructor.
    ~StSstHit();

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
    /// @brief Returns the local hit position along the given coordinate axis (0, 1, or 2).
    float        localPosition(unsigned int) const;
    /// @brief Returns the sector number [1-4] for the given ladder index.
    static unsigned int sector(unsigned int);
    /// @brief Returns the sector number [1-4] for this hit.
    unsigned int sector() const;
    /// @brief Sets the local hit position along three coordinate axes.
    void         setLocalPosition(float, float, float);
    /// @brief Sets the ADC values for the P-side (index 0) and N-side (index 1) of the hit cluster.
    void         setADC(unsigned short, unsigned short); 
    /// @brief Returns the ADC value for the given side (0 = P-side, 1 = N-side).
    int          getADC(unsigned int) const; 
    /// @brief Returns the geometry volume ID encoding ladder and wafer.
    virtual int  volumeID() const;
    /// @brief Prints hit information to the output stream.
    void         Print(const Option_t *option="") const;
    
    /// @brief Returns the detector identifier (kSstId).
    StDetectorId detector() const;

protected:
    static StMemoryPool mPool;  //! Memory pool for efficient hit allocation (not streamed).
    Float_t mLocalPosition[3];  ///<  Local hit position: [0] = P-side, [1] = N-side, [2] = z.
    UShort_t   mADC[2];         ///<  ADC values: [0] = P-side cluster, [1] = N-side cluster.
    
private:
    enum {mWaferPerLadder=16};
    
    ClassDef(StSstHit,2)
};

inline StDetectorId StSstHit::detector() const {return kSstId;}

inline unsigned int
StSstHit::sector(unsigned int ladder) {
    if (ladder <=  2 || ladder == 20) return 1;
    if (ladder >=  3 && ladder <=  9) return 2;
    if (ladder >= 10 && ladder <= 12) return 3;
    if (ladder >= 13 && ladder <= 19) return 4;
    return 0;
}

inline unsigned int
StSstHit::sector() const {return sector(ladder()); }

ostream& operator<<(ostream& os, StSstHit const & v);

#endif
