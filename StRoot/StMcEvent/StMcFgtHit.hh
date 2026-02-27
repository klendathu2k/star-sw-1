/***************************************************************************
 *
 * $Id: StMcFgtHit.hh,v 2.7 2011/10/17 00:24:00 fisyak Exp $
 * $Log: StMcFgtHit.hh,v $
 * Revision 2.7  2011/10/17 00:24:00  fisyak
 * Add time of flight for hits
 *
 * Revision 2.6  2009/10/13 19:14:27  perev
 * Wei-Ming update
 *
 * Revision 2.5  2005/11/22 21:44:51  fisyak
 * Add compress Print for McEvent, add Ssd collections
 *
 * Revision 2.4  2005/09/28 21:30:14  fisyak
 * Persistent StMcEvent
 *
 * Revision 2.3  2005/07/19 20:07:34  calderon
 * Addition of default constructor, including base class StMcHit constructor.
 * Bracket calls to StMemoryPool inside #ifdef.
 *
 * Revision 2.2  2005/07/06 20:05:28  calderon
 * Remove forward declaration of StThreeVectorF, use #include, and only in
 * StMcHit base class.  StThreeVectorF is not a class anymore, it is now
 * only a typedef, only template version of StThreeVector exists now.
 *
 * Revision 2.1  2005/04/18 20:11:33  calderon
 * Addition of Fgt and Fst files.  Modified other files to accomodate changes.
 *
 *
 **************************************************************************/
/// @file StMcFgtHit.hh
/// @brief Monte Carlo hit class for the Forward GEM Tracker (FGT).
#ifndef StMcFgtHit_hh
#define StMcFgtHit_hh

#include "StMcHit.hh"
#include "tables/St_g2t_fgt_hit_Table.h" 

/// @brief Monte Carlo hit class for the Forward GEM Tracker (FGT).
///
/// Inherits position, momentum, and track information from StMcHit.
/// The volume ID encodes the disc (layer), quadrant, and strip indices.
class StMcFgtHit : public StMcHit {
public:
  /// @brief Default constructor.
  StMcFgtHit() {}
  /// @brief Constructor from hit kinematics, energy, and volume ID.
  StMcFgtHit(const StThreeVectorF& x,const StThreeVectorF& p,
	     Float_t de = 0, Float_t ds = 0, Float_t tof = 0, Long_t k = 0, Long_t volId = 0, StMcTrack* parent=0) : 
    StMcHit(x,p,de,ds,tof,k,volId,parent) {}
  /// @brief Constructor from a GEANT g2t_fgt_hit table row.
  StMcFgtHit(g2t_fgt_hit_st* pt) : 
    StMcHit(StThreeVectorF(pt->x[0], pt->x[1], pt->x[2]),
	    StThreeVectorF(pt->p[0], pt->p[1], pt->p[2]), 
	    pt->de, pt->ds, pt->tof, pt->id, pt->volume_id, 0) {}
  /// @brief Destructor.
  ~StMcFgtHit() {}
  
  /// @brief Returns the disc (layer) index decoded from the volume ID.
  ULong_t layer() const;
  /// @brief Returns the quadrant index decoded from the volume ID.
  ULong_t quad() const;
  /// @brief Prints hit information to standard output (ROOT menu entry).
  virtual void Print(Option_t *option="") const; // *MENU* 
    
private:
    ClassDef(StMcFgtHit,2)
};
ostream&  operator<<(ostream& os, const StMcFgtHit&);
#endif
