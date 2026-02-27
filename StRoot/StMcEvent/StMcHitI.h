#ifndef STAR_STMCHITI
#define STAR_STMCHITI
/// @file StMcHitI.h
/// @brief Pure interface for Monte Carlo hit objects, enabling hit matching with reconstruction.
#include "StThreeVectorF.hh"
// MS event hit intergace

/// @brief Pure abstract interface that all typed MC hit classes implement.
///
/// Provides a uniform accessor API over position, momentum, energy loss, step length,
/// key, volume ID, time-of-flight, and parent track index.  The kMatched bit flag
/// records whether a matching reconstructed hit has been found.
class StMcHitI {
  public:
    /// @brief Bit flags for hit matching status.
    enum EMcHitBits {
       kMatched = BIT(23) ///< Set when this hit has been matched to a reconstructed hit
    };
    StMcHitI() {}
    virtual ~StMcHitI() {}
    /// @brief Return the 3-D position of the hit assembled from x(), y(), z() components.
    virtual StThreeVectorF             position() const;
    /// @brief Return the global x coordinate of the hit (cm).
    virtual float                             x() const  =0;
    /// @brief Return the global y coordinate of the hit (cm).
    virtual float                             y() const  =0;
    /// @brief Return the global z coordinate of the hit (cm).
    virtual float                             z() const  =0;
    /// @brief Return the local momentum assembled from px(), py(), pz() components.
    virtual StThreeVectorF         localMomentum() const;
    /// @brief Return the local momentum x component (GeV/c).
    virtual float                            px() const  =0;
    /// @brief Return the local momentum y component (GeV/c).
    virtual float                            py() const  =0;
    /// @brief Return the local momentum z component (GeV/c).
    virtual float                            pz() const  =0;
    /// @brief Return the energy deposited in the sensitive volume (GeV).
    virtual float                            dE() const  =0;
    /// @brief Return the step length through the sensitive volume (cm).
    virtual float                            dS() const  =0;
    /// @brief Return the g2t primary key for this hit.
    virtual long                            key() const  =0;
    /// @brief Return the detector volume ID (encoding is detector-specific).
    virtual long                       volumeId() const  =0;
    /// @brief Return the global time of flight at the hit (ns).
    virtual float                           tof() const  =0;

    /// @brief Return the index of the parent track in the g2t_track table.
    virtual long               parentTrackIndex() const = 0;
};

#define MCHITCLASS(CLASSNAME, TYPE)                       \
class CLASSNAME : public StMcHitT<TYPE*> {                \
protected:                                                \
     CLASSNAME(){;}                                       \
public:                                                   \
    CLASSNAME(TYPE*data) :StMcHitT<TYPE*>(data) {;}       \
    virtual ~CLASSNAME(){;}

#define ENDMCHITCLASS };

#endif
