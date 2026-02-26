/*!
 * \class StRnDHit 
 * \author Mike Miller and Andrew Rose, Jan 2006
 */
/***************************************************************************
 *
 * $Id: StRnDHit.h,v 2.3 2017/05/04 01:06:46 perev Exp $
 *
 * Author: Mike Miller and Andrew Rose, Jan 2006
 ***************************************************************************
 *
 * Description:  This is an experimental class and not final yet
 *
 ***************************************************************************
 *
 * $Log: StRnDHit.h,v $
 * Revision 2.3  2017/05/04 01:06:46  perev
 * Own err matrix added
 *
 * Revision 2.2  2006/09/27 18:31:43  ullrich
 * Fixed setDouble() interface. Was sooo wrong.
 *
 * Revision 2.1  2006/01/19 21:42:06  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StRnDHit_hh
#define StRnDHit_hh

/// @file StRnDHit.h
/// @brief Generic hit class for Research and Development (R&D) detector studies.

#include "StHit.h"
#include "StMemoryPool.hh"
#include "StEnumerations.h"

/// @brief Generic hit class for R&D (Research and Development) detector prototyping.
class StRnDHit : public StHit
{
public:
    /// @brief Default constructor.
    StRnDHit();
    /// @brief Construct with full hit information for a given detector.
    StRnDHit(const StThreeVectorF& position,
	   const StThreeVectorF& error,
	   unsigned int hwPosition, float charge, unsigned char trackRefCount = 0,
	   unsigned short idTruth=0,  unsigned short quality=0,  unsigned short id =0,
	   StDetectorId = kUnknownId);
    /// @brief Destructor.
    ~StRnDHit();

    /// @brief Returns the detector identifier.
    StDetectorId detector() const;
    /// @brief Returns the detector layer index.
    unsigned int    layer() const;
    /// @brief Returns the detector ladder index.
    unsigned int    ladder() const;
    /// @brief Returns the detector wafer index.
    unsigned int    wafer() const;
    
    /// @brief Returns detector-specific spare integer 0.
    int    extraByte0() const;
    /// @brief Returns detector-specific spare integer 1.
    int    extraByte1() const;
    
    /// @brief Returns the key index from the associated StMcHit (MC truth).
    int    key() const;
    /// @brief Returns the volume identifier from the associated StMcHit (MC truth).
    int    volumeId() const;
    
    /// @brief Returns detector-specific spare double 0.
    double double0() const;
    /// @brief Returns detector-specific spare double 1.
    double double1() const;
    /// @brief Returns detector-specific spare double 2.
    double double2() const;
    /// @brief Returns detector-specific spare double 3.
    double double3() const;
    /// @brief Returns detector-specific spare double 4.
    double double4() const;
    
    /// @brief Sets the detector layer index.
    void setLayer(short);
    /// @brief Sets the detector ladder index.
    void setLadder(short);
    /// @brief Sets the detector wafer index.
    void setWafer(short);
    /// @brief Sets detector-specific spare integer 0.
    void setExtraByte0(int);
    /// @brief Sets detector-specific spare integer 1.
    void setExtraByte1(int);
    /// @brief Sets the detector identifier.
    void setDetectorId(StDetectorId);
    
    /// @brief Sets the MC truth key index.
    void setKey(int);
    /// @brief Sets the MC truth volume identifier.
    void setVolumeId(int);
    
    /// @brief Sets detector-specific spare double 0.
    void setDouble0(double);
    /// @brief Sets detector-specific spare double 1.
    void setDouble1(double);
    /// @brief Sets detector-specific spare double 2.
    void setDouble2(double);
    /// @brief Sets detector-specific spare double 3.
    void setDouble3(double);
    /// @brief Sets detector-specific spare double 4.
    void setDouble4(double);

  /// @brief Sets the 3×3 hit covariance matrix from a flat upper-triangle array.
  void setErrorMatrix(const float* M);

  /// @brief Returns the 3×3 hit covariance matrix.
  StMatrixF       covariantMatrix() const;//{ return mErrMatrix; }

    void* operator new(size_t sz,void *p)     { return p;}
    void* operator new(size_t)     { return mPool.alloc(); }
    void  operator delete(void* p) { mPool.free(p); }
    
    friend ostream& operator<<(ostream& os, const StRnDHit& h);
    
protected:
    Short_t mLayer;   ///< Detector layer index.
    Short_t mLadder;  ///< Detector ladder index.
    Short_t mWafer;   ///< Detector wafer index.
    
    //Extras
    Int_t mExtraByte0;  ///< Spare integer for detector-specific data (slot 0).
    Int_t mExtraByte1;  ///< Spare integer for detector-specific data (slot 1).
    
    //info to get back to StMcHit pointer:
    Int_t mKey;       ///< Key index from StMcHit for MC truth association.
    Int_t mVolumeId;  ///< Volume identifier from StMcHit for MC truth association.
    
    //and 5 overflow doubles
    Double_t mDouble0;  ///< Spare double for detector-specific data (slot 0).
    Double_t mDouble1;  ///< Spare double for detector-specific data (slot 1).
    Double_t mDouble2;  ///< Spare double for detector-specific data (slot 2).
    Double_t mDouble3;  ///< Spare double for detector-specific data (slot 3).
    Double_t mDouble4;  ///< Spare double for detector-specific data (slot 4).

    // this has to go once the playing and testing is over.
    // should be hard wired in member function.
    StDetectorId mDetectorId;  ///< Detector identifier.

  float mErrorMatrix[9];  ///< Upper triangle of the 3×3 hit position covariance matrix.
    
    static StMemoryPool mPool;  //!
    
    ClassDef(StRnDHit,2)        
};

inline unsigned int  StRnDHit::layer() const {return mLayer;}
inline unsigned int  StRnDHit::ladder() const {return mLadder;}
inline unsigned int  StRnDHit::wafer() const {return mWafer;}
inline int    StRnDHit::extraByte0() const {return mExtraByte0;}
inline int    StRnDHit::extraByte1() const {return mExtraByte1;}
inline int    StRnDHit::key() const {return mKey;}
inline int    StRnDHit::volumeId() const {return mVolumeId;}
inline double StRnDHit::double0() const {return mDouble0;}
inline double StRnDHit::double1() const {return mDouble1;}
inline double StRnDHit::double2() const {return mDouble2;}
inline double StRnDHit::double3() const {return mDouble3;}
inline double StRnDHit::double4() const {return mDouble4;}
    
inline void StRnDHit::setLayer(short v) {mLayer = v;}
inline void StRnDHit::setLadder(short v) {mLadder = v;}
inline void StRnDHit::setWafer(short v) {mWafer = v;}
inline void StRnDHit::setExtraByte0(int v) {mExtraByte0=v;}
inline void StRnDHit::setExtraByte1(int v) {mExtraByte1=v;}
inline void StRnDHit::setKey(int v) {mKey = v;}
inline void StRnDHit::setVolumeId(int v) {mVolumeId=v;}
inline void StRnDHit::setDouble0(double val) {mDouble0 = val;}
inline void StRnDHit::setDouble1(double val) {mDouble1 = val;}
inline void StRnDHit::setDouble2(double val) {mDouble2 = val;}
inline void StRnDHit::setDouble3(double val) {mDouble3 = val;}
inline void StRnDHit::setDouble4(double val) {mDouble4 = val;}

#endif
