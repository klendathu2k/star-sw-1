/*!
 * \class StPxlHit 
 * \authors S. MArgetis, J. Bouchet, Jan 2013
 * \Initial Revision.
 */
/***************************************************************************
 * 
 * $Id: StPxlHit.h,v 2.6 2017/05/04 00:58:28 perev Exp $
 *
 * Author: S. Margetis, J. Bouchet, Jan 2013
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StPxlHit.h,v $
 * Revision 2.6  2017/05/04 00:58:28  perev
 * Cleanup
 *
 * Revision 2.5  2016/02/25 17:10:20  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.4  2015/05/13 18:05:25  ullrich
 * New constructors for setting local hit position, proper initialization
 * of all data member, modified existing constructor, new getter and
 * setter for local hit coordinates.
 *
 * Revision 2.3  2014/04/10 16:00:13  jeromel
 * Changes to inlcude Ist structure (Thomas OK-ed / may revisit some comments)
 *
 * Revision 2.2  2013/06/09 22:04:42  ullrich
 * Modified layer() method.
 *
 * Revision 2.1  2013/03/05 14:40:40  ullrich
 * Initial Revision.
 * 
 **************************************************************************/
#ifndef StPxlHit_hh
#define StPxlHit_hh
/// @file StPxlHit.h
/// @brief Pixel Detector (PXL) hit class for the STAR HFT innermost layer.
#include "StHit.h"
#include "StMemoryPool.hh"
#include "StEnumerations.h"

/// @brief Reconstructed hit in the STAR Pixel Detector (PXL), the innermost HFT layer.
class StPxlHit : public StHit
{
public:
    /// @brief Default constructor.
    StPxlHit();
    /// @brief Constructor with full local position, geometry, charge, and truth information.
    StPxlHit(const double localPos[3], unsigned int sector, unsigned int ladder,
             unsigned int sensor, const StThreeVectorF& position, const StThreeVectorF& error,
             unsigned int hwPosition, float charge, unsigned char trackRefCount = 0,
             unsigned short idTruth=0, unsigned short quality=0, unsigned short id=0);
    /// @brief Constructor with local position, geometry, and Monte Carlo truth label.
    StPxlHit(const double localPos[3], unsigned int sector, unsigned int ladder,
             unsigned int sensor, unsigned short idTruth);
    /// @brief Constructor with cluster centroid (mean row/column) and geometry indices.
    StPxlHit(float meanRow, float meanColumn, unsigned int sector, unsigned int ladder,
             unsigned int sensor);
    /// @brief Destructor.
    ~StPxlHit();

    /// @brief Returns the detector identifier (kPxlId).
    StDetectorId detector() const;

    /// @brief Returns the sector number (1–10).
    unsigned int sector() const;
    /// @brief Returns the ladder number within the sector (1–4).
    unsigned int ladder() const;
    /// @brief Returns the sensor number within the ladder (1–10).
    unsigned int sensor() const;
    /// @brief Returns the mean cluster row in pixel coordinates.
    float meanRow() const;
    /// @brief Returns the mean cluster column in pixel coordinates.
    float meanColumn() const;
    /// @brief Returns the number of raw hits (pixels) forming this cluster.
    unsigned int nRawHits() const;
    /// @brief Returns the PXL layer number (1 for inner, 2 for outer ladders).
    unsigned int layer() const ;

    /// @brief Returns the local hit coordinate along the given axis (0=u, 1=v, 2=transverse).
    float localPosition(unsigned int) const;
    /// @brief Returns a pointer to the three-element local position array (u, v, transverse).
    const float* localPosition() const;
    /// @brief Sets the local hit position (u, v, transverse) within the sensor.
    void  setLocalPosition(float, float, float);
    /// @brief Sets the local v-coordinate (sensor normal direction) of the hit.
    void  setLocalY(float y);

    /// @brief Sets the sector number.
    void setSector(unsigned char);
    /// @brief Sets the ladder number.
    void setLadder(unsigned char);
    /// @brief Sets the sensor number.
    void setSensor(unsigned char);
    /// @brief Sets the detector identifier.
    void setDetectorId(StDetectorId);
    /// @brief Sets the mean cluster row.
    void setMeanRow(float);
    /// @brief Sets the mean cluster column.
    void setMeanColumn(float);
    /// @brief Sets the number of raw hits (cluster size).
    void setNRawHits(unsigned char);

    /// @brief Returns true; PXL hits are sortable by hardware position.
    virtual bool isSortable() const;
    
    void* operator new(size_t sz,void *p)     { return p;}
    void* operator new(size_t)     { return mPool.alloc(); }
    void  operator delete(void* p) { mPool.free(p); }
    
protected:
    UChar_t mSector;         ///< Sector number (1–10).
    UChar_t mLadder;         ///< Ladder number within the sector (1–4).
    UChar_t mSensor;         ///< Sensor number within the ladder (1–10).
    Float_t mMeanRow;        ///< Mean row of the PXL cluster in pixel coordinates.
    Float_t mMeanColumn;     ///< Mean column of the PXL cluster in pixel coordinates.
    UChar_t mNRawHits;       ///< Number of raw pixel hits (cluster size).
    /// @brief Local hit position within the sensor wafer: (u, v, transverse).
    Float_t mLocalPosition[3];
    
    // this has to go once the playing and testing is over.
    // should be hard wired in member function.
    StDetectorId mDetectorId; ///< Detector identifier.
    
    static StMemoryPool mPool;  //!
    
    ClassDef(StPxlHit,1)
};

ostream& operator<<(ostream&, const StPxlHit&);

inline unsigned int StPxlHit::sector() const {return mSector;}
inline unsigned int StPxlHit::ladder() const {return mLadder;}
inline unsigned int StPxlHit::sensor() const {return mSensor;}
inline float StPxlHit::meanRow() const {return mMeanRow;}
inline float StPxlHit::meanColumn()  const {return mMeanColumn;}
inline unsigned int StPxlHit::nRawHits() const {return mNRawHits;}
inline unsigned int StPxlHit::layer() const {return (mLadder==1)? 1 : 2;}
inline const Float_t* StPxlHit::localPosition() const { return mLocalPosition; }
inline void StPxlHit::setLocalY(float y) { mLocalPosition[1] = y; }
inline void StPxlHit::setSector(unsigned char v) {mSector = v;}
inline void StPxlHit::setLadder(unsigned char v) {mLadder = v;}
inline void StPxlHit::setSensor(unsigned char v) {mSensor = v;}
inline void StPxlHit::setMeanRow(float v) {mMeanRow = v;}
inline void StPxlHit::setMeanColumn(float v) {mMeanColumn = v;}
inline void StPxlHit::setNRawHits(unsigned char v) {mNRawHits = v;}

inline bool StPxlHit::isSortable() const { return true; }

#endif
