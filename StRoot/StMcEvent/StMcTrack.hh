/*!
 * \class StMcTrack
 * \brief Monte Carlo Track class
 * \author Manuel Calderon de la Barca Sanchez
 * \date   July 1999
 *
 ***************************************************************************
 *
 * $Id: StMcTrack.hh,v 2.30 2018/03/15 22:00:35 smirnovd Exp $
 *
 ***************************************************************************
 *
 * $Log: StMcTrack.hh,v $
 * Revision 2.30  2018/03/15 22:00:35  smirnovd
 * Fix linker error by removing declared but undefined functions
 *
 * Revision 2.29  2013/03/25 23:47:08  perev
 * Mustafa.Pxl corrs
 *
 * Revision 2.28  2012/03/22 01:03:21  perev
 * Etr add
 *
 * Revision 2.27  2011/10/11 01:22:24  perev
 * Not used anymore or ever
 *
 * Revision 2.26  2011/07/20 17:36:09  perev
 * Fsc added
 *
 * Revision 2.25  2011/01/26 19:46:24  perev
 * FPD ==> STAR Soft
 *
 * Revision 2.24  2007/10/16 19:49:20  fisyak
 * rename Hft => Pxl, remove Hpd, Igt and Fst
 *
 * Revision 2.23  2006/09/25 14:20:43  fisyak
 * Add Hpd Hits
 *
 * Revision 2.21  2005/11/22 21:44:52  fisyak
 * Add compress Print for McEvent, add Ssd collections
 *
 * Revision 2.20  2005/09/28 21:30:15  fisyak
 * Persistent StMcEvent
 *
 * Revision 2.19  2005/07/07 18:20:49  calderon
 * Added support for IGT detector.
 *
 * Revision 2.18  2005/05/27 23:37:25  calderon
 * Update for EEMC, add eprs, esmdu esdmv hits to StMcEvent.
 *
 * Revision 2.17  2005/04/18 20:11:33  calderon
 * Addition of Fgt and Fst files.  Modified other files to accomodate changes.
 *
 * Revision 2.16  2005/01/27 23:40:49  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.15  2004/09/14 05:00:30  calderon
 * Added support for Ist, Ssd and changes to Pixel, from "El Kai".
 *
 * Revision 2.14  2004/01/13 21:03:34  fisyak
 * Replace iostream by Stiostream.h (for icc)
 *
 * Revision 2.13  2003/12/04 05:56:47  calderon
 * Inclusion of Endcap EMC hit collection in StMcEvent and
 * of the Endcap hit vector in StMcTrack.
 * fix const of StMcVertex::parent() to avoid warnings in user code
 *
 * Revision 2.12  2003/10/08 20:17:55  calderon
 * -using <iostream>, std::cout, std::ostream.
 * -changes in FTPC volume Id.
 *   o Causes changes in decoding of plane().
 *   o sector() is added.
 *   o print volumeId and sector() in the operator<<.
 *
 * Revision 2.11  2003/09/02 17:58:41  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.10  2003/08/20 18:50:21  calderon
 * Addition of Tof classes and Pixel classes.  Modified track, event, and
 * container code to reflect this.
 * Fix bug in StMcVertex and in clearing of some hit collections.
 *
 * Revision 2.9  2003/02/19 03:16:05  calderon
 * Introduction of Ctb Hit Class and Ctb Hit Collection class, modified
 * StMcTrack, and StMcEvent accordingly.  Clearing of hits in StMcSvtWaferHitCollection.
 *
 * Revision 2.8  2000/06/06 02:58:42  calderon
 * Introduction of Calorimeter classes.  Modified several classes
 * accordingly.
 *
 * Revision 2.7  2000/04/06 23:29:10  calderon
 * Store the parent track for all tracks.
 *
 * Revision 2.6  2000/04/06 08:34:56  calderon
 * Version using the particle table:
 * 1) Constructor for particle_st*
 * 2) Pointer to parent track from particle table
 * 3) PDG encoding when track is from particle table
 * 4) Generator label, used to index entries in the table for debugging
 *
 * Revision 2.5  2000/04/04 22:25:24  calderon
 * add inline function to return primary key from g2t table
 *
 * Revision 2.4  2000/03/06 18:05:23  calderon
 * 1) Modified SVT Hits storage scheme from layer-ladder-wafer to
 * barrel-ladder-wafer.
 * 2) Added Rich Hit class and collection, and links to them in other
 * classes.
 *
 * Revision 2.3  2000/01/18 20:52:31  calderon
 * Works with CC5
 *
 * Revision 2.2  1999/12/03 00:51:53  calderon
 * Tested with new StMcEventMaker.  Added messages for
 * diagnostics.
 *
 * Revision 2.1  1999/11/19 19:06:34  calderon
 * Recommit after redoing the files.
 *
 * Revision 2.0  1999/11/17 02:12:16  calderon
 * Completely revised for new StEvent
 *
 * Revision 1.4  1999/09/24 01:23:17  fisyak
 * Reduced Include Path
 *
 * Revision 1.3  1999/09/23 21:25:54  calderon
 * Added Log & Id
 * Modified includes according to Yuri
 *
 *
 **************************************************************************/
