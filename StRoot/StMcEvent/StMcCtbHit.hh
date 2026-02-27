/***************************************************************************
 *
 * $Id: StMcCtbHit.hh,v 2.5 2011/10/17 00:24:00 fisyak Exp $
 * $Log: StMcCtbHit.hh,v $
 * Revision 2.5  2011/10/17 00:24:00  fisyak
 * Add time of flight for hits
 *
 * Revision 2.4  2005/09/28 21:30:14  fisyak
 * Persistent StMcEvent
 *
 * Revision 2.3  2005/07/06 20:05:28  calderon
 * Remove forward declaration of StThreeVectorF, use #include, and only in
 * StMcHit base class.  StThreeVectorF is not a class anymore, it is now
 * only a typedef, only template version of StThreeVector exists now.
 *
 * Revision 2.2  2005/01/27 23:40:46  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.1  2003/02/19 03:29:41  calderon
 * Introduction of CTB classes to repository.
 *
 * Revision 1.0  2003/03/18 00:00:00  gans
 * Introduction of Ctb classes.  Modified several classes
 * accordingly.
 */
/// @file StMcCtbHit.hh
/// @brief Monte Carlo hit class for the Central Trigger Barrel (CTB).
#ifndef StMcCtbHit_hh
#define StMcCtbHit_hh

#include "StMcHit.hh"
#include "tables/St_g2t_ctf_hit_Table.h"

/// @brief Monte Carlo hit class for the Central Trigger Barrel (CTB).
///
/// Inherits position, momentum, and track information from StMcHit.
/// The volume ID encodes the slat and tray indices.
class StMcCtbHit : public StMcHit {
public:
  /// @brief Default constructor.
  StMcCtbHit(){}
  /// @brief Constructor from hit kinematics, energy, and volume ID.
  StMcCtbHit(const StThreeVectorF& x,const StThreeVectorF& p,
	     Float_t de, Float_t ds, Float_t tof, Long_t k, Long_t volId, StMcTrack* parent=0) : 
    StMcHit(x,p,de,ds,tof,k,volId,parent) {}
  /// @brief Constructor from a GEANT g2t_ctf_hit table row.
  StMcCtbHit(g2t_ctf_hit_st* pt) : StMcHit(StThreeVectorF(pt->x[0], pt->x[1], pt->x[2]),
					   StThreeVectorF(pt->p[0], pt->p[1], pt->p[2]),
					   pt->de, pt->ds, pt->tof, pt->id, pt->volume_id, 0) {}
  /// @brief Destructor.
  virtual ~StMcCtbHit() {}
  /// @brief Decodes the volume ID into slat and tray indices.
  void get_slat_tray(unsigned int & slat, unsigned int & tray) const;
private:
  ClassDef(StMcCtbHit,2)
};

ostream&  operator<<(ostream& os, const StMcCtbHit&);

#endif
