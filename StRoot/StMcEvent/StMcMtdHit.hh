//                                                                           
/// @file StMcMtdHit.hh
/// @brief Monte Carlo hit in the Muon Telescope Detector (MTD).
#ifndef StMcMtdHit_hh
#define StMcMtdHit_hh

#include "StObject.h"
#include "Stiostream.h"
#include "StThreeVectorF.hh"
#include "StMcHit.hh"

//#include "StMemoryPool.hh"

class StMcTrack;
class g2t_mtd_hit_st;


/// @brief Monte Carlo hit in the Muon Telescope Detector (MTD).
///
/// Extends StMcHit with backleg/module/cell geometry identifiers, Geant path
/// length, and parent track ID for a single simulated MTD hit.
class StMcMtdHit : public StMcHit {
public:
  /// @brief Default constructor.
  StMcMtdHit();
  /// @brief Constructor from a g2t_mtd_hit_st Geant table row.
  StMcMtdHit(g2t_mtd_hit_st*);

  /// @brief Destructor.
  virtual ~StMcMtdHit();
    
    /// @brief Inequality comparison.
    int operator!=(const StMcMtdHit&) const;

    /// @brief Returns true if both hits occupy the same backleg/module/cell.
    bool sameCell(const StMcMtdHit&) const;

  // "Get" Methods
    /// @brief Returns the MTD backleg number.
    virtual int                         backleg() const;
    /// @brief Returns the module number within the backleg.
    virtual int                          module() const;
    /// @brief Returns the cell number within the module.
    virtual int                            cell() const;
    /// @brief Returns the Geant track path length through active material (cm).
    virtual float                    pathLength() const;
    /// @brief Returns the parent MC track identifier.
    virtual int                   parentTrackId() const;	

  // "Set" Methods

  //  void* operator new(size_t)     { return mPool.alloc(); }
  //  void  operator delete(void* p) { mPool.free(p); }
    
protected:
    int                  mBackleg;     ///< MTD backleg number.
    int                  mModule;      ///< Module number within the backleg.
    int                  mCell;        ///< Cell number within the module.
    float                mPathLength;  ///< Track path length through active material (cm).
    int			 mParentTrackId; ///< Parent MC track identifier.

    ClassDef(StMcMtdHit,1)//
};

ostream&  operator<<(ostream& os, const StMcMtdHit&);

inline   int StMcMtdHit::backleg()	const {return mBackleg; 	}
inline   int StMcMtdHit::module() 	const {return mModule; 		}
inline   int StMcMtdHit::cell() 	const {return mCell; 		}
inline float StMcMtdHit::pathLength()  	const {return mPathLength; 	}
inline   int StMcMtdHit::parentTrackId()const {return mParentTrackId; 	}

#endif