/// @file StMcTrack.hh
/// @brief Monte Carlo track representing a simulated particle trajectory.
#ifndef StMcTrack_hh
#define StMcTrack_hh 
#include "Stiostream.h"
#include "StLorentzVectorF.hh"
#include "StMcContainers.hh"
#include "StObject.h"
#include "StEnumerations.h"
class StParticleDefinition;
class g2t_track_st;
class particle_st;
class StMcTrack;
ostream&  operator<<(ostream& os, const StMcTrack&);

class StMcTrack : public StObject {
public:
    /// @brief Default constructor.
    StMcTrack();
    virtual ~StMcTrack();
    /// @brief Construct from a GEANT g2t_track table row.
    StMcTrack(g2t_track_st* trk);
    /// @brief Construct from a generator particle table row.
    StMcTrack(particle_st* trk);
    
    // StMcTrack(const StMcTrack&);                     use default
    // const StMcTrack & operator=(const StMcTrack&);   use default
    
    int operator==(const StMcTrack&) const;
    int operator!=(const StMcTrack&) const;

    void initToZero();
  // "Get" Methods
  /// @brief Return the 4-momentum (px, py, pz, E) of the track.
  const StLorentzVectorF& fourMomentum() const { return mFourMomentum; }
  /// @brief Return the 3-momentum vector of the track.
  const StThreeVectorF& momentum() const { return mFourMomentum.vect(); }
  /// @brief Return the energy of the track (GeV).
  float energy() const { return mFourMomentum.e(); }
  /// @brief Return the transverse momentum pT (GeV/c).
  float pt() const { return mFourMomentum.perp(); }
  /// @brief Return the rapidity of the track.
  float rapidity() const { return mFourMomentum.rapidity(); }
  /// @brief Return the pseudo-rapidity (eta) of the track.
  float pseudoRapidity() const { return mFourMomentum.pseudoRapidity(); }
  /// @brief Return the vertex where this track was created.
  StMcVertex* startVertex() { return mStartVertex; }
  /// @brief Return the vertex where this track was created (const).
  const StMcVertex* startVertex() const { return mStartVertex; }
  /// @brief Return the vertex where this track stopped (nullptr if it left the detector).
  StMcVertex* stopVertex() { return mStopVertex; }
  /// @brief Return the stop vertex (const).
  const StMcVertex* stopVertex() const { return mStopVertex; }
  /// @brief Return the parent track that produced this track (nullptr for primary tracks).
  StMcTrack* parent() { return mParent; }
  /// @brief Return the parent track (const).
  const StMcTrack* parent() const { return mParent; }
  /// @brief Return the list of intermediate decay/scatter vertices along the track.
  StPtrVecMcVertex& intermediateVertices() { return mIntermediateVertices; }
  /// @brief Return the list of intermediate vertices (const).
  const StPtrVecMcVertex& intermediateVertices() const { return mIntermediateVertices; }
  /// @brief Return the list of TPC hits produced by this track.
  StPtrVecMcTpcHit& tpcHits() { return mTpcHits; }
  const StPtrVecMcTpcHit& tpcHits() const { return mTpcHits; }
  /// @brief Return the list of SVT hits produced by this track.
  StPtrVecMcSvtHit& svtHits() { return mSvtHits; }
  const StPtrVecMcSvtHit& svtHits() const { return mSvtHits; }
  /// @brief Return the list of SSD hits produced by this track.
  StPtrVecMcSsdHit& ssdHits() { return mSsdHits; }
  const StPtrVecMcSsdHit& ssdHits() const { return mSsdHits; }
  /// @brief Return the list of FTPC hits produced by this track.
  StPtrVecMcFtpcHit& ftpcHits() { return mFtpcHits; }
  const StPtrVecMcFtpcHit& ftpcHits() const { return mFtpcHits; }
  /// @brief Return the list of RICH hits produced by this track.
  StPtrVecMcRichHit& richHits() { return mRichHits; }
  const StPtrVecMcRichHit& richHits() const { return mRichHits; }
  /// @brief Return the list of CTB hits produced by this track.
  StPtrVecMcCtbHit& ctbHits() { return mCtbHits; }
  const StPtrVecMcCtbHit& ctbHits() const { return mCtbHits; }
  /// @brief Return the list of Barrel EMC hits produced by this track.
  StPtrVecMcCalorimeterHit& bemcHits() { return mBemcHits; }
  const StPtrVecMcCalorimeterHit& bemcHits() const { return mBemcHits; }
  StPtrVecMcCalorimeterHit& bprsHits() { return mBprsHits; }
  const StPtrVecMcCalorimeterHit& bprsHits() const { return mBprsHits; }
  StPtrVecMcCalorimeterHit& bsmdeHits() { return mBsmdeHits; }
  const StPtrVecMcCalorimeterHit& bsmdeHits() const { return mBsmdeHits; }
  StPtrVecMcCalorimeterHit& bsmdpHits() { return mBsmdpHits; }
  const StPtrVecMcCalorimeterHit& bsmdpHits() const { return mBsmdpHits; }
  /// @brief Return the list of TOF hits produced by this track.
  StPtrVecMcTofHit& tofHits() { return mTofHits; }
  const StPtrVecMcTofHit& tofHits() const { return mTofHits; }
  /// @brief Return the list of MTD hits produced by this track.
  StPtrVecMcMtdHit& mtdHits() { return mMtdHits; }
  const StPtrVecMcMtdHit& mtdHits() const { return mMtdHits; }
  /// @brief Return the list of Endcap EMC hits produced by this track.
  StPtrVecMcCalorimeterHit& eemcHits() { return mEemcHits; }
  const StPtrVecMcCalorimeterHit& eemcHits() const { return mEemcHits; }
  StPtrVecMcCalorimeterHit& eprsHits() { return mEprsHits; }
  const StPtrVecMcCalorimeterHit& eprsHits() const { return mEprsHits; }
  StPtrVecMcCalorimeterHit& esmduHits() { return mEsmduHits; }
  const StPtrVecMcCalorimeterHit& esmduHits() const { return mEsmduHits; }
  StPtrVecMcCalorimeterHit& esmdvHits() { return mEsmdvHits; }
  const StPtrVecMcCalorimeterHit& esmdvHits() const { return mEsmdvHits; }
  StPtrVecMcCalorimeterHit& fpdHits() { return mFpdHits; }
  const StPtrVecMcCalorimeterHit& fpdHits() const { return mFpdHits; }
  StPtrVecMcCalorimeterHit& fscHits() { return mFscHits; }
  const StPtrVecMcCalorimeterHit& fscHits() const { return mFscHits; }
  /// @brief Return the list of Pixel detector hits produced by this track.
  StPtrVecMcPxlHit& pxlHits() { return mPxlHits; }
  const StPtrVecMcPxlHit& pxlHits() const { return mPxlHits; }
  /// @brief Return the list of IST hits produced by this track.
  StPtrVecMcIstHit& istHits() { return mIstHits; }
  const StPtrVecMcIstHit& istHits() const { return mIstHits; }
  /// @brief Return the list of FGT hits produced by this track.
  StPtrVecMcFgtHit& fgtHits() { return mFgtHits; }
  const StPtrVecMcFgtHit& fgtHits() const { return mFgtHits; }
  /// @brief Return the list of ETR hits produced by this track.
  StPtrVecMcEtrHit& etrHits() { return mEtrHits; }
  const StPtrVecMcEtrHit& etrHits() const { return mEtrHits; }
  /// @brief Return the particle definition (type) for this track; loads from DB if needed.
  StParticleDefinition* particleDefinition();
  const StParticleDefinition* particleDefinition() const { return mParticleDefinition; }
  /// @brief Return all hits for a given detector ID; returns nullptr if unsupported.
  const StPtrVecMcHit *Hits(StDetectorId Id) const;
  /// @brief Return calorimeter hits for a given detector ID; returns nullptr if unsupported.
  const StPtrVecMcCalorimeterHit *CalorimeterHits(StDetectorId Id) const;
  /// @brief Return non-zero if this track belongs to an EM/hadronic shower.
  int isShower() const { return mIsShower; }
  /// @brief Return the GEANT particle ID for this track.
  long geantId() const { return mGeantId; }
  /// @brief Return the PDG particle encoding for this track.
  long pdgId() const { return mPdgId; }
  /// @brief Return the primary key (g2t row index) of this track.
  long key() const { return mKey; }
  /// @brief Return the event-generator label used to index the generator particle table.
  long eventGenLabel() const { return mEventGenLabel; }

