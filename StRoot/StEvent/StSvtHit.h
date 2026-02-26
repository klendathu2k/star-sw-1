/*!
 * \class StSvtHit 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StSvtHit.h,v 2.18 2016/02/26 14:45:15 ullrich Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSvtHit.h,v $
 * Revision 2.18  2016/02/26 14:45:15  ullrich
 * Implemented detector() which is abstract method inherited from StHit.
 *
 * Revision 2.17  2009/11/23 22:20:51  ullrich
 * Minor cleanup performed, fixed compiler warnings.
 *
 * Revision 2.16  2009/11/23 16:34:07  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.15  2009/11/10 00:41:11  ullrich
 * Changed print-out format and added new method shell().
 *
 * Revision 2.14  2007/09/20 20:02:47  ullrich
 * Added new members to hold and access the number of anodes and of pixels.
 *
 * Revision 2.13  2006/04/27 21:59:00  ullrich
 * Added data member and methods to deal with local positions.
 *
 * Revision 2.12  2005/07/19 21:38:56  perev
 * Cleanup
 *
 * Revision 2.11  2004/07/15 16:36:25  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.10  2003/01/08 19:43:11  perev
 * CleanUp
 *
 * Revision 2.9  2002/02/22 22:56:51  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.8  2001/08/07 20:50:53  caines
 * Implement better packing of hardware and charge values
 *
 * Revision 2.7  2001/04/05 04:00:43  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.6  2001/03/24 03:34:59  perev
 * clone() -> clone() const
 *
 * Revision 2.5  1999/12/13 20:16:22  ullrich
 * Changed numbering scheme for hw_position unpack methods (STAR conventions).
 *
 * Revision 2.4  1999/11/11 11:03:57  ullrich
 * Inlined layer(), sector() and ladder().
 *
 * Revision 2.3  1999/11/09 19:35:22  ullrich
 * Memory now allocated using StMemoryPool via overloaded new/delete
 *
 * Revision 2.2  1999/11/04 21:40:57  ullrich
 * Added missing default constructor
 *
 * Revision 2.1  1999/10/28 22:26:44  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.0  1999/10/12 18:42:45  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StSvtHit_hh
#define StSvtHit_hh

/// @file StSvtHit.h
/// @brief Defines the StSvtHit class representing a reconstructed hit in the Silicon Vertex Tracker.

#include "StHit.h"
#include "StMemoryPool.hh"

/// @brief Reconstructed hit in the STAR Silicon Vertex Tracker (SVT).
class StSvtHit : public StHit {
public:
    /// @brief Default constructor.
    StSvtHit();
    /// @brief Constructor with global position, position error, hardware address, charge, and fit flag.
    StSvtHit(const StThreeVectorF&,
             const StThreeVectorF&,
             unsigned int, float, unsigned char = 0);
    // StSvtHit(const StSvtHit&);            use default
    // StSvtHit& operator=(const StSvtHit&); use default
    /// @brief Destructor.
    ~StSvtHit();

    void* operator new(size_t sz,void *p)     { return p;}
    void* operator new(size_t)     { return mPool.alloc(); }
    void  operator delete(void* p) { mPool.free(p); }

    /// @brief Returns the SVT layer number [1-6].
    unsigned int layer() const;
    /// @brief Returns the SVT layer number for the given barrel and ladder indices.
    static unsigned int layer(unsigned int barrel, unsigned int ladder);
    /// @brief Returns the ladder number within the barrel [1-8].
    unsigned int ladder() const;
    /// @brief Returns the wafer number on the ladder [1-7].
    unsigned int wafer() const;
    /// @brief Returns the barrel number [1-3].
    unsigned int barrel() const;
    /// @brief Returns the hybrid number on the wafer [1-2].
    unsigned int hybrid() const;
    /// @brief Returns the hybrid index encoded in the hardware position word.
    unsigned int index() const;
    /// @brief Returns the anode position of the hit in units of 1/4 anode pitch.
    float anode() const;
    /// @brief Returns the time-bucket position of the hit in units of 1/4 time-bucket.
    float timebucket() const;
    /// @brief Returns the peak ADC value of the hit cluster.
    float peakADC() const;
    /// @brief Returns the local hit position along the given coordinate axis (0 or 1).
    float localPosition(unsigned int) const;
    /// @brief Returns the number of anode strips associated with the hit cluster.
    int numberOfAnodes() const;
    /// @brief Returns the number of pixels (anode × time-bucket) in the hit cluster.
    int numberOfPixels() const;

    /// @brief Returns the shell identifier (East/West) for the given barrel and ladder.
    static unsigned int shell(unsigned int barrel, unsigned int ladder);
    /// @brief Returns the shell identifier (East/West) for this hit.
    unsigned int shell() const;    
    
    /// @brief Sets the peak ADC value of the hit cluster.
    void setPeak(float);
    /// @brief Sets the anode position of the hit in units of 1/4 anode pitch.
    void setAnode(float);
    /// @brief Sets the time-bucket position of the hit in units of 1/4 time-bucket.
    void setTimebucket(float);
    /// @brief Sets the local hit position along both coordinate axes.
    void setLocalPosition(float, float);
    /// @brief Returns the geometry volume ID encoding barrel, ladder, and wafer.
    virtual int volumeID() const;
    /// @brief Sets the number of anode strips in the hit cluster.
    void setNumberOfAnodes(unsigned short);
    /// @brief Sets the number of pixels in the hit cluster.
    void setNumberOfPixels(unsigned short);
    /// @brief Prints hit information to the output stream.
    void Print(Option_t *option="") const;
    
    /// @brief Returns the detector identifier (kSvtId).
    StDetectorId detector() const;

protected:
    static StMemoryPool mPool;  //! Memory pool for efficient hit allocation (not streamed).
    Float_t mPeak;              ///<  Peak ADC value of the hit cluster.
    Float_t mAnode;             ///<  Anode position of the hit in units of 1/4 anode pitch.
    Float_t mTimebucket;        ///<  Time-bucket position of the hit in units of 1/4 time-bucket.
    Float_t mLocalPosition[2];  ///<  Local hit position: [0] = anode direction, [1] = drift direction.
    UShort_t mNumberOfAnodes;   ///<  Number of anode strips in the hit cluster.
    UShort_t mNumberOfPixels;   ///<  Number of pixels (anode × time-bucket) in the hit cluster.
    
private:
    enum {mNBarrel=3};
    ClassDef(StSvtHit,3)
};

inline unsigned int
StSvtHit::index() const
{
    // bits 4-13: Hybrid index
    return (mHardwarePosition>>4)&((1L<<9)-1);
}

inline StDetectorId StSvtHit::detector() const {return kSvtId;}
inline float StSvtHit::timebucket() const { return mTimebucket; }
inline void StSvtHit::setPeak(float val) { mPeak = val; }
inline void StSvtHit::setAnode(float val) { mAnode = val; }
inline void StSvtHit::setTimebucket(float val) { mTimebucket = val; }
ostream&              operator<<(ostream& os, StSvtHit const & v);

#endif
