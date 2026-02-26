/*!
 * \class StXiVertex 
 * \author Gene Van Buren, Feb 1999, revised Thomas Ullrich Sep 99
 */
/***************************************************************************
 *
 * $Id: StXiVertex.h,v 2.10 2009/11/23 16:34:08 fisyak Exp $
 *
 * Author: Gene Van Buren, Feb 1999, revised Thomas Ullrich Sep 99
 ***************************************************************************
 *
 * Description: vertex class for cascades
 *
 ***************************************************************************
 *
 * $Log: StXiVertex.h,v $
 * Revision 2.10  2009/11/23 16:34:08  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.9  2004/07/15 16:36:26  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.8  2003/01/24 22:30:05  genevb
 * Allow for signed DCA of Xi to PrimVertex
 *
 * Revision 2.7  2002/02/22 22:56:53  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.6  2001/06/10 21:03:32  perev
 * Solaris: consting
 *
 * Revision 2.5  2001/05/30 17:45:55  perev
 * StEvent branching
 *
 * Revision 2.4  2001/04/05 04:00:47  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.3  2001/03/24 03:35:01  perev
 * clone() -> clone() const
 *
 * Revision 2.2  1999/11/04 13:31:19  ullrich
 * Changed order of constructor arguments
 *
 * Revision 2.1  1999/10/28 22:28:18  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.0  1999/10/12 18:43:37  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StXiVertex_hh
#define StXiVertex_hh
/// @file StXiVertex.h
/// @brief Vertex representing a cascade (Xi or Omega) decay: bachelor track plus an associated V0.

#include "StVertex.h"
#include "StEnumerations.h"

class StV0Vertex;

/// @brief Vertex representing a cascade (Ξ or Ω) decay: one bachelor charged track plus an associated V0.
///
/// StXiVertex stores the secondary decay position, bachelor-track momentum,
/// link to the associated StV0Vertex, and DCA quantities for the cascade
/// hypothesis reconstruction (Ξ→Λπ, Ω→ΛK, etc.).
class StXiVertex : public StVertex {
public:
    /// @brief Default constructor; initialises all members to zero.
    StXiVertex();
    // StXiVertex(const StXiVertex&);            use default
    // StXiVertex& operator=(const StXiVertex&); use default
    ~StXiVertex();

    /// @brief Returns kXiVtx.
    StVertexId            type() const;
    /// @brief Number of bachelor daughter tracks (always 1).
    unsigned int          numberOfDaughters() const;
    /// @brief The bachelor (charged) daughter track.
    StTrack*              daughter(unsigned int = 0);
    const StTrack*        daughter(unsigned int = 0) const;
    /// @brief Bachelor tracks passing the supplied filter.
    StPtrVecTrack         daughters(StTrackFilter&);

    /// @brief DCA of the bachelor track to the primary vertex (cm).
    float                 dcaBachelorToPrimaryVertex() const;
    /// @brief DCA of the associated V0 to the primary vertex (cm).
    float                 dcaV0ToPrimaryVertex() const;
    /// @brief DCA between the bachelor track and the V0 at the Xi decay point (cm).
    float                 dcaDaughters() const;
    /// @brief DCA of the Xi candidate to the primary vertex (unsigned, cm).
    float                 dcaParentToPrimaryVertex() const;
    /// @brief Signed DCA of the Xi candidate to the primary vertex (cm).
    float                 signedDcaParentToPrimaryVertex() const;
    /// @brief Momentum of the bachelor track at the Xi vertex (GeV/c).
    const StThreeVectorF& momentumOfBachelor() const;
    /// @brief Reconstructed momentum of the V0 at the Xi vertex (GeV/c).
    StThreeVectorF        momentumOfV0() const;
    /// @brief Total Xi momentum (bachelor + V0) (GeV/c).
    StThreeVectorF        momentum() const;
    /// @brief Associated V0 vertex (Λ or K0s decay point).
    StV0Vertex*           v0Vertex();
    /// @brief The bachelor track.
    StTrack*              bachelor();
    /// @brief Electric charge of the bachelor track.
    double                chargeOfBachelor();

    /// @brief Set the DCA of the bachelor to the primary vertex (cm).
    void setDcaBachelorToPrimaryVertex(float);
    /// @brief Set the bachelor track momentum at the Xi vertex (GeV/c).
    void setMomentumOfBachelor(const StThreeVectorF&);
    /// @brief Set the DCA between the bachelor and the V0 (cm).
    void setDcaDaughters(float);
    /// @brief Set the DCA of the Xi to the primary vertex (cm).
    void setDcaParentToPrimaryVertex(float);
    /// @brief Set the associated V0 vertex link.
    void setV0Vertex(StV0Vertex*);
    /// @brief Add the bachelor track link.
    void addDaughter(StTrack*);
    /// @brief Remove the bachelor track link.
    void removeDaughter(StTrack*);

protected:
//  StTrack*               mDaughter;                   //$LINK
#ifdef __CINT__
    StObjLink              mDaughter;                   ///< Link to the bachelor (charged) daughter track.
#else
    StLink<StTrack>        mDaughter;                   ///< Link to the bachelor (charged) daughter track.
#endif //__CINT__
    Float_t                mDcaBachelorToPrimaryVertex; ///< DCA of the bachelor to the primary vertex (cm).
    StThreeVectorF         mMomentumOfBachelor;         ///< Bachelor track momentum at the Xi vertex (GeV/c).
    Float_t                mDcaDaughters;               ///< DCA between the bachelor and the V0 (cm).
    Float_t                mDcaParentToPrimaryVertex;   ///< DCA of the Xi candidate to the primary vertex (cm).
//  StV0Vertex*            mV0Vertex;                   //$LINK
#ifdef __CINT__
    StObjLink              mV0Vertex;                   ///< Link to the associated V0 vertex.
#else
    StLink<StV0Vertex>     mV0Vertex;                   ///< Link to the associated V0 vertex.
#endif //__CINT__
    ClassDef(StXiVertex,2)
};
#endif
