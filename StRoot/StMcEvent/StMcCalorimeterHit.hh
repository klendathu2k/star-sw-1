// $Id: StMcCalorimeterHit.hh,v 2.7 2005/09/28 21:30:14 fisyak Exp $
//
// $Log: StMcCalorimeterHit.hh,v $
// Revision 2.7  2005/09/28 21:30:14  fisyak
// Persistent StMcEvent
//
// Revision 2.6  2005/01/27 23:40:46  calderon
// Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
//
// Revision 2.5  2004/01/13 21:03:34  fisyak
// Replace iostream by Stiostream.h (for icc)
//
// Revision 2.4  2003/10/08 20:17:54  calderon
// -using <iostream>, std::cout, std::ostream.
// -changes in FTPC volume Id.
//   o Causes changes in decoding of plane().
//   o sector() is added.
//   o print volumeId and sector() in the operator<<.
//
// Revision 2.3  2003/09/02 17:58:41  perev
// gcc 3.2 updates + WarnOff
//
// Revision 2.2  2000/06/06 02:58:40  calderon
// Introduction of Calorimeter classes.  Modified several classes
// accordingly.
//
// Revision 2.1  2000/05/05 14:54:19  calderon
// Initial revision
//
//                                                                           
/// @file StMcCalorimeterHit.hh
/// @brief Abstract base class for Monte Carlo calorimeter hits.
#ifndef StMcCalorimeterHit_hh
#define StMcCalorimeterHit_hh

#include "StObject.h"
#include "Stiostream.h"

//#include "StMemoryPool.hh"

class StMcTrack;

/// @brief Abstract base class representing a Monte Carlo hit in a calorimeter detector.
///
/// Stores energy deposit and volume identification (module, eta bin, sub-sector)
/// for simulated hits. Used as the concrete hit type for BEMC and EEMC collections.
class StMcCalorimeterHit : public StObject {
public:
    /// @brief Default constructor.
    StMcCalorimeterHit();
    /// @brief Constructor with module, eta, sub-sector, and energy deposit.
    StMcCalorimeterHit(int, int, int, float);
    /// @brief Constructor with module, eta, sub-sector, energy deposit, and parent track.
    StMcCalorimeterHit(int, int, int, float, StMcTrack*);
    /// @brief Destructor.
    virtual ~StMcCalorimeterHit();
    
    /// @brief Equality operator; returns non-zero if both hits occupy the same cell.
    int operator==(const StMcCalorimeterHit&) const;
    /// @brief Inequality operator; returns non-zero if hits are in different cells.
    int operator!=(const StMcCalorimeterHit&) const;
    /// @brief Accumulates the energy deposit from another hit in the same cell.
    void operator+=(const StMcCalorimeterHit&);

    /// @brief Returns true if this hit and the argument share the same cell (module/eta/sub).
    bool sameCell(const StMcCalorimeterHit&) const;

  // "Get" Methods
    /// @brief Returns the module number.
    virtual int                          module() const;
    /// @brief Returns the eta bin index.
    virtual int                             eta() const;
    /// @brief Returns the sub-sector index.
    virtual int                             sub() const;
    /// @brief Returns the energy deposit (GeV).
    virtual float                            dE() const;
    /// @brief Returns a pointer to the parent Monte Carlo track.
    virtual StMcTrack*              parentTrack() const;	

  // "Set" Methods

    /// @brief Sets the module number.
    virtual void setModule(int);
    /// @brief Sets the eta bin index.
    virtual void setEta(int);
    /// @brief Sets the sub-sector index.
    virtual void setSub(int);
    /// @brief Sets the energy deposit (GeV).
    virtual void setdE(float);
    /// @brief Sets the parent Monte Carlo track.
    virtual void setParentTrack(StMcTrack*);
    
  //  void* operator new(size_t)     { return mPool.alloc(); }
  //  void  operator delete(void* p) { mPool.free(p); }
    
protected:
    int                  mModule;      ///< Module number.
    int                  mEta;         ///< Eta bin index.
    int                  mSub;         ///< Sub-sector index.
    float                mdE;          ///< Energy deposit in GeV.
    StMcTrack*           mParentTrack; ///< Pointer to the parent Monte Carlo track.

  //private:
  //  static StMemoryPool mPool; 
    ClassDef(StMcCalorimeterHit,1)
};

ostream&  operator<<(ostream& os, const StMcCalorimeterHit&);

inline int  StMcCalorimeterHit::module() const {return mModule; }
inline int  StMcCalorimeterHit::eta() const {return mEta; }
inline int  StMcCalorimeterHit::sub() const {return mSub; }
inline float StMcCalorimeterHit::dE()  const {return mdE; }
inline StMcTrack* StMcCalorimeterHit::parentTrack() const {return mParentTrack; }

#endif

