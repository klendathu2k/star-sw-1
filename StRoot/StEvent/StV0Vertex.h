/*!
 * \class StV0Vertex 
 * \author Thomas Ullrich, Sep 1999
 */
/***************************************************************************
 *
 * $Id: StV0Vertex.h,v 2.8 2009/11/23 16:34:08 fisyak Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StV0Vertex.h,v $
 * Revision 2.8  2009/11/23 16:34:08  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.7  2004/07/15 16:36:26  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.6  2002/11/26 02:19:11  perev
 * StEventMaker ITTF modif
 *
 * Revision 2.5  2002/03/08 20:28:37  ullrich
 * Custom Streamer written.
 *
 * Revision 2.4  2002/02/22 22:56:53  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:46  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2001/03/24 03:35:01  perev
 * clone() -> clone() const
 *
 * Revision 2.1  1999/10/28 22:28:04  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.0  1999/10/12 18:43:26  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StV0Vertex_hh
#define StV0Vertex_hh
/// @file StV0Vertex.h
/// @brief Vertex representing a V0 neutral two-prong decay (K0s, Lambda, anti-Lambda, etc.).

#include "StVertex.h"
#include "StContainers.h"


/// @brief Vertex representing a V0 neutral two-prong decay (K0s, Lambda, anti-Lambda, etc.).
///
/// StV0Vertex stores the decay position, momenta of the two daughter tracks at
/// the vertex, DCA quantities, and the type identifier kV0Vtx.  The positive
/// and negative daughters are accessed by StChargeSign.
class StV0Vertex : public StVertex {
public:
    /// @brief Default constructor; initialises all members to zero.
    StV0Vertex();
    // StV0Vertex(const StV0Vertex&);            use default
    // StV0Vertex& operator=(const StV0Vertex&); use default
    ~StV0Vertex();
    
    /// @brief Returns kV0Vtx.
    StVertexId            type() const;
    /// @brief Number of daughter tracks (always 2 for a V0).
    unsigned int          numberOfDaughters() const;
    /// @brief Daughter track of the given charge sign (negative or positive).
    StTrack*              daughter(StChargeSign sign);
    const StTrack*        daughter(StChargeSign sign) const;
    /// @brief Daughter track by index (0 = negative, 1 = positive).
    StTrack*              daughter(unsigned int);
    const StTrack*        daughter(unsigned int) const;
    /// @brief Daughter tracks passing the supplied filter.
    StPtrVecTrack         daughters(StTrackFilter&);
    /// @brief Add a daughter track link (negative first, then positive).
    void                  addDaughter(StTrack*);
    /// @brief Remove a daughter track link.
    void                  removeDaughter(StTrack*);

    /// @brief DCA of the daughter with the given charge sign to the primary vertex (cm).
    float                 dcaDaughterToPrimaryVertex(StChargeSign sign) const;
    /// @brief DCA between the two daughter helices at the V0 decay point (cm).
    float                 dcaDaughters() const;
    /// @brief DCA of the reconstructed V0 candidate to the primary vertex (cm).
    float                 dcaParentToPrimaryVertex() const;
    /// @brief Momentum of the daughter with the given charge sign at the V0 vertex (GeV/c).
    const StThreeVectorF& momentumOfDaughter(StChargeSign sign) const;
    /// @brief Total V0 momentum (vector sum of daughter momenta) (GeV/c).
    StThreeVectorF        momentum() const;
    
    /// @brief Set the DCA of the specified daughter to the primary vertex (cm).
    void setDcaDaughterToPrimaryVertex(StChargeSign, float);
    /// @brief Set the momentum of the specified daughter at the V0 vertex (GeV/c).
    void setMomentumOfDaughter(StChargeSign, const StThreeVectorF&);
    /// @brief Set the DCA between the two daughter helices (cm).
    void setDcaDaughters(float);
    /// @brief Set the DCA of the V0 to the primary vertex (cm).
    void setDcaParentToPrimaryVertex(float);

private:
    StPtrVecTrack    mDaughters;                    ///< Non-owning pointers to the two daughter tracks.
    Float_t          mDcaDaughtersToPrimaryVertex[2]; ///< DCA of each daughter to the primary vertex (cm); [0]=negative, [1]=positive.
    //    StThreeVectorF   mMomentumOfDaughters[2];
    StThreeVectorF   mMomentumOfDaughters[2];       ///< Daughter momenta at the V0 vertex (GeV/c); [0]=negative, [1]=positive.
    Float_t          mDcaDaughters;                 ///< DCA between the two daughter helices at the decay point (cm).
    Float_t          mDcaParentToPrimaryVertex;     ///< DCA of the V0 candidate to the primary vertex (cm).

    ClassDef(StV0Vertex,3)
};
#endif
