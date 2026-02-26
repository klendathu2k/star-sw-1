/*!
 * \class StTpcHit
 * \author Thomas Ullrich, Jan 1999
 */
/***************************************************************************
 *
 * $Id: StTpcHit.h,v 2.34 2018/10/17 20:45:26 fisyak Exp $
 *
 * Author: Thomas Ullrich, Jan 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTpcHit.h,v $
 * Revision 2.34  2018/10/17 20:45:26  fisyak
 * Restore update for Run XVIII dE/dx calibration removed by Gene on 08/07/2018
 *
 * Revision 2.32  2018/08/06 15:54:30  fisyak
 * Add dX
 *
 * Revision 2.30  2016/05/16 02:44:00  perev
 * Added init of mAdd. (Coverity)
 *
 * Revision 2.29  2016/02/25 17:10:20  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.28  2014/01/15 21:00:54  fisyak
 * Step back with version
 *
 * Revision 2.27  2014/01/15 20:59:14  fisyak
 * Step back with version
 *
 * Revision 2.25  2013/12/17 15:01:40  fisyak
 * Add transient data members for upper and lower pad positons
 *
 * Revision 2.23  2013/07/16 14:29:04  fisyak
 * Restore mass fit tracks
 *
 * Revision 2.22  2012/10/23 20:17:16  fisyak
 * Allow more then 64 pad rows
 *
 * Revision 2.21  2011/10/17 00:13:49  fisyak
 * Add handles for IdTruth info
 *
 * Revision 2.20  2011/05/12 22:25:48  fisyak
 * Restore hit errors as persistent, add sort to TpcHit
 *
 * Revision 2.19  2011/03/31 19:25:13  fisyak
 * Keep ADC value for cluster
 *
 * Revision 2.18  2011/01/20 18:25:41  genevb
 * Place FCF_flags include where it is needed
 *
 * Revision 2.17  2010/10/04 19:06:13  fisyak
 * Add reference to FCF flag definition
 *
 * Revision 2.16  2010/08/31 20:01:04  fisyak
 * Fix no. of padsInHit, accounting afterburner
 *
 * Revision 2.15  2010/03/26 13:47:29  fisyak
 * Add methods to modify hit content
 *
 * Revision 2.14  2009/11/23 22:20:51  ullrich
 * Minor cleanup performed, fixed compiler warnings.
 *
 * Revision 2.13  2009/11/23 16:34:07  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.12  2007/10/03 21:47:36  ullrich
 * Added several new member to hold hit length info.
 *
 * Revision 2.11  2004/08/06 15:37:09  fisyak
 * Add clster id
 *
 * Revision 2.10  2004/07/15 16:36:25  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.9  2003/01/08 19:43:11  perev
 * CleanUp
 *
 * Revision 2.8  2002/02/22 22:56:52  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.7  2001/04/05 04:00:44  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.6  2001/03/24 03:34:59  perev
 * clone() -> clone() const
 *
 * Revision 2.5  1999/12/13 20:16:27  ullrich
 * Changed numbering scheme for hw_position unpack methods (STAR conventions).
 *
 * Revision 2.4  1999/12/01 15:56:31  ullrich
 * Renamed xxxInCluster() methods to xxxInHit()
 *
 * Revision 2.3  1999/11/11 10:19:55  ullrich
 * Inlined sector() and padrow().
 *
 * Revision 2.2  1999/11/09 19:35:27  ullrich
 * Memory now allocated using StMemoryPool via overloaded new/delete
 *
 * Revision 2.1  1999/10/28 22:27:10  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.0  1999/10/12 18:42:51  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StTpcHit_hh
#define StTpcHit_hh

/// @file StTpcHit.h
/// @brief Reconstructed hit class for the STAR Time Projection Chamber.

#include "StHit.h"
#include "StMemoryPool.hh"
#include "TMath.h"

/// @brief Reconstructed hit (cluster) in the STAR Time Projection Chamber.
class StTpcHit : public StHit {
  
 public:
    /// @brief Default constructor; initialises all cluster-extent fields to zero.
 StTpcHit() : StHit() {mMinpad = mMaxpad = mMintmbk = mMaxtmbk = 0; mMcl_x = mMcl_t = 0; mAdc =0; mChargeModified = 0; mdX = 0;}
    /// @brief Construct a TPC hit with full position, charge, hardware address, and cluster extent.
 StTpcHit(const StThreeVectorF& p,
	  const StThreeVectorF& e,
	  UInt_t hw, float q, UChar_t c = 0,
	  Int_t IdTruth=0, UShort_t quality=0,
	  UShort_t Id =0,
	  Short_t mnpad=0, Short_t mxpad=0, Short_t mntmbk=0,
	  Short_t mxtmbk=0, Float_t cl_x = 0, Float_t cl_t = 0, UShort_t Adc = 0) 
   :  StHit(p, e, hw, q, c, IdTruth, quality, Id), mAdc(Adc) {
    setExtends(cl_x, cl_t, mnpad, mxpad, mntmbk, mxtmbk); mChargeModified = 0; mdX = 0;}
    /// @brief Destructor.
  ~StTpcHit() {}
    
    /// @brief Returns the sub-detector identifier (kTpcId or kiTpcId).
  StDetectorId   detector() const;

    /// @brief Placement new using caller-supplied memory.
    void* operator new(size_t /* sz */,void *p) { return p;}
    /// @brief Pool-allocating new operator; uses StMemoryPool for efficiency.
    void* operator new(size_t) { return mPool.alloc(); }
    /// @brief Pool-deallocating delete operator.
    void  operator delete(void* p) { mPool.free(p); }

    /// @brief Sets the modified (recalibrated) cluster charge.
    void     setChargeModified(Float_t Charge) {mChargeModified = Charge;}
    /// @brief Sets the cluster centroid position in pad and time-bucket coordinates (stored × 64).
    void     setPadTmbk(Float_t cl_x, Float_t cl_t) { mMcl_x = TMath::Nint(cl_x*64);  mMcl_t = TMath::Nint(cl_t*64);}
    /// @brief Sets the cluster centroid and its extent in pad and time-bucket coordinates.
    void     setExtends(Float_t cl_x, Float_t cl_t, Short_t mnpad, Short_t mxpad, Short_t mntmbk, Short_t mxtmbk);
    /// @brief Sets the cluster ADC sum.
    void     setAdc(UShort_t Adc = 0) {mAdc = Adc;}
    /// @brief Sets the estimated track path length through the pad (dX) used for dE/dx.
    void     setdX(Float_t dX) {mdX = dX;}
    /// @brief Returns the TPC sector number (1–24); packed in hardware address bits 4–8.
    UInt_t   sector() const {return bits(4, 5);}   // bits 4-8  -> 1-24
    /// @brief Returns the TPC padrow number (1–128); packed in hardware address bits 9–15.
    UInt_t   padrow() const {return bits(9, 7);}   // bits 9-15 -> 1-128
    /// @brief Returns the number of pads that fired in this cluster.
    UInt_t   padsInHit()   const {return maxPad() - minPad() + 1;}
    /// @brief Returns the number of pixels (or time buckets for FCF) in the cluster; obsolete for TCL.
    UInt_t   pixelsInHit() const {return bits(22,10);};   // bits 22-31 obsolete (TCL only, FCF put no. of time buckets)
    /// @brief Returns the lowest pad number contributing to this cluster.
    UChar_t  minPad()   const {return TMath::Nint(mMcl_x/64.) - mMinpad;}
    /// @brief Returns the highest pad number contributing to this cluster.
    UChar_t  maxPad()   const {return TMath::Nint(mMcl_x/64.) + mMaxpad;}
    /// @brief Returns the first (earliest) time bucket in this cluster.
    Short_t  minTmbk()  const {return TMath::Nint(mMcl_t/64.) - mMintmbk;}
    /// @brief Returns the last (latest) time bucket in this cluster.
    Short_t  maxTmbk()  const {return TMath::Nint(mMcl_t/64.) + mMaxtmbk;}
    /// @brief Returns a combined volume identifier: sector × 100 + padrow.
    Int_t    volumeID() const {return 100 * sector() + padrow();}
    /// @brief Returns the number of time buckets fired in this cluster.
    Short_t  timeBucketsInHit()   const {return maxTmbk() - minTmbk() + 1;} // number of time bucket fired in this hit
    /// @brief Returns the cluster centroid time bucket (continuous, floating-point).
    Float_t  timeBucket() const {return static_cast<float>(mMcl_t)/64.;}
    /// @brief Returns the cluster centroid pad position (continuous, floating-point).
    Float_t  pad() const {return static_cast<float>(mMcl_x)/64.;}
    /// @brief Returns the cluster ADC sum.
    UShort_t adc() const {return mAdc;}
    /// @brief Returns the modified (recalibrated) cluster charge; transient.
    Float_t  chargeModified() const {return mChargeModified;}
    /// @brief Returns the estimated track path length through the pad for dE/dx.
    Float_t  dX() {return mdX;}
    /// @brief Prints hit information to the given output option.
    void     Print(Option_t *option="") const;
    /// @brief Returns kTRUE; hits are sortable by sector, padrow, and drift distance.
    virtual Bool_t   IsSortable() const { return kTRUE; }
    /// @brief Compares this hit to @p obj, ordering by sector, padrow, then |z|.
    virtual Int_t    Compare(const TObject *obj) const {
        StTpcHit *hit = (StTpcHit *) obj;
        if (sector() > hit->sector()) return kTRUE;
        if (padrow() > hit->padrow()) return kTRUE;
        if (TMath::Abs(position().z()) > TMath::Abs(hit->position().z())) return kTRUE;
        return kFALSE;
    }
    /// @brief Returns the 3-D position of the upper pad-row edge of the cluster (y_local + padlength/2); transient.
    virtual const StThreeVectorF& positionU() const {return *&mPositionU;}
    /// @brief Returns the 3-D position of the lower pad-row edge of the cluster (y_local − padlength/2); transient.
    virtual const StThreeVectorF& positionL() const {return *&mPositionL;}
    /// @brief Sets the upper pad-row edge position of the cluster; transient.
    virtual void setPositionU(const StThreeVectorF& p) {mPositionU = p;}
    /// @brief Sets the lower pad-row edge position of the cluster; transient.
    virtual void setPositionL(const StThreeVectorF& p) {mPositionL = p;}
    
