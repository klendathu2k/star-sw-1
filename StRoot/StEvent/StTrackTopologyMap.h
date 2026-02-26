/*!
 * \class StTrackTopologyMap
 * \author Thomas Ullrich, AUg 1999
 */
/***************************************************************************
 *
 * $Id: StTrackTopologyMap.h,v 2.14 2018/03/27 02:41:00 genevb Exp $
 *
 * Author: Thomas Ullrich, AUg 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTrackTopologyMap.h,v $
 * Revision 2.14  2018/03/27 02:41:00  genevb
 * iTPC modifications, plus proper use of booleans
 *
 * Revision 2.13  2016/02/24 22:01:12  ullrich
 * Added method hasHitInSstLayer().
 *
 * Revision 2.12  2014/03/16 16:06:24  fisyak
 * Xin\'s fix for HFT
 *
 * Revision 2.11  2007/11/07 00:54:54  ullrich
 * Added PXL and IST.
 *
 * Revision 2.10  2005/06/23 19:04:24  ullrich
 * Added overloaded version of hasHitInDetector() taking up to 6 args.
 *
 * Revision 2.9  2003/09/02 17:58:06  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.8  2002/02/22 22:56:53  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.7  2001/04/24 21:32:07  genevb
 * Additional helper functions
 *
 * Revision 2.6  2001/04/05 04:00:46  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.5  2000/05/17 17:21:38  ullrich
 * New method largestGap() and new output operator.
 *
 * Revision 2.4  2000/04/12 19:44:03  genevb
 * Reimplement mMap data members as individual unsigned ints
 *
 * Revision 2.3  2000/04/10 19:59:33  genevb
 * StRoot/StEvent/doc/tex/
 *
 * Revision 2.2  1999/12/13 20:16:39  ullrich
 * Changed numbering scheme for hw_position unpack methods (STAR conventions).
 *
 * Revision 2.1  1999/10/13 19:44:17  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StTrackTopologyMap_hh
#define StTrackTopologyMap_hh
/// @file StTrackTopologyMap.h
/// @brief Compact bit-map encoding which detector layers a reconstructed track has hits in.

#include <Stiostream.h>
#include "StObject.h"
#include "StEnumerations.h"

/// @brief Compact bit-map encoding which detector layers a reconstructed track has hits in.
///
/// The map is stored in two 32-bit words (mMap0, mMap1) covering TPC, SVT, FTPC,
/// SSD, CTB, TOF, RICH, BEMC, EEMC, and other detectors, plus a 64-bit word
/// (mMap_iTpc) for the inner TPC (iTPC) upgrade.  Bit-layout conventions follow
/// the STAR topology-map definition document.
class StTrackTopologyMap : public StObject {
public:
    /// @brief Default constructor; clears all bits.
    StTrackTopologyMap();
    /// @brief Construct from two 32-bit words and an optional iTPC 64-bit word.
    StTrackTopologyMap(unsigned int, unsigned int, unsigned long long = 0);
    /// @brief Construct from an array of unsigned long values and an optional iTPC word.
    StTrackTopologyMap(const unsigned long*, unsigned long long = 0);
    /// @brief Construct from an array of unsigned int values and an optional iTPC word.
    StTrackTopologyMap(const unsigned int*, unsigned long long = 0);
    // StTrackTopologyMap(const StTrackTopologyMap&);            use default
    // StTrackTopologyMap& operator=(const StTrackTopologyMap&); use default
    ~StTrackTopologyMap();  
    
    /// @brief True if the primary vertex was used in the track fit.
    bool           primaryVertexUsed() const;
    /// @brief Number of hits in the specified detector encoded in the map.
    unsigned int   numberOfHits(StDetectorId) const;
    /// @brief True if the track has at least one hit in the specified detector.
    bool           hasHitInDetector(StDetectorId) const;
    /// @brief True if the track has a hit in any of the specified detectors (up to 6).
    bool           hasHitInDetector(StDetectorId, StDetectorId,
                                    StDetectorId = kUnknownId, StDetectorId = kUnknownId,
                                    StDetectorId = kUnknownId, StDetectorId = kUnknownId) const;
    /// @brief True if the track has a hit in the given row of the specified detector (first row = 1).
    bool           hasHitInRow(StDetectorId, unsigned int) const;
    /// @brief True if the track has a hit in the given SVT layer (first layer = 1).
    bool           hasHitInSvtLayer(unsigned int) const;
    /// @brief True if the track has a hit in the given PXL layer (first layer = 1).
    bool           hasHitInPxlLayer(unsigned int) const;
    /// @brief True if the track has a hit in the given IST layer (first layer = 1).
    bool           hasHitInIstLayer(unsigned int) const;
    /// @brief True if the track has a hit in the given SSD layer (first layer = 1).
    bool           hasHitInSsdLayer(unsigned int) const;
    /// @brief True if the track has a hit in the given SST layer (alias for SSD layer).
    bool           hasHitInSstLayer(unsigned int) const;
    /// @brief True if the track has an MWPC (TPC prompt) hit.
    bool           hasHitInMwpc() const;
    /// @brief True if the track has a TPC prompt hit (alias for MWPC).
    bool           hasHitInTpcPrompt() const;
    /// @brief True if the track has a CTB hit.
    bool           hasHitInCtb() const;
    /// @brief True if the track has a TOF hit.
    bool           hasHitInTof() const;
    /// @brief True if the track has a RICH hit.
    bool           hasHitInRich() const;
    /// @brief True if the track has a BEMC hit.
    bool           hasHitInBemc() const;
    /// @brief True if the track has an EEMC hit.
    bool           hasHitInEemc() const;
    /// @brief True if the track has post-crossing TPC hits.
    bool           postXTrack() const;
    /// @brief True if the track crosses the TPC central membrane.
    bool           membraneCrossingTrack() const;
    
    /// @brief True if the track was reconstructed in the TPC only.
    bool           trackTpcOnly() const;
    /// @brief True if the track was reconstructed in the SVT only.
    bool           trackSvtOnly() const;
    /// @brief True if the track was reconstructed using SVT and TPC hits.
    bool           trackTpcSvt() const;
    /// @brief True if the track was reconstructed in the FTPC east.
    bool           trackFtpcEast() const;
    /// @brief True if the track was reconstructed in the FTPC west.
    bool           trackFtpcWest() const;
    /// @brief True if the track was reconstructed in either FTPC.
    bool           trackFtpc() const;
    
    /// @brief True if the track has a turn-around point (spiral track).
    bool           turnAroundFlag() const;
    /// @brief Raw 64-bit topology map word by index (0 = mMap0/mMap1 combined, 1 = iTPC word).
    unsigned long long  data(unsigned int) const;
    
    /// @brief Largest gap (in rows) without a hit for the specified detector.
    int            largestGap(StDetectorId) const;
    
protected:
    /// @brief Test a single bit in the 64-bit combined TPC map (range 0–63).
    bool bit(int) const;
    /// @brief Test a single bit in the iTPC 64-bit map (range 0–63).
    bool iTpcBit(int) const;
    /// @brief True if the map uses the legacy FTPC encoding format.
    bool ftpcFormat() const;
    /// @brief True if the map uses the HFT hit encoding format (Run 13+).
    bool hftFormat() const;
    
private:
    UInt_t     mMap0;      ///< First 32-bit word of the topology bit-map (TPC rows 1–32, SVT layers, etc.).
    UInt_t     mMap1;      ///< Second 32-bit word of the topology bit-map (TPC rows 33–45, FTPC, fast detectors).
    ULong64_t  mMap_iTpc;  ///< 64-bit word encoding iTPC (inner TPC) hit pattern.
    
    ClassDef(StTrackTopologyMap,2)
};

ostream& operator<< (ostream&, const StTrackTopologyMap&);

inline bool StTrackTopologyMap::hasHitInSstLayer(unsigned int val) const  
{
    return hasHitInSsdLayer(val);
}

inline bool StTrackTopologyMap::hasHitInTpcPrompt() const
{
    return hasHitInMwpc();
}

#endif
