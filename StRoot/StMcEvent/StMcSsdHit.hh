/***************************************************************************
 *
 * $Id: StMcSsdHit.hh,v 2.6 2011/10/17 00:24:01 fisyak Exp $
 * $Log: StMcSsdHit.hh,v $
 * Revision 2.6  2011/10/17 00:24:01  fisyak
 * Add time of flight for hits
 *
 * Revision 2.5  2005/11/22 21:44:52  fisyak
 * Add compress Print for McEvent, add Ssd collections
 *
 * Revision 2.4  2005/09/28 21:30:15  fisyak
 * Persistent StMcEvent
 *
 * Revision 2.3  2005/07/06 20:05:28  calderon
 * Remove forward declaration of StThreeVectorF, use #include, and only in
 * StMcHit base class.  StThreeVectorF is not a class anymore, it is now
 * only a typedef, only template version of StThreeVector exists now.
 *
 * Revision 2.2  2005/01/27 23:40:47  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.1  2004/09/14 05:00:30  calderon
 * Added support for Ist, Ssd and changes to Pixel, from "El Kai".
 *
 * Revision 2.1  2003/08/20 18:50:21  calderon
 * Addition of Tof classes and SSd classes.  Modified track, event, and
 * container code to reflect this.
 * Fix bug in StMcVertex and in clearing of some hit collections.
 *
 *
 **************************************************************************/
/// @file StMcSsdHit.hh
/// @brief Monte Carlo hit in the STAR Silicon Strip Detector (SSD).

#ifndef StMcSsdHit_hh
#define StMcSsdHit_hh

#include "StMcHit.hh"
#include "tables/St_g2t_ssd_hit_Table.h" 

/// @brief Monte Carlo hit in the Silicon Strip Detector (SSD).
///
/// Extends StMcHit with SSD-specific geometry accessors (ladder and wafer)
/// decoded from the volume ID.
class StMcSsdHit : public StMcHit {
public:
  /// @brief Default constructor.
  StMcSsdHit() {}
  /// @brief Constructor from hit properties.
  StMcSsdHit(const StThreeVectorF& x,const StThreeVectorF& p,
	     Float_t de = 0, Float_t ds = 0, Float_t tof = 0, Long_t k = 0, Long_t volId = 0, StMcTrack* parent=0) : 
    StMcHit(x,p,de,ds,tof,k,volId,parent) {}
  /// @brief Constructor from a GEANT SSD hit table row.
  StMcSsdHit(g2t_ssd_hit_st* pt) : 
    StMcHit(StThreeVectorF(pt->x[0], pt->x[1], pt->x[2]),
	    StThreeVectorF(pt->p[0], pt->p[1], pt->p[2]), 
	    pt->de, pt->ds, pt->tof, pt->id, pt->volume_id, 0) {}
  /// @brief Destructor.
  ~StMcSsdHit() {}
  /// @brief Returns the SSD ladder number decoded from the volume ID.
  ULong_t ladder() const {return  mVolumeId%100;     }
  /// @brief Returns the SSD wafer number decoded from the volume ID.
  ULong_t wafer()  const {return ((mVolumeId%10000-7000)/100)%100;}
  /// @brief Prints hit information.
  virtual void Print(Option_t *option="") const; // *MENU* 
    
private:
    ClassDef(StMcSsdHit,2)
};

ostream&  operator<<(ostream& os, const StMcSsdHit&);


#endif
