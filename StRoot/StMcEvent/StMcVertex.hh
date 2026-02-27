/***************************************************************************
 *
 * $Id: StMcVertex.hh,v 2.13 2005/11/22 21:44:52 fisyak Exp $
 * $Log: StMcVertex.hh,v $
 * Revision 2.13  2005/11/22 21:44:52  fisyak
 * Add compress Print for McEvent, add Ssd collections
 *
 * Revision 2.12  2005/09/28 21:30:15  fisyak
 * Persistent StMcEvent
 *
 * Revision 2.11  2005/01/27 23:40:49  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.10  2004/01/13 21:03:34  fisyak
 * Replace iostream by Stiostream.h (for icc)
 *
 * Revision 2.9  2003/12/04 05:56:47  calderon
 * Inclusion of Endcap EMC hit collection in StMcEvent and
 * of the Endcap hit vector in StMcTrack.
 * fix const of StMcVertex::parent() to avoid warnings in user code
 *
 * Revision 2.8  2003/10/08 20:17:55  calderon
 * -using <iostream>, std::cout, std::ostream.
 * -changes in FTPC volume Id.
 *   o Causes changes in decoding of plane().
 *   o sector() is added.
 *   o print volumeId and sector() in the operator<<.
 *
 * Revision 2.7  2003/09/02 17:58:41  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.6  2003/08/20 18:50:21  calderon
 * Addition of Tof classes and Pixel classes.  Modified track, event, and
 * container code to reflect this.
 * Fix bug in StMcVertex and in clearing of some hit collections.
 *
 * Revision 2.5  2000/03/29 16:15:55  calderon
 * Added more information from g2t_vertex table
 *
 * Revision 2.4  2000/01/18 20:52:31  calderon
 * Works with CC5
 *
 * Revision 2.3  1999/12/14 07:04:50  calderon
 * Numbering scheme as per SVT request.
 *
 * Revision 2.2  1999/12/03 00:51:53  calderon
 * Tested with new StMcEventMaker.  Added messages for
 * diagnostics.
 *
 * Revision 2.1  1999/11/19 19:06:34  calderon
 * Recommit after redoing the files.
 *
 * Revision 2.0  1999/11/17 02:12:17  calderon
 * Completely revised for new StEvent
 *
 * Revision 1.4  1999/09/24 01:23:17  fisyak
 * Reduced Include Path
 *
 * Revision 1.3  1999/09/23 21:25:55  calderon
 * Added Log & Id
 * Modified includes according to Yuri
 *
 *
 **************************************************************************/
/// @file StMcVertex.hh
/// @brief Monte Carlo vertex class representing a particle interaction point.
#ifndef StMcVertex_hh
#define StMcVertex_hh
#include "StMcContainers.hh"
#include "Stiostream.h"
#include "StObject.h"
#include "TString.h"
#include "StThreeVectorF.hh"

class g2t_vertex_st;

/// @brief Monte Carlo vertex representing a particle production or decay point.
class StMcVertex : public StObject {
public:
    /// @brief Default constructor.
    StMcVertex();
    /// @brief Construct from a GEANT g2t_vertex table row.
    StMcVertex(g2t_vertex_st*);
    ~StMcVertex();
    
    int operator==(const StMcVertex&) const;
    int operator!=(const StMcVertex&) const;
    
    // "Get" Methods
    
  /// @brief Return the 3-D position of the vertex (cm).
  const StThreeVectorF&       position() const          { return *&mPosition;}
  /// @brief Return the list of daughter tracks produced at this vertex.
  StPtrVecMcTrack&            daughters()               { return *&mDaughters; } 
  const StPtrVecMcTrack&      daughters() const         { return *&mDaughters; } 
  /// @brief Return the number of daughter tracks.
  unsigned int                numberOfDaughters()       { return mDaughters.size(); }
  unsigned int                numberOfDaughters() const { return mDaughters.size(); }
  /// @brief Return the i-th daughter track, or nullptr if out of range.
  StMcTrack*                  daughter(unsigned int i)  { return (i < mDaughters.size() ? mDaughters[i] : 0); }
  const StMcTrack*            daughter(unsigned int i) const { return (i < mDaughters.size() ? mDaughters[i] : 0); }
  /// @brief Return the parent (incoming) track that created this vertex.
  const StMcTrack*            parent() const            { return mParent; }   
  /// @brief Return the GEANT volume name where the vertex occurred.
  TString const               &geantVolume() const      { return *&mGeantVolume; }
  /// @brief Return the global time of flight at which the vertex occurred (ns).
  float tof() const { return mTof; }  
  /// @brief Return the GEANT process code that created the vertex.
  long geantProcess() const { return mGeantProcess; }      
  /// @brief Return the GEANT medium ID where the vertex occurred.
  long geantMedium() const { return mGeantMedium; }      
  /// @brief Return the generator-level process code.
  long generatorProcess() const { return mGeneratorProcess; }      
  /// @brief Return the primary key (g2t row index) of this vertex.
  long key() const { return mKey; }      
    
  // "Set" Methods
  
    /// @brief Set the 3-D position of the vertex.
    void setPosition(const StThreeVectorF&);
    /// @brief Set the parent track pointer.
    void setParent(StMcTrack* );         
    /// @brief Add a daughter track to the vertex.
    void addDaughter(StMcTrack*);
    /// @brief Set the GEANT volume name.
    void setGeantVolume(const Char_t *name);
    /// @brief Set the time of flight at the vertex.
    void setTof(float);
    /// @brief Set the GEANT process code.
    void setGeantProcess(int); 
    /// @brief Remove a daughter track from the vertex.
    void removeDaughter(StMcTrack*);
    virtual void Print(Option_t *option="") const; // *MENU* 
protected:
    
    StThreeVectorF       mPosition;          ///< 3-D position of the vertex (cm)
    StPtrVecMcTrack      mDaughters;         ///< Tracks produced at this vertex
    StMcTrack*           mParent;            ///< Parent track that created this vertex
    TString              mGeantVolume;       ///< GEANT volume name where vertex occurred
    float                mTof;              ///< Global time of flight at the vertex (ns)
    long                 mGeantProcess;      ///< GEANT process code
    long                 mGeneratorProcess;  ///< Generator-level process code
    long                 mKey;              ///< Primary key (g2t row index)
    long                 mGeantMedium;       ///< GEANT medium ID
    ClassDef(StMcVertex,1)
};
ostream&  operator<<(ostream& os, const StMcVertex&);
#endif
