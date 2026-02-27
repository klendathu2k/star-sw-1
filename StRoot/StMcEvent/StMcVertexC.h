/***************************************************************************
 *
 * $Id: StMcVertexC.h,v 2.1 2010/05/04 23:58:43 fine Exp $
 * $Log: StMcVertexC.h,v $
 * Revision 2.1  2010/05/04 23:58:43  fine
 * Vertex, and emc models
 *
 * Revision 2.2  2010/04/28 20:15:45  fine
 * Implementation if the new OO for Mc hits
 *
 * Revision 2.1  2010/04/28 18:10:11  fine
 * New OO model for Mc event components
 *
 * Revision 2.10  2005/11/22 21:44:52  fisyak
 * Add compress Print for McEvent, add Ssd collections
  *
 *
 **************************************************************************/
/// @file StMcVertexC.h
/// @brief Concrete MC vertex class wrapping a g2t_vertex_st row, used in EMC/calorimeter contexts.
#ifndef STMCVERTEXT_HH
#define STMCVERTEXT_HH

#include "StEventObject.h"
#include "StMcHitI.h"
#include "g2t_vertex.h"

#include "TString.h"

/// @brief Concrete MC vertex class wrapping a g2t_vertex_st row.
///
/// Implements StMcHitI using a raw pointer to a GEANT vertex table entry.
/// Used in EMC/calorimeter reconstruction contexts as an alternative to StMcVertex.
class StMcVertexC : public StEventObject<g2t_vertex_st*> , public StMcHitI {
protected:
    /// @brief Protected default constructor.
    StMcVertexC(){;}
public:
    /// @brief Constructor from a raw g2t_vertex_st table pointer.
    StMcVertexC(g2t_vertex_st*data) : StEventObject<g2t_vertex_st*>(data) {;}
    // StMcVertexC(const StSvtHit&);                  use default
    // const StMcVertexC & operator=(const StMcVertexC&);   use default
    /// @brief Destructor.
    virtual ~StMcVertexC(){;}
    /// @brief Equality comparison by vertex key.
    int operator==(const StMcVertexC&r) const;
    /// @brief Inequality comparison by vertex key.
    int operator!=(const StMcVertexC&r) const
    { return !operator==(r); }
    // "Get" Methods
    /// @brief Returns the x-coordinate of the vertex (cm).
    virtual float                             x() const { return StEventObject<g2t_vertex_st*>::fData->ge_x[0];   }
    /// @brief Returns the y-coordinate of the vertex (cm).
    virtual float                             y() const { return StEventObject<g2t_vertex_st*>::fData->ge_x[1];   }
    /// @brief Returns the z-coordinate of the vertex (cm).
    virtual float                             z() const { return StEventObject<g2t_vertex_st*>::fData->ge_x[2];   }
    
    /// @brief Returns the GEANT process code that created this vertex.
    virtual long                   geantProcess() const { return StEventObject<g2t_vertex_st*>::fData->ge_proc;   }
    /// @brief Returns the generator process code associated with this vertex.
    virtual long               generatorProcess() const { return StEventObject<g2t_vertex_st*>::fData->eg_proc;   }
    /// @brief Returns the GEANT medium identifier at the vertex position.
    virtual long                    geantMedium() const { return StEventObject<g2t_vertex_st*>::fData->ge_medium; }

    /// @brief Returns the unique vertex key (row ID in the g2t table).
    virtual long                            key() const { return StEventObject<g2t_vertex_st*>::fData->id;        }
    /// @brief Returns the numeric GEANT volume identifier encoded as a long.
    virtual long                       volumeId() const { long tmp;
                                                          memcpy(&tmp, &StEventObject<g2t_vertex_st*>::fData->ge_volume,4);
                                                          return tmp; 
                                                        }
    /// @brief Returns the generator-level time of flight at this vertex (ns).
    virtual float                           tof() const { return StEventObject<g2t_vertex_st*>::fData->eg_tof;    }
    /// @brief Returns the GEANT volume name as a TString.
    TString                         geantVolume() const { char tmp[5]={0};
                                                          strncpy(tmp, StEventObject<g2t_vertex_st*>::fData->ge_volume,4);
                                                          return TString(tmp); 
                                                        }
#if 0
    StPtrVecMcTrack&            daughters()               { return return StEventObject<g2t_vertex_st*>::fData->*&mDaughters; } 
    const StPtrVecMcTrack&      daughters() const         { return return StEventObject<g2t_vertex_st*>::fData->*&mDaughters; } 
    unsigned int                numberOfDaughters()       { return return StEventObject<g2t_vertex_st*>::fData->mDaughters.size(); }
    unsigned int                numberOfDaughters() const { return return StEventObject<g2t_vertex_st*>::fData->mDaughters.size(); }
    StMcTrack*                  daughter(unsigned int i)  { return (i < mDaughters.size() ? mDaughters[i] : 0); }
    const StMcTrack*            daughter(unsigned int i) const { return (i < mDaughters.size() ? mDaughters[i] : 0); }
    const StMcTrack*            parent() const            { return return StEventObject<g2t_vertex_st*>::fData->mParent; }   
#endif
};

#endif

