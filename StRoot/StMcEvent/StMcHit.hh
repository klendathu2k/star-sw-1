/***************************************************************************
 *
 * $Id: StMcHit.hh,v 2.14 2016/05/16 23:47:09 perev Exp $
 * $Log: StMcHit.hh,v $
 * Revision 2.14  2016/05/16 23:47:09  perev
 * Coverity fix
 *
 * Revision 2.13  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.12  2011/10/17 00:24:00  fisyak
 * Add time of flight for hits
 *
 * Revision 2.10  2005/11/22 21:44:52  fisyak
 * Add compress Print for McEvent, add Ssd collections
 *
 * Revision 2.9  2005/09/28 21:30:14  fisyak
 * Persistent StMcEvent
 *
 * Revision 2.8  2004/01/13 21:02:51  fisyak
 * Add inheritance from StObject
 *
 * Revision 2.7  2003/10/08 20:17:55  calderon
 * -using <iostream>, std::cout, std::ostream.
 * -changes in FTPC volume Id.
 *   o Causes changes in decoding of plane().
 *   o sector() is added.
 *   o print volumeId and sector() in the operator<<.
 *
 * Revision 2.6  2003/09/02 17:58:41  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.5  2000/06/06 02:58:41  calderon
 * Introduction of Calorimeter classes.  Modified several classes
 * accordingly.
 *
 * Revision 2.4  2000/05/05 15:25:43  calderon
 * Reduced dependencies and made constructors more efficient
 *
 * Revision 2.3  2000/04/17 23:01:15  calderon
 * Added local momentum to hits as per Lee's request
 *
 * Revision 2.2  1999/12/14 07:04:49  calderon
 * Numbering scheme as per SVT request.
 *
 * Revision 2.1  1999/11/19 19:06:33  calderon
 * Recommit after redoing the files.
 *
 * Revision 2.0  1999/11/17 02:12:16  calderon
 * Completely revised for new StEvent
 *
 * Revision 1.4  1999/09/24 01:23:16  fisyak
 * Reduced Include Path
 *
 * Revision 1.3  1999/09/23 21:25:51  calderon
 * Added Log & Id
 * Modified includes according to Yuri
 *
 *
 **************************************************************************/
/// @file StMcHit.hh
/// @brief Abstract base class for all Monte Carlo detector hits.
#ifndef StMcHit_hh
#define StMcHit_hh

#include "StObject.h"
#include "Stiostream.h"
#include "StThreeVectorF.hh"
#include "tables/St_g2t_hits_Table.h"
#include "StMcTrack.hh"

/// @brief Abstract base class for all Monte Carlo detector hits in STAR.
class StMcHit : public StObject {
public:
  /// @brief Bit flags for hit matching status.
  enum EMcHitBits {
    kMatched = BIT(23) ///< Set when this hit has been matched to a reconstructed hit
  };
  /// @brief Default constructor; initializes all fields to zero.
  StMcHit()  : mPosition(0,0,0), mLocalMomentum(0,0,0), mdE(0),mdS(0),mTof(0),mKey(0),mVolumeId(0),mParentTrack(0) {}
  /// @brief Construct with all hit properties specified explicitly.
  StMcHit(const StThreeVectorF& x,const StThreeVectorF& p,
	  Float_t de, Float_t ds, Float_t tof, Long_t k, Long_t volId, StMcTrack* parent=0)
    : mPosition(x), mLocalMomentum(p), mdE(de), mdS(ds), mTof(tof), mKey(k), mVolumeId(volId), mParentTrack(parent) {}
  /// @brief Construct from a GEANT g2t_hits table row.
  StMcHit(g2t_hits_st* pt) : mPosition(pt->x[0],pt->x[1],pt->x[2]), mLocalMomentum(pt->p[0],pt->p[1],pt->p[2]),
			     mdE(pt->de), mdS(pt->ds), mTof(pt->tof), mKey(pt->id), mVolumeId(0),
			     mParentTrack(0) {}
    // StMcHit(const StSvtHit&);                  use default
    // const StMcHit & operator=(const StMcHit&);   use default
  virtual ~StMcHit() {}
    
  Int_t operator==(const StMcHit& h) const;
  Int_t operator!=(const StMcHit& h) const {return !(*this == h); }
    

  // "Get" Methods
  /// @brief Return the global position of the hit (cm).
  virtual const StThreeVectorF&      position() const { return mPosition;}
  /// @brief Return the local momentum vector of the particle at the hit (GeV/c).
  virtual const StThreeVectorF& localMomentum() const { return mLocalMomentum;}
  /// @brief Return the energy deposited in the sensitive volume (GeV).
  virtual Float_t                            dE() const { return mdE; }
  /// @brief Return the step length through the sensitive volume (cm).
  virtual Float_t                            dS() const { return mdS; }
  /// @brief Return the global time of flight at the hit (ns).
  virtual Float_t                           tof() const { return mTof; }
  /// @brief Return the primary key (g2t row index) of this hit.
  virtual Long_t                            key() const { return mKey; }
  /// @brief Return the detector volume ID encoding (detector-specific).
  virtual Long_t                       volumeId() const { return mVolumeId; }
  /// @brief Return a pointer to the MC track that produced this hit.
  virtual StMcTrack*              parentTrack() const { return mParentTrack; }	
  // "Set" Methods

  /// @brief Set the global hit position.
  virtual void setPosition(const StThreeVectorF& val) { mPosition = val; }
  /// @brief Set the local momentum at the hit.
  virtual void setLocalMomentum(const StThreeVectorF& val) { mLocalMomentum = val; }
  /// @brief Set the energy deposited.
  virtual void setdE(Float_t val) 	{ mdE  = val;}
  /// @brief Set the step length.
  virtual void setdS(Float_t  val) 	{ mdS  = val;}
  /// @brief Set the time of flight.
  virtual void setTof(Float_t tof) 	{ mTof = tof;}
  /// @brief Set the g2t primary key.
  virtual void setKey(Long_t val) 	{ mKey = val;}
  /// @brief Set the detector volume ID.
  virtual void setVolumeId(Long_t val) 	{ mVolumeId = val; }
  /// @brief Set the parent MC track pointer.
  virtual void setParentTrack(StMcTrack* val) { mParentTrack = val; }
  virtual void Print(Option_t *option="") const; // *MENU* 
    
protected:
  StThreeVectorF       mPosition;       ///< Global hit position (cm)
  StThreeVectorF       mLocalMomentum;  ///< Local momentum at the hit (GeV/c)
  Float_t              mdE;             ///< Energy deposited in the volume (GeV)
  Float_t              mdS;             ///< Step length through the volume (cm)
  Float_t              mTof;            ///< Time of flight at the hit (ns)
  Long_t               mKey;            ///< Primary key (g2t row index)
  Long_t               mVolumeId;       ///< Detector volume ID (encoding is detector-specific)
  StMcTrack*           mParentTrack;    ///< Parent MC track that produced this hit
  ClassDef(StMcHit,2)
};
ostream&  operator<<(ostream& os, const StMcHit&);
#endif