  // "Set" Methods
    /// @brief Set the 4-momentum of this track.
    void setFourMomentum(const StLorentzVectorF&); 
    /// @brief Set the start (creation) vertex.
    void setStartVertex(StMcVertex*); 
    /// @brief Set the stop (termination) vertex.
    void setStopVertex(StMcVertex*); 
    void setIntermediateVertices(StPtrVecMcVertex&); 
    void setTpcHits(StPtrVecMcTpcHit&); 
    void setSvtHits(StPtrVecMcSvtHit&); 
    void setSsdHits(StPtrVecMcSsdHit&); 
    void setFtpcHits(StPtrVecMcFtpcHit&); 
    void setRichHits(StPtrVecMcRichHit&); 
    void setCtbHits(StPtrVecMcCtbHit&); 
    void setBemcHits(StPtrVecMcCalorimeterHit&); 
    void setBprsHits(StPtrVecMcCalorimeterHit&); 
    void setBsmdeHits(StPtrVecMcCalorimeterHit&); 
    void setBsmdpHits(StPtrVecMcCalorimeterHit&); 
    void setTofHits(StPtrVecMcTofHit&); 
    void setMtdHits(StPtrVecMcMtdHit&); 
    void setEemcHits(StPtrVecMcCalorimeterHit&); 
    void setEprsHits(StPtrVecMcCalorimeterHit&); 
    void setEsmduHits(StPtrVecMcCalorimeterHit&); 
    void setEsmdvHits(StPtrVecMcCalorimeterHit&); 
    void setFscHits(StPtrVecMcCalorimeterHit&); 
    void setPxlHits(StPtrVecMcPxlHit&); 
    void setIstHits(StPtrVecMcIstHit&); 
    void setFgtHits(StPtrVecMcFgtHit&); 
    void setEtrHits(StPtrVecMcEtrHit&); 

