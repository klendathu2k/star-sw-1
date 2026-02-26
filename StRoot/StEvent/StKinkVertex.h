/*!
 * \class StKinkVertex 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StKinkVertex.h,v 2.9 2009/11/23 16:34:06 fisyak Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StKinkVertex.h,v $
 * Revision 2.9  2009/11/23 16:34:06  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.8  2004/07/15 16:36:24  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.7  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.6  2001/05/30 17:45:54  perev
 * StEvent branching
 *
 * Revision 2.5  2001/04/05 04:00:38  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.4  2001/03/24 03:34:52  perev
 * clone() -> clone() const
 *
 * Revision 2.3  1999/11/04 16:33:26  ullrich
 * Fixed wrong option to ClassDef macro.
 *
 * Revision 2.2  1999/10/28 22:25:56  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:43:22  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StKinkVertex_hh
#define StKinkVertex_hh
/// @file StKinkVertex.h
/// @brief Vertex representing a charged-particle kink decay topology (one parent, one daughter).

#include "StVertex.h"

class StParticleDefinition;

/// @brief Vertex representing a kink decay topology: one parent track decaying to one charged daughter.
///
/// A kink occurs when a charged particle (typically K± or π±) decays in flight
/// to a single detectable charged daughter.  StKinkVertex stores the decay point,
/// momenta of parent and daughter at the vertex, DCA quantities, and GEANT particle
/// hypotheses for both tracks.
class StKinkVertex : public StVertex {
public:
    /// @brief Default constructor; initialises all members to zero.
    StKinkVertex();
    // StKinkVertex(const StKinkVertex&);            use default
    // StKinkVertex& operator=(const StKinkVertex&); use default
    ~StKinkVertex();
    
    /// @brief Returns kKinkVtx.
    StVertexId     type() const;
    /// @brief Number of daughter tracks (always 1 for a kink).
    unsigned int   numberOfDaughters() const;
    /// @brief The single daughter track (index ignored beyond 0).
    StTrack*       daughter(unsigned int = 0);
    const StTrack* daughter(unsigned int = 0) const;
    /// @brief Daughter tracks passing the supplied filter.
    StPtrVecTrack  daughters(StTrackFilter&);

    /// @brief Particle hypothesis for the parent track.
    StParticleDefinition* pidParent() const;
    /// @brief Particle hypothesis for the daughter track.
    StParticleDefinition* pidDaughter() const;
    /// @brief GEANT particle ID of the parent track.
    unsigned short        geantIdParent() const;
    /// @brief GEANT particle ID of the daughter track.
    unsigned short        geantIdDaughter() const;
    /// @brief DCA between the parent and daughter track helices at the kink point (cm).
    float                 dcaParentDaughter() const;
    /// @brief DCA of the daughter track to the primary vertex (cm).
    float                 dcaDaughterPrimaryVertex() const;
    /// @brief DCA of the parent track to the primary vertex (cm).
    float                 dcaParentPrimaryVertex() const;
    /// @brief Distance between the last hit on the parent and the first hit on the daughter (cm).
    float                 hitDistanceParentDaughter() const;
    /// @brief Distance between the last hit on the parent track and the kink vertex (cm).
    float                 hitDistanceParentVertex() const;
    /// @brief Energy loss difference for decay hypothesis index i (0–2).
    float                 dE(unsigned int i) const;
    /// @brief Decay angle of the daughter in the laboratory frame (rad).
    float                 decayAngle() const;
    /// @brief Decay angle of the daughter in the parent rest frame (rad).
    float                 decayAngleCM() const;
    /// @brief Momentum of the parent track at the kink vertex (GeV/c).
    const StThreeVectorF& parentMomentum() const;
    StThreeVectorF&       parentMomentum();
    /// @brief Momentum of the daughter track at the kink vertex (GeV/c).
    const StThreeVectorF& daughterMomentum() const;
    StThreeVectorF&       daughterMomentum();

    /// @brief Set the GEANT particle ID of the parent.
    void setGeantIdParent(unsigned short);
    /// @brief Set the GEANT particle ID of the daughter.
    void setGeantIdDaughter(unsigned short);
    /// @brief Set the parent–daughter DCA (cm).
    void setDcaParentDaughter(float);
    /// @brief Set the DCA of the daughter to the primary vertex (cm).
    void setDcaDaughterPrimaryVertex(float);
    /// @brief Set the DCA of the parent to the primary vertex (cm).
    void setDcaParentPrimaryVertex(float);
    /// @brief Set the hit distance between parent and daughter (cm).
    void setHitDistanceParentDaughter(float);
    /// @brief Set the hit distance between parent last hit and kink vertex (cm).
    void setHitDistanceParentVertex(float);
    /// @brief Set the energy loss difference for hypothesis index i.
    void setdE(unsigned int, float);
    /// @brief Set the decay angle in the lab frame (rad).
    void setDecayAngle(float);
    /// @brief Set the decay angle in the parent rest frame (rad).
    void setDecayAngleCM(float);
    /// @brief Set the parent momentum at the kink vertex (GeV/c).
    void setParentMomentum(const StThreeVectorF&);
    /// @brief Set the daughter momentum at the kink vertex (GeV/c).
    void setDaughterMomentum(const StThreeVectorF&);
    /// @brief Add the daughter track link to this vertex.
    void addDaughter(StTrack*);
    /// @brief Remove the daughter track link from this vertex.
    void removeDaughter(StTrack*);

protected:
//    StTrack*       mDaughter;         //$LINK
#ifdef __CINT__
    StObjLink        mDaughter;		///< Link to the single daughter track.
#else
    StLink<StTrack>  mDaughter;		///< Link to the single daughter track.
#endif //__CINT__
    UShort_t       mParentGeantId;              ///< GEANT particle ID of the parent track.
    UShort_t       mDaughterGeantId;            ///< GEANT particle ID of the daughter track.
    Float_t        mDcaParentDaughter;          ///< DCA between parent and daughter helices at the kink point (cm).
    Float_t        mDcaDaughterPrimaryVertex;   ///< DCA of the daughter to the primary vertex (cm).
    Float_t        mDcaParentPrimaryVertex;     ///< DCA of the parent to the primary vertex (cm).
    Float_t        mHitDistanceParentDaughter;  ///< Distance between last parent hit and first daughter hit (cm).
    Float_t        mHitDistanceParentVertex;    ///< Distance between last parent hit and kink vertex (cm).
    Float_t        mDeltaEnergy[3];             ///< Energy loss differences for three decay hypotheses.
    Float_t        mDecayAngle;                 ///< Decay angle in the laboratory frame (rad).
    Float_t        mDecayAngleCM;               ///< Decay angle in the parent rest frame (rad).
    StThreeVectorF mParentMomentum;             ///< Parent track momentum at the kink vertex (GeV/c).
    StThreeVectorF mDaughterMomentum;           ///< Daughter track momentum at the kink vertex (GeV/c).

    ClassDef(StKinkVertex,2)
};
#endif