protected:
    /// @brief Memory pool for efficient per-event allocation of StTpcHit objects; transient.
    static StMemoryPool mPool;  //!
    UChar_t     mMinpad;     ///< Offset from the centroid pad to the lowest pad in the cluster.
    UChar_t     mMaxpad;     ///< Offset from the centroid pad to the highest pad in the cluster.
    UChar_t     mMintmbk;    ///< Offset from the centroid time bucket to the first time bucket in the cluster.
    UChar_t     mMaxtmbk;    ///< Offset from the centroid time bucket to the last time bucket in the cluster.
    Short_t     mMcl_x;      ///< Cluster centroid pad position × 64 (fixed-point encoding).
    Short_t     mMcl_t;      ///< Cluster centroid time bucket × 64 (fixed-point encoding).
    UShort_t    mAdc;        ///< Cluster ADC sum.
    /// @brief Modified (recalibrated) cluster charge; transient.
    Float_t     mChargeModified; //!
    StThreeVectorF mPositionU; ///< Upper pad-row edge position: y_local + padlength/2; transient.
    StThreeVectorF mPositionL; ///< Lower pad-row edge position: y_local − padlength/2; transient.
    Float_t        mdX;        ///< Estimated track path length through the pad (dX) from StdEdxY2Maker; transient.
    ClassDef(StTpcHit,10)
};
ostream&              operator<<(ostream& os, StTpcHit const & v);

inline StDetectorId StTpcHit::detector() const {return (bits(1,1) ? kiTpcId : kTpcId);}

#endif