    /// @brief Set the shower flag (non-zero if the track belongs to a shower).
    void setShower(char); 
    /// @brief Set the GEANT particle ID.
    void setGeantId(long); 
    /// @brief Set the PDG particle encoding.
    void setPdgId(long); 
    /// @brief Set the g2t primary key for this track.
    void setKey(long);     
    /// @brief Set the event-generator label for this track.
    void setEventGenLabel(long);     
    /// @brief Set the parent track pointer.
    void setParent(StMcTrack*);     
  /// @brief Mark the track as primary (produced at the collision vertex).
  void setPrimary(Bool_t val) {mIsPrimary = val;}

    void addTpcHit(StMcTpcHit*); 
    void addSvtHit(StMcSvtHit*); 
    void addSsdHit(StMcSsdHit*); 
    void addFtpcHit(StMcFtpcHit*); 
    void addRichHit(StMcRichHit*); 
    void addCtbHit(StMcCtbHit*); 
    void addBemcHit(StMcCalorimeterHit*); 
    void addBprsHit(StMcCalorimeterHit*); 
    void addBsmdeHit(StMcCalorimeterHit*); 
    void addBsmdpHit(StMcCalorimeterHit*); 
    void addTofHit(StMcTofHit*); 
    void addMtdHit(StMcMtdHit*); 
    void addEemcHit(StMcCalorimeterHit*); 
    void addEprsHit(StMcCalorimeterHit*); 
    void addEsmduHit(StMcCalorimeterHit*); 
    void addEsmdvHit(StMcCalorimeterHit*); 
    void addFpdHit(StMcCalorimeterHit*);
    void addFscHit(StMcCalorimeterHit*);
    void addPxlHit(StMcPxlHit*); 
    void addIstHit(StMcIstHit*);
    void addFgtHit(StMcFgtHit*); 
    void addEtrHit(StMcEtrHit*); 
    void removeTpcHit(StMcTpcHit*); 
    void removeSvtHit(StMcSvtHit*); 
    void removeSsdHit(StMcSsdHit*); 
    void removeFtpcHit(StMcFtpcHit*); 
    void removeRichHit(StMcRichHit*); 
    void removeCtbHit(StMcCtbHit*); 
    void removeCalorimeterHit(StPtrVecMcCalorimeterHit&, StMcCalorimeterHit*); 
    void removeBemcHit(StMcCalorimeterHit*); 
    void removeBprsHit(StMcCalorimeterHit*); 
    void removeBsmdeHit(StMcCalorimeterHit*); 
    void removeBsmdpHit(StMcCalorimeterHit*); 
    void removeTofHit(StMcTofHit*); 
    void removeMtdHit(StMcMtdHit*); 
    void removeEemcHit(StMcCalorimeterHit*); 
    void removeEprsHit(StMcCalorimeterHit*); 
    void removeEsmduHit(StMcCalorimeterHit*); 
    void removeEsmdvHit(StMcCalorimeterHit*); 
    void removePxlHit(StMcPxlHit*); 
    void removeIstHit(StMcIstHit*); 
    void removeFgtHit(StMcFgtHit*); 
    void removeEtrHit(StMcEtrHit*); 
  void Print(Option_t *option="") const;
  /// @brief Return true if this is a primary track.
  Bool_t IsPrimary() const {return mIsPrimary;}
    //    void setTopologyMap(StTrackTopologyMap&); 
    
protected:
    StLorentzVectorF         mFourMomentum;              ///< 4-momentum (px, py, pz, E)
    StMcVertex*              mStartVertex;               ///< Vertex where the track was created
    StMcVertex*              mStopVertex;                ///< Vertex where the track stopped
    StPtrVecMcVertex         mIntermediateVertices;      ///< Intermediate scatter/decay vertices
    StPtrVecMcTpcHit         mTpcHits;                  ///< TPC hits from this track
    StPtrVecMcSvtHit         mSvtHits;                  ///< SVT hits from this track
    StPtrVecMcSsdHit         mSsdHits;                  ///< SSD hits from this track
    StPtrVecMcFtpcHit        mFtpcHits;                 ///< FTPC hits from this track
    StPtrVecMcRichHit        mRichHits;                 ///< RICH hits from this track
    StPtrVecMcCtbHit         mCtbHits;                  ///< CTB hits from this track
    StPtrVecMcCalorimeterHit mBemcHits;                 ///< Barrel EMC hits from this track
    StPtrVecMcCalorimeterHit mBprsHits;                 ///< Barrel PRS hits from this track
    StPtrVecMcCalorimeterHit mBsmdeHits;                ///< Barrel SMD-eta hits from this track
    StPtrVecMcCalorimeterHit mBsmdpHits;                ///< Barrel SMD-phi hits from this track
    StPtrVecMcTofHit         mTofHits;                  ///< TOF hits from this track
    StPtrVecMcMtdHit         mMtdHits;                  ///< MTD hits from this track
    StPtrVecMcCalorimeterHit mEemcHits;                 ///< Endcap EMC hits from this track
    StPtrVecMcCalorimeterHit mEprsHits;                 ///< Endcap PRS hits from this track
    StPtrVecMcCalorimeterHit mEsmduHits;                ///< Endcap SMD-U hits from this track
    StPtrVecMcCalorimeterHit mEsmdvHits;                ///< Endcap SMD-V hits from this track
    StPtrVecMcCalorimeterHit mFpdHits;                  ///< FPD hits from this track
    StPtrVecMcCalorimeterHit mFscHits;                  ///< FSC hits from this track
    StPtrVecMcPxlHit         mPxlHits;                  ///< Pixel detector hits from this track
    StPtrVecMcIstHit         mIstHits;                  ///< IST hits from this track
    StPtrVecMcFgtHit         mFgtHits;                  ///< FGT hits from this track
    StPtrVecMcEtrHit         mEtrHits;                  ///< ETR hits from this track
  StParticleDefinition*    mParticleDefinition; //!
    StMcTrack*               mParent;                   ///< Parent track (nullptr for primaries)
    char                     mIsShower;                 ///< Non-zero if track belongs to a shower
    long                     mGeantId;                  ///< GEANT particle ID
    long                     mPdgId;                    ///< PDG particle encoding
    long                     mKey;                      ///< Primary key (g2t row index)
    long                     mEventGenLabel;            ///< Event-generator index for debugging
  Bool_t                   mIsPrimary;                  ///< True if this is a primary track
    ClassDef(StMcTrack,2)
};
#endif




