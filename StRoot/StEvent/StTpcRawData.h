/***************************************************************************
 *
 * $Id: StTpcRawData.h,v 2.14 2018/09/27 22:01:24 ullrich Exp $
 *
 * Author: Yuri Fisyak, Mar 2008
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTpcRawData.h,v $
 * Revision 2.14  2018/09/27 22:01:24  ullrich
 * Added missing inheritance, SObject, for StDigitalPair
 *
 * Revision 2.13  2018/09/27 20:03:33  ullrich
 * Added ClassDef for StDigitalPair
 *
 * Revision 2.12  2018/04/05 03:16:20  smirnovd
 * Make StTpcDigitalSector compatible with iTPC
 *
 * Revision 2.11  2018/02/18 23:18:45  perev
 * Remove iTPC related update
 *
 * Revision 2.9  2012/05/07 14:41:59  fisyak
 * Remove hardcoded separation between Inner and Outer Sectors
 *
 * Revision 2.8  2011/03/31 19:27:47  fisyak
 * Add more safety for work with pixel data
 *
 * Revision 2.7  2009/11/23 22:20:51  ullrich
 * Minor cleanup performed, fixed compiler warnings.
 *
 * Revision 2.6  2009/10/12 23:52:32  fisyak
 * Fix relation npad from pad row
 *
 * Revision 2.5  2008/07/31 20:47:27  fisyak
 * Modify operator += and =
 *
 * Revision 2.4  2008/06/20 14:56:34  fisyak
 * Add protection for pad no.
 *
 * Revision 2.3  2008/05/27 14:40:08  fisyak
 * keep pixel raw data as short istead of uchar
 *
 * Revision 2.2  2008/04/24 16:06:25  fisyak
 * Clean up before next move
 *
 * Revision 2.1  2008/03/13 16:42:24  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StTpcRawData_h
#define StTpcRawData_h

/// @file StTpcRawData.h
/// @brief Containers for raw TPC ADC data organised by sector, padrow, pad, and time bin.

#include "StObject.h"
#include <vector>
#include <utility>
#include "StSequence.hh"
#include "StTpcPixel.h"
#include "StDetectorDbMaker/St_tpcPadPlanesC.h"
#include "StDetectorDbMaker/St_tpcPadConfigC.h"
#define __MaxNumberOfTimeBins__ 512
typedef std::vector<Short_t>  StVectorADC;
typedef std::vector<UShort_t> StVectorIDT;

/// @brief A contiguous sequence of ADC samples for one pad starting at a given time bin.
///
/// Stores raw ADC values (and optional MC truth IDs) for a run of consecutive time bins.
class StDigitalPair : public StObject {
public:
    /// @brief Construct a digital pair starting at the given time bin.
    StDigitalPair(UShort_t time=0)      {mTime=time;}
    /// @brief Destructor.
    virtual ~StDigitalPair() {}
    /// @brief Appends an ADC sample to the sequence.
    void add(Short_t adc)               {mAdc.push_back(adc);}
    /// @brief Appends an ADC sample together with its MC truth track ID.
    void add(Short_t adc,Int_t idt)     {mAdc.push_back(adc); mIdt.push_back(idt);}

    /// @brief Returns a pointer to the first ADC sample in the sequence.
    Short_t* adc()   const {return (Short_t*)&mAdc[0];}
    /// @brief Returns true when MC truth IDs are available (one per ADC sample).
    Bool_t   isIdt() const {return mAdc.size() == mIdt.size();}
    /// @brief Returns a pointer to the MC truth ID array, or null if not available.
    UShort_t*idt()   const {return (UShort_t*) (isIdt() ? &mIdt[0] : 0);}
    /// @brief Returns the number of ADC samples in this sequence.
    Int_t    size()  const {return mAdc.size();}
    /// @brief Returns the starting time-bin index of this sequence.
    UShort_t time()  const {return mTime;}

private:
    UShort_t    mTime;     ///< Starting time-bin index of the ADC sequence.
    StVectorADC mAdc;      ///< Vector of ADC samples for consecutive time bins.
    StVectorIDT mIdt;      ///< Vector of MC truth track IDs, one per ADC sample (optional).
    ClassDef(StDigitalPair,1)
};

typedef std::vector<StDigitalPair>           StDigitalTimeBins;
typedef std::vector<StDigitalTimeBins>       StDigitalPadRow;
typedef std::vector<StDigitalPadRow>         StDigitalSector;

typedef std::vector<StDigitalPair>::iterator StDigitalTimeBinIterator;
typedef StDigitalTimeBins::iterator          StDigitalTimeBinsIterator;
typedef StDigitalPadRow::iterator            StDigitalPadRowIterator;
typedef StDigitalSector::iterator            StDigitalRowIterator;

typedef std::vector<StSequence>              StVecSequence;
typedef std::vector<UShort_t*>               StVecIds;
typedef std::vector<UChar_t>                 StVecPads;
typedef std::vector<UChar_t> 	               StVecUChar;
typedef std::vector<Int_t> 	               StVecInt;
typedef std::vector<StTpcPixel>              StVectPixel;

/// @brief Raw ADC data for a single TPC sector, organised as a 3-D array: padrow × pad × time-bin sequences.
class StTpcDigitalSector : public StObject {
public:
    /// @brief Construct a digital sector using pad-plane geometry from the database.
    StTpcDigitalSector(void *db = 0);
    /// @brief Construct a digital sector for the given sector number.
    StTpcDigitalSector(int sector);
    /// @brief Destructor.
    virtual ~StTpcDigitalSector() {}
    /// @brief Returns a const pointer to the time-bin sequence list for the given padrow and pad (1-based).
    const StDigitalTimeBins* timeBinsOfRowAndPad(Int_t rowN, Int_t padN) const { return (&mData[(rowN-1)][(padN-1)]);}
    /// @brief Returns a mutable pointer to the time-bin sequence list for the given padrow and pad (1-based).
    StDigitalTimeBins*       timeBinsOfRowAndPad(Int_t rowN, Int_t padN)       { return (&mData[(rowN-1)][(padN-1)]);}
    /// @brief Returns a mutable pointer to the pad-indexed data for the given padrow (1-based).
    StDigitalPadRow*         padsOfRow(Int_t rowN)                             { return (&mData[(rowN-1)]);}
    /// @brief Returns a mutable pointer to the full sector data array (indexed by padrow).
    StDigitalSector*         rows()                                            { return (&mData);}

    /// @brief Returns the number of padrows present in this sector's data.
    Int_t  numberOfRows()             		        const    { return mData.size();}
    /// @brief Returns the number of pads stored for the given padrow (1-based).
    Int_t  numberOfPadsInRow(Int_t rowN)		const    { return mData[(rowN-1)].size();}
    /// @brief Returns the number of time-bin sequences stored for the given padrow and pad (1-based).
    Int_t  numberOfTimeBins(Int_t rowN, Int_t padN) 	const    { return mData[(rowN-1)][(padN-1)].size();}

    /// @brief Assigns a pre-built time-bin sequence list to the specified padrow and pad (1-based).
    void   assignTimeBins(int row , int pad, StDigitalTimeBins*);
    /// @brief Returns the ADC sequences for the given padrow and pad as StSequence objects (for cluster finding).
    Int_t  getSequences(Int_t row, Int_t pad, Int_t *nSeq, StSequence** seq, UShort_t ***Id);
    /// @brief Fills @p padList with the pad numbers that have data in the given padrow; returns pad count.
    Int_t  getPadList(Int_t row, UChar_t **padList);
    /// @brief Fills ADC and MC truth ID arrays for the given pad; applies 8→10 bit ADC conversion.
    Int_t  getTimeAdc(Int_t row, Int_t pad, Short_t ADCs[__MaxNumberOfTimeBins__],
		      UShort_t IDTs[__MaxNumberOfTimeBins__]); // with  8 => 10 conversion
    /// @brief Fills 8-bit ADC and MC truth ID arrays for the given pad.
    Int_t  getTimeAdc(Int_t row, Int_t pad, UChar_t  ADCs[__MaxNumberOfTimeBins__],
		      UShort_t IDTs[__MaxNumberOfTimeBins__]);
    /// @brief Stores 16-bit ADC (and optional MC truth ID) data for the given pad; applies 10→8 bit compression.
    Int_t  putTimeAdc(Int_t row, Int_t pad, Short_t *ADCs, UShort_t *IDTs = 0);     // with 10 =>  8 conversion
    /// @brief Stores 8-bit ADC (and optional MC truth ID) data for the given pad.
    Int_t  putTimeAdc(Int_t row, Int_t pad, UChar_t  *ADCs, UShort_t *IDTs = 0);
    /// @brief Sets the sector number for this digital sector.
    void   setSector(Int_t sector) {mSector = sector;}
    /// @brief Clears all ADC data from this sector.
    void   clear();
    /// @brief Removes empty entries to reduce memory footprint; returns number of entries removed.
    Int_t  cleanup();
    /// @brief Prints a summary of the sector data.
    virtual void   Print(const Option_t *opt="") const;
    /// @brief Prints the ADC values for the given padrow and pad.
    virtual Int_t  PrintTimeAdc(Int_t row, Int_t pad) const;
    /// @brief Merges the ADC data from another sector into this one.
    StTpcDigitalSector &operator+= (StTpcDigitalSector& v);
    /// @brief Returns the number of pads in the given padrow for this sector (from database).
    Int_t numberOfPadsAtRow(Int_t row) {return (row > 0 && row <= mNoRows) ? St_tpcPadConfigC::instance()->padsPerRow(mSector, row) : 0;}
    /// @brief Assignment operator; copies ADC data from another sector.
    StTpcDigitalSector& operator=(const StTpcDigitalSector&);
    /// @brief Returns the sector number.
    Int_t sector() {return mSector;}
    /// @brief Returns the number of padrows configured for this sector.
    Int_t numberOfRows() {return mNoRows;}
private:
    StTpcDigitalSector(const StTpcDigitalSector&);

private:
    StDigitalSector       mData;      ///< 3-D ADC data: [padrow][pad][time-bin sequences].
    Int_t                 mSector;    ///< TPC sector number (1–24).
    StVecPads             mPadList;   ///< Scratch buffer for pad-list queries.
    StVecSequence         mSequence;  ///< Scratch buffer for sequence queries.
    StVecIds              mIds;       ///< Scratch buffer for MC truth ID queries.
    Int_t                 mNoRows;    ///< Number of padrows configured for this sector.
    ClassDef(StTpcDigitalSector,2)
};

/// @brief Top-level container for raw TPC ADC data from all sectors in one event.
class StTpcRawData : public StObject {
public:
    /// @brief Construct a raw-data container for the given number of TPC sectors (default 24).
    StTpcRawData(Int_t noSectors = 24) {setNoSectors(noSectors);}
    /// @brief Destructor; releases all owned StTpcDigitalSector objects.
    virtual ~StTpcRawData() {clear();}
    /// @brief Returns the number of sector slots in the container.
    UInt_t size() {return mSectors.size();}
    /// @brief Returns the number of sector slots in the container (alias for size()).
    UInt_t getNoSectors() {return size();}
    /// @brief Returns a pointer to the digital sector for the given sector number (1-based), or null.
    StTpcDigitalSector *GetSector(UInt_t sector) {return sector > 0 && sector <= size() ? mSectors[sector-1] : 0;}
    /// @brief Returns a pointer to the digital sector for the given sector number (1-based), or null.
    StTpcDigitalSector *getSector(UInt_t sector) {return GetSector(sector);}
    /// @brief Fills @p pixels with StTpcPixel objects from the specified sector, padrow, pad and time-bin range.
    Int_t  getVecOfPixels(StVectPixel &pixels, Int_t sector, Int_t row, Int_t padMin = 1, Int_t padMax = -1,
			  Int_t tMin = 0, Int_t tMax = -1);
    /// @brief Resizes the sector array and initialises all slots to null.
    void   setNoSectors(UInt_t noSectors = 0) {mSectors.resize(noSectors); for (UInt_t i = 0; i < noSectors; i++) mSectors[i] = 0;}
    /// @brief Stores a pointer to a digital sector at the given (1-based) sector slot.
    void   setSector(UInt_t sector, StTpcDigitalSector* digitSector);
    /// @brief Releases and deletes all owned digital sector objects.
    void   clear() {Clear();}
    /// @brief Releases and deletes all owned digital sector objects.
    void   Clear(const Option_t *opt = "");
    /// @brief Merges the ADC data from another StTpcRawData container into this one.
    StTpcRawData &operator+= (StTpcRawData& v);
    /// @brief Prints a summary of the raw data for all sectors.
    virtual void Print(const Option_t *opt="") const;
private:
    std::vector<StTpcDigitalSector*> mSectors; ///< Pointers to per-sector raw ADC data (size = number of sectors).
    ClassDef(StTpcRawData,1)
 };
#endif
